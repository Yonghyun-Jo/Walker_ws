#!/usr/bin/env python3
"""Motion data control GUI for P73 Walker (vision student, 25D sparse command).

Standalone PyQt5 window that controls motion data playback. Supports a PLAYLIST:
queue several motions and play them back-to-back with smooth seam blending so the
policy executes a sequence of behaviours without a command discontinuity.

Publishes /p73/motion_cmd (25D sparse student command) for the policy and
/p73/ghost_state (20D absolute pose) for MuJoCo ghost visualization.

Sparse command layout (25D, yaw-local; anchor=base_link, feet=[L_Foot, R_Foot]):
    [0:2]  root_vel_local_xy   [2] root_z   [3] roll  [4] pitch  [5] yaw_rate
    [6:12]  feet_pos_local   (L xyz, R xyz; yaw-local, base_link-anchor-relative)
    [12:24] feet_ori_rot6d   (L Rot6D, R Rot6D; yaw-local)
    [24]    waist_yaw_ref

Usage:
    walker
    python3 ~/ros2_ws/src/p73_cc/scripts/motion_gui.py
"""

import glob
import os
import pickle
import sys

import numpy as np

# ROS 2
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray

# Qt
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtWidgets import (
    QApplication, QCheckBox, QComboBox, QDoubleSpinBox, QHBoxLayout, QLabel,
    QListWidget, QPushButton, QSlider, QVBoxLayout, QWidget,
)

# Bodies referenced by the sparse command (must match mimic_env.py tracked cfg).
ANCHOR_BODY = "base_link"
FOOT_BODIES = ["L_Foot_Link", "R_Foot_Link"]
NUM_SPARSE_CMD = 25


# ── Motion math (mirrors motion_cmd_publisher.py / calc_sparse_command_proprio) ──

def quat_rotate_inverse(q_wxyz, v):
    w, x, y, z = q_wxyz
    t = 2.0 * np.cross(np.array([-x, -y, -z]), v)
    return v + w * t + np.cross(np.array([-x, -y, -z]), t)


def extract_yaw_quat(q_wxyz):
    w, x, y, z = q_wxyz
    yaw = np.arctan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z))
    return np.array([np.cos(yaw / 2), 0.0, 0.0, np.sin(yaw / 2)])


def _quat_mul_wxyz(a, b):
    """Hamilton product of two quaternions in wxyz convention."""
    aw, ax, ay, az = a
    bw, bx, by, bz = b
    return np.array([
        aw * bw - ax * bx - ay * by - az * bz,
        aw * bx + ax * bw + ay * bz - az * by,
        aw * by - ax * bz + ay * bw + az * bx,
        aw * bz + ax * by - ay * bx + az * bw,
    ])


def quat_conjugate(q_wxyz):
    w, x, y, z = q_wxyz
    return np.array([w, -x, -y, -z])


def quat_to_rot6d(q_wxyz):
    """(4,) wxyz -> (6,) Rot6D (first two rotation-matrix columns)."""
    w, x, y, z = q_wxyz
    col0 = np.array([1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y)])
    col1 = np.array([2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x)])
    return np.concatenate([col0, col1])


def euler_from_quat(q_wxyz):
    w, x, y, z = q_wxyz
    roll = np.arctan2(2.0 * (w * x + y * z), 1.0 - 2.0 * (x * x + y * y))
    sinp = np.clip(2.0 * (w * y - z * x), -1.0, 1.0)
    pitch = np.arcsin(sinp)
    yaw = np.arctan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z))
    return roll, pitch, yaw


def renorm_rot6d(v6):
    """Gram-Schmidt re-orthonormalize a (possibly blended) 6D rotation rep."""
    c0 = v6[:3]
    c1 = v6[3:]
    n0 = np.linalg.norm(c0)
    b0 = c0 / n0 if n0 > 1e-8 else np.array([1.0, 0.0, 0.0])
    c1 = c1 - np.dot(c1, b0) * b0
    n1 = np.linalg.norm(c1)
    b1 = c1 / n1 if n1 > 1e-8 else np.array([0.0, 1.0, 0.0])
    return np.concatenate([b0, b1])


def compute_sparse_cmd(root_pos, root_rot_wxyz, root_vel_w, root_ang_vel_w,
                       anchor_pos_w, feet_pos_w, feet_quat_w, waist_yaw):
    """Compute 25D sparse student command — mirrors calc_sparse_command_proprio.

    feet_pos_w: (2,3) world positions [L, R]; feet_quat_w: (2,4) wxyz [L, R].
    """
    yaw_quat = extract_yaw_quat(root_rot_wxyz)
    yaw_inv = quat_conjugate(yaw_quat)
    root_vel_local = quat_rotate_inverse(yaw_quat, root_vel_w)
    roll, pitch, _ = euler_from_quat(root_rot_wxyz)

    cmd = np.zeros(NUM_SPARSE_CMD)
    cmd[0] = root_vel_local[0]
    cmd[1] = root_vel_local[1]
    cmd[2] = root_pos[2]
    cmd[3] = roll
    cmd[4] = pitch
    cmd[5] = root_ang_vel_w[2]
    for k in range(2):
        cmd[6 + 3 * k: 9 + 3 * k] = quat_rotate_inverse(yaw_quat, feet_pos_w[k] - anchor_pos_w)
    for k in range(2):
        q_local = _quat_mul_wxyz(yaw_inv, feet_quat_w[k])
        cmd[12 + 6 * k: 18 + 6 * k] = quat_to_rot6d(q_local)
    cmd[24] = waist_yaw
    return cmd


def blend_sparse_cmd(cmd_a, cmd_b, alpha):
    """Cross-fade two 25D sparse commands; Rot6D blocks are re-orthonormalized.

    Linear in all scalar/positional channels (gentle over a short window); the two
    feet Rot6D blocks [12:18],[18:24] are Gram-Schmidt-renormalized so they stay
    valid rotation features for the policy.
    """
    out = (1.0 - alpha) * cmd_a + alpha * cmd_b
    out[12:18] = renorm_rot6d(out[12:18])
    out[18:24] = renorm_rot6d(out[18:24])
    return out


# ── Motion data loader ──────────────────────────────────────────────

class MotionData:
    """Loads and precomputes a motion pkl file (root + body-level, 25D-ready)."""

    def __init__(self, pkl_path: str):
        self.path = pkl_path
        self.name = os.path.basename(pkl_path)
        with open(pkl_path, "rb") as f:
            data = pickle.load(f)

        self.fps = int(data["fps"])
        self.root_pos = np.array(data["root_pos"], dtype=np.float64)
        # xyzw → wxyz
        rot_raw = np.array(data["root_rot"], dtype=np.float64)
        self.root_rot = np.zeros_like(rot_raw)
        self.root_rot[:, 0] = rot_raw[:, 3]
        self.root_rot[:, 1] = rot_raw[:, 0]
        self.root_rot[:, 2] = rot_raw[:, 1]
        self.root_rot[:, 3] = rot_raw[:, 2]
        self.dof_pos = np.array(data["dof_pos"], dtype=np.float64)
        self.num_frames = len(self.root_pos)

        # Body-level data for feet pos/ori (25D sparse command).
        conv = str(data.get("body_quat_convention", "wxyz"))
        if conv != "wxyz":
            raise ValueError(f"{self.name}: expected body_quat_convention 'wxyz', got {conv!r}")
        self.body_pos_w = np.array(data["body_pos_w"], dtype=np.float64)    # (N, B, 3)
        self.body_quat_w = np.array(data["body_quat_w"], dtype=np.float64)  # (N, B, 4) wxyz
        body_list = [str(b) for b in data["body_link_list"]]
        self.anchor_idx = body_list.index(ANCHOR_BODY)
        self.feet_idx = [body_list.index(fn) for fn in FOOT_BODIES]

        # ── Align motion data to robot spawn direction (ghost placement) ──
        # Motion faces -Y (yaw≈-90°); robot spawns facing +X. Rotate +90° about Z,
        # anchor frame 0 beside the robot (y=+0.5m). The sparse command is yaw-local,
        # so this alignment leaves the command unchanged — it only places the ghost.
        self._align_to_robot(yaw_offset_deg=90.0, ghost_origin=np.array([0.0, 0.5, 0.0]))

        dt = 1.0 / self.fps
        self.root_vel = np.zeros_like(self.root_pos)
        self.root_vel[1:] = (self.root_pos[1:] - self.root_pos[:-1]) / dt
        self.root_vel[0] = self.root_vel[1]

        self.root_ang_vel = np.zeros((self.num_frames, 3))
        for i in range(1, self.num_frames):
            r0, p0, y0 = euler_from_quat(self.root_rot[i - 1])
            r1, p1, y1 = euler_from_quat(self.root_rot[i])
            self.root_ang_vel[i] = np.array([r1 - r0, p1 - p0, y1 - y0]) / dt
        self.root_ang_vel[0] = self.root_ang_vel[1]

        self.duration = self.num_frames / self.fps
        self._standing = self._compute_cmd(0, zero_vel=True)

    def _align_to_robot(self, yaw_offset_deg: float, ghost_origin: np.ndarray):
        """Rotate root + bodies by yaw_offset around Z, then translate so frame 0
        starts at ghost_origin. Bodies are transformed consistently with root so the
        feet (25D) are computed in the same frame as the root."""
        theta = np.radians(yaw_offset_deg)
        cos_t, sin_t = np.cos(theta), np.sin(theta)
        rot2d = np.array([[cos_t, -sin_t], [sin_t, cos_t]])

        # Rotate root xy (keep z)
        xy_rotated = (rot2d @ self.root_pos[:, :2].T).T
        self.root_pos[:, 0] = xy_rotated[:, 0]
        self.root_pos[:, 1] = xy_rotated[:, 1]
        offset_xy = ghost_origin[:2] - self.root_pos[0, :2]
        self.root_pos[:, 0] += offset_xy[0]
        self.root_pos[:, 1] += offset_xy[1]

        # Rotate body positions xy + same translation (rigid). (N, B, 2)
        bxy = self.body_pos_w[:, :, :2]
        bxy_rot = np.einsum("ij,nbj->nbi", rot2d, bxy)
        self.body_pos_w[:, :, 0] = bxy_rot[:, :, 0] + offset_xy[0]
        self.body_pos_w[:, :, 1] = bxy_rot[:, :, 1] + offset_xy[1]

        # Rotate quaternions: q_new = q_z(theta) * q_old
        qz = np.array([np.cos(theta / 2), 0.0, 0.0, np.sin(theta / 2)])
        for i in range(self.num_frames):
            self.root_rot[i] = _quat_mul_wxyz(qz, self.root_rot[i])
            for b in range(self.body_quat_w.shape[1]):
                self.body_quat_w[i, b] = _quat_mul_wxyz(qz, self.body_quat_w[i, b])

    def _compute_cmd(self, frame: int, zero_vel: bool = False) -> np.ndarray:
        f = int(np.clip(frame, 0, self.num_frames - 1))
        rv = np.zeros(3) if zero_vel else self.root_vel[f]
        rav = np.zeros(3) if zero_vel else self.root_ang_vel[f]
        return compute_sparse_cmd(
            self.root_pos[f], self.root_rot[f], rv, rav,
            self.body_pos_w[f, self.anchor_idx],
            self.body_pos_w[f, self.feet_idx],
            self.body_quat_w[f, self.feet_idx],
            self.dof_pos[f, -1],
        )

    def get_sparse_cmd(self, frame: int) -> np.ndarray:
        return self._compute_cmd(frame)

    def get_ghost_state(self, frame: int) -> np.ndarray:
        f = int(np.clip(frame, 0, self.num_frames - 1))
        ghost = np.zeros(20)
        ghost[0:3] = self.root_pos[f]
        ghost[3:7] = self.root_rot[f]
        ghost[7:20] = self.dof_pos[f, :13]
        return ghost

    def get_standing_cmd(self) -> np.ndarray:
        return self._standing.copy()


# ── Playlist (sequential playback with seam blending) ───────────────

class Playlist:
    """Ordered MotionData list. Maps a global elapsed time to a (cmd, ghost),
    cross-fading between consecutive motions over `blend` seconds."""

    def __init__(self):
        self.items: list[MotionData] = []
        self.blend = 0.3  # transition seconds between motions
        self._loaded: dict[str, MotionData] = {}

    def add(self, path: str):
        md = self._loaded.get(path)
        if md is None:
            md = MotionData(path)
            self._loaded[path] = md
        self.items.append(md)

    def remove(self, idx: int):
        if 0 <= idx < len(self.items):
            self.items.pop(idx)

    def clear(self):
        self.items = []

    def move(self, idx: int, delta: int):
        j = idx + delta
        if 0 <= idx < len(self.items) and 0 <= j < len(self.items):
            self.items[idx], self.items[j] = self.items[j], self.items[idx]
            return j
        return idx

    def is_empty(self) -> bool:
        return len(self.items) == 0

    def total_time(self, loop: bool) -> float:
        """Total schedule duration. With loop, includes the wrap-around transition."""
        if self.is_empty():
            return 0.0
        t = sum(m.duration for m in self.items)
        n = len(self.items)
        seams = n if (loop and n > 1) else max(0, n - 1)
        return t + seams * self.blend

    def sample(self, elapsed: float, loop: bool):
        """Return (cmd25, ghost20, motion_index, local_frame, in_transition).

        Segments laid out as: motion0, [blend], motion1, [blend], ..., and when
        looping a final [blend] back to motion0.
        """
        if self.is_empty():
            return None
        n = len(self.items)
        total = self.total_time(loop)

        if loop and total > 0:
            elapsed = elapsed % total
        else:
            elapsed = min(elapsed, total)

        t = elapsed
        for i, m in enumerate(self.items):
            # motion segment
            if t < m.duration or (i == n - 1 and not loop and t <= m.duration + 1e-6):
                frame = int(np.clip(t * m.fps, 0, m.num_frames - 1))
                return m.get_sparse_cmd(frame), m.get_ghost_state(frame), i, frame, False
            t -= m.duration
            # transition segment after motion i (exists if not-last, or last&loop)
            has_seam = (i < n - 1) or loop
            if has_seam:
                if t < self.blend:
                    a = self.items[i]
                    b = self.items[(i + 1) % n]
                    alpha = t / self.blend if self.blend > 1e-6 else 1.0
                    cmd = blend_sparse_cmd(a.get_sparse_cmd(a.num_frames - 1),
                                           b.get_sparse_cmd(0), alpha)
                    ghost = b.get_ghost_state(0)
                    return cmd, ghost, (i + 1) % n, 0, True
                t -= self.blend

        # past the end (non-loop): hold last motion's final frame
        last = self.items[-1]
        return (last.get_sparse_cmd(last.num_frames - 1),
                last.get_ghost_state(last.num_frames - 1), n - 1, last.num_frames - 1, False)


# ── ROS node ────────────────────────────────────────────────────────

class MotionNode(Node):
    def __init__(self):
        super().__init__("motion_gui")
        self.cmd_pub = self.create_publisher(Float64MultiArray, "/p73/motion_cmd", 10)
        self.ghost_pub = self.create_publisher(Float64MultiArray, "/p73/ghost_state", 10)

        # Subscribe to taskCommand for auto-start
        try:
            from p73_msgs.msg import TaskCmd
            self.task_sub = self.create_subscription(TaskCmd, "/p73/taskCommand", self._task_cb, 10)
            self._use_taskcmd = True
        except ImportError:
            from std_msgs.msg import UInt32
            self.task_sub = self.create_subscription(UInt32, "/p73/taskCommand", self._task_uint_cb, 10)
            self._use_taskcmd = False

        self.cc_mode_activated = False  # set by callback, read by GUI

        # NOTE: motion playback is paced by wall-clock (50Hz tick). A previous
        # version paced on /mujoco/full_state[-1] as if it were sim_time, but
        # full_state carries qpos only (last element = a joint angle), so the
        # ghost frame was driven by an oscillating joint -> violent vibration.
        # The mujoco viewer runs real-time, so wall-clock == sim time here.

    def _task_cb(self, msg):
        if 5 <= msg.task_mode < 10:
            self.cc_mode_activated = True

    def _task_uint_cb(self, msg):
        if 5 <= msg.data < 10:
            self.cc_mode_activated = True

    def publish_cmd(self, cmd: np.ndarray):
        try:
            msg = Float64MultiArray()
            msg.data = cmd.tolist()
            self.cmd_pub.publish(msg)
        except Exception:
            pass

    def publish_ghost(self, ghost: np.ndarray):
        try:
            msg = Float64MultiArray()
            msg.data = ghost.tolist()
            self.ghost_pub.publish(msg)
        except Exception:
            pass


# ── GUI ─────────────────────────────────────────────────────────────

class MotionGuiWindow(QWidget):
    MOTION_DATA_DIR = os.path.expanduser("~/ros2_ws/src/p73_cc/motion_data")

    def __init__(self, node: MotionNode):
        super().__init__()
        self.node = node
        self.playlist = Playlist()
        self.playing = False
        self.elapsed = 0.0          # global playlist time (s), wall-clock paced
        self.policy_dt = 0.02       # 50Hz, matches IsaacLab decimation
        self._slider_pressed = False
        self._cur_motion_idx = 0
        self._cur_frame = 0
        self._in_transition = False

        self.setWindowTitle("P73 Motion Control (vision, 25D)")
        self.setMinimumWidth(460)
        self._build_ui()
        self._load_file_list()

        self._shutting_down = False

        # 50Hz publish timer
        self._pub_timer = QTimer(self)
        self._pub_timer.timeout.connect(self._tick)
        self._pub_timer.start(20)

        # 10ms ROS spin timer
        self._ros_timer = QTimer(self)
        self._ros_timer.timeout.connect(self._ros_spin)
        self._ros_timer.start(10)

    def _ros_spin(self):
        if self._shutting_down:
            return
        try:
            rclpy.spin_once(self.node, timeout_sec=0)
        except Exception:
            self.shutdown()

    def shutdown(self):
        if self._shutting_down:
            return
        self._shutting_down = True
        self._pub_timer.stop()
        self._ros_timer.stop()
        QApplication.instance().quit()

    def _build_ui(self):
        layout = QVBoxLayout(self)

        # Motion browser + add
        file_row = QHBoxLayout()
        file_row.addWidget(QLabel("Motion:"))
        self.file_combo = QComboBox()
        file_row.addWidget(self.file_combo, 1)
        self.btn_add = QPushButton("Add →")
        self.btn_add.clicked.connect(self._on_add)
        file_row.addWidget(self.btn_add)
        layout.addLayout(file_row)

        # Playlist + side controls
        pl_row = QHBoxLayout()
        self.playlist_widget = QListWidget()
        self.playlist_widget.setMaximumHeight(130)
        pl_row.addWidget(self.playlist_widget, 1)
        side = QVBoxLayout()
        self.btn_up = QPushButton("↑")
        self.btn_down = QPushButton("↓")
        self.btn_remove = QPushButton("Remove")
        self.btn_clear = QPushButton("Clear")
        self.btn_up.clicked.connect(lambda: self._on_move(-1))
        self.btn_down.clicked.connect(lambda: self._on_move(+1))
        self.btn_remove.clicked.connect(self._on_remove)
        self.btn_clear.clicked.connect(self._on_clear)
        for b in (self.btn_up, self.btn_down, self.btn_remove, self.btn_clear):
            side.addWidget(b)
        side.addStretch(1)
        pl_row.addLayout(side)
        layout.addLayout(pl_row)

        # Transport controls
        btn_row = QHBoxLayout()
        self.btn_play = QPushButton("Play")
        self.btn_play.clicked.connect(self._on_play)
        self.btn_pause = QPushButton("Pause")
        self.btn_pause.clicked.connect(self._on_pause)
        self.btn_stop = QPushButton("Stop")
        self.btn_stop.clicked.connect(self._on_stop)
        self.chk_loop = QCheckBox("Loop")
        self.chk_loop.setChecked(True)
        btn_row.addWidget(self.btn_play)
        btn_row.addWidget(self.btn_pause)
        btn_row.addWidget(self.btn_stop)
        btn_row.addWidget(self.chk_loop)
        layout.addLayout(btn_row)

        # Blend control
        blend_row = QHBoxLayout()
        blend_row.addWidget(QLabel("Transition blend (s):"))
        self.spin_blend = QDoubleSpinBox()
        self.spin_blend.setRange(0.0, 2.0)
        self.spin_blend.setSingleStep(0.05)
        self.spin_blend.setValue(self.playlist.blend)
        self.spin_blend.valueChanged.connect(self._on_blend_changed)
        blend_row.addWidget(self.spin_blend)
        blend_row.addStretch(1)
        layout.addLayout(blend_row)

        # Timeline slider (global playlist time, in 50Hz steps)
        self.slider = QSlider(Qt.Horizontal)
        self.slider.setMinimum(0)
        self.slider.setMaximum(0)
        self.slider.sliderPressed.connect(self._slider_press)
        self.slider.sliderReleased.connect(self._slider_release)
        self.slider.valueChanged.connect(self._slider_changed)
        layout.addWidget(self.slider)

        # Status
        self.lbl_status = QLabel("Playlist empty")
        layout.addWidget(self.lbl_status)

        # Auto-start
        self.chk_auto = QCheckBox("Auto-start on CC mode (task_mode 5~9)")
        self.chk_auto.setChecked(True)
        layout.addWidget(self.chk_auto)

    # ── Playlist management ──

    def _load_file_list(self):
        self.file_combo.clear()
        pkls = sorted(glob.glob(os.path.join(self.MOTION_DATA_DIR, "*.pkl")))
        if not pkls:
            alt = os.path.expanduser(
                "~/isaaclab5.2/isaaclab_walker_motion/source/isaaclab_walker_motion/"
                "isaaclab_walker_motion/assets/data/p73_walker/motion_data"
            )
            pkls = sorted(glob.glob(os.path.join(alt, "*.pkl")))
        for p in pkls:
            self.file_combo.addItem(os.path.basename(p), p)
        # Seed the playlist with the first motion so there is something to play.
        if pkls:
            self._add_path(pkls[0])

    def _add_path(self, path: str):
        try:
            self.playlist.add(path)
        except Exception as e:
            self.node.get_logger().error(f"Failed to load {os.path.basename(path)}: {e}")
            return
        self._refresh_playlist_widget()
        self._refresh_slider_range()

    def _on_add(self):
        path = self.file_combo.currentData()
        if path:
            self._add_path(path)
            self.node.get_logger().info(f"Added: {os.path.basename(path)} (playlist size {len(self.playlist.items)})")

    def _on_remove(self):
        idx = self.playlist_widget.currentRow()
        if idx >= 0:
            self.playlist.remove(idx)
            self._refresh_playlist_widget()
            self._refresh_slider_range()

    def _on_clear(self):
        self.playlist.clear()
        self._on_stop()
        self._refresh_playlist_widget()
        self._refresh_slider_range()

    def _on_move(self, delta: int):
        idx = self.playlist_widget.currentRow()
        if idx >= 0:
            j = self.playlist.move(idx, delta)
            self._refresh_playlist_widget()
            self.playlist_widget.setCurrentRow(j)

    def _on_blend_changed(self, val: float):
        self.playlist.blend = float(val)
        self._refresh_slider_range()

    def _refresh_playlist_widget(self):
        self.playlist_widget.clear()
        for k, m in enumerate(self.playlist.items):
            self.playlist_widget.addItem(f"{k+1}. {m.name}  ({m.duration:.1f}s)")

    def _refresh_slider_range(self):
        total = self.playlist.total_time(self.chk_loop.isChecked())
        self.slider.blockSignals(True)
        self.slider.setMaximum(max(0, int(total / self.policy_dt)))
        self.slider.blockSignals(False)

    # ── Transport ──

    def _on_play(self):
        if self.playlist.is_empty():
            return
        self.playing = True

    def _on_pause(self):
        self.playing = False

    def _on_stop(self):
        self.playing = False
        self.elapsed = 0.0
        self.slider.blockSignals(True)
        self.slider.setValue(0)
        self.slider.blockSignals(False)

    def _slider_press(self):
        self._slider_pressed = True

    def _slider_release(self):
        self._slider_pressed = False
        self.elapsed = self.slider.value() * self.policy_dt

    def _slider_changed(self, val):
        if self._slider_pressed:
            self.elapsed = val * self.policy_dt

    # ── Main loop (50Hz) ──

    def _tick(self):
        if self._shutting_down or self.playlist.is_empty():
            if not self.playlist.is_empty():
                return
            self.lbl_status.setText("Playlist empty — add motions above")
            return

        loop = self.chk_loop.isChecked()

        # Auto-start on CC mode
        if self.chk_auto.isChecked() and self.node.cc_mode_activated:
            self.node.cc_mode_activated = False
            self.elapsed = 0.0
            self.playing = True
            self.node.get_logger().info(
                f"CC mode detected — auto-starting playlist "
                f"({len(self.playlist.items)} motions, wall-clock paced)")

        if self.playing:
            self.elapsed += self.policy_dt
            if not loop and self.elapsed >= self.playlist.total_time(loop):
                self.elapsed = self.playlist.total_time(loop)
                self.playing = False

        # Publish
        if self.playing or self._slider_pressed:
            sampled = self.playlist.sample(self.elapsed, loop)
            if sampled is not None:
                cmd, ghost, mi, fr, intr = sampled
                self._cur_motion_idx, self._cur_frame, self._in_transition = mi, fr, intr
                self.node.publish_cmd(cmd)
                self.node.publish_ghost(ghost)
        else:
            # Standing reference of the first motion when stopped/paused.
            first = self.playlist.items[0]
            self.node.publish_cmd(first.get_standing_cmd())
            sampled = self.playlist.sample(self.elapsed, loop)
            if sampled is not None:
                self.node.publish_ghost(sampled[1])

        # Update UI
        if not self._slider_pressed:
            self.slider.blockSignals(True)
            self.slider.setValue(int(self.elapsed / self.policy_dt))
            self.slider.blockSignals(False)
        self._update_label()

    def _update_label(self):
        if self.playlist.is_empty():
            self.lbl_status.setText("Playlist empty")
            return
        loop = self.chk_loop.isChecked()
        total = self.playlist.total_time(loop)
        state = "Playing" if self.playing else "Stopped"
        n = len(self.playlist.items)
        if self._in_transition:
            seg = f"→ blending into #{self._cur_motion_idx + 1}/{n}"
        else:
            m = self.playlist.items[self._cur_motion_idx]
            seg = f"#{self._cur_motion_idx + 1}/{n} {m.name}  f{self._cur_frame}/{m.num_frames}"
        self.lbl_status.setText(
            f"{state}  |  {seg}  |  {self.elapsed:.1f}s / {total:.1f}s")


# ── Main ────────────────────────────────────────────────────────────

def main():
    import signal

    rclpy.init()
    node = MotionNode()

    app = QApplication(sys.argv)
    win = MotionGuiWindow(node)
    win.show()

    # Let SIGINT/SIGTERM cleanly quit the Qt loop
    signal.signal(signal.SIGINT, lambda *_: win.shutdown())
    signal.signal(signal.SIGTERM, lambda *_: win.shutdown())
    # Ensure Python signal handlers run even when Qt blocks
    sig_timer = QTimer()
    sig_timer.timeout.connect(lambda: None)
    sig_timer.start(200)

    ret = app.exec_()

    try:
        node.destroy_node()
    except Exception:
        pass
    try:
        rclpy.shutdown()
    except Exception:
        pass
    sys.exit(ret)


if __name__ == "__main__":
    main()
