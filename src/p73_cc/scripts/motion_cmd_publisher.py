#!/usr/bin/env python3
"""Publish SPARSE student command from pkl motion data to /p73/motion_cmd (walker_vision).

The walker_vision student is conditioned on a 25D low-dim deployable command
(base root + L/R foot pos/ori + waist yaw ref) — the interface a high-level
vision/diffusion policy would emit. This mirrors Isaac Lab's
`WalkerMimicEnv.calc_sparse_command_proprio`. For deployment we play a reference
motion pkl and emit the same 25D command the student saw in training.

Reads a motion pkl and publishes the 25D command at 50Hz. Waits for CC mode
activation (task_mode 5~9) before starting playback. Also publishes
/p73/ghost_state (20D absolute pose) for MuJoCo ghost visualization.

Usage:
    python3 motion_cmd_publisher.py [--pkl PATH] [--loop] [--start FRAME]

Sparse command layout (25D, yaw-local; anchor=base_link, feet=[L_Foot, R_Foot]):
    [0:2]   root_vel_local_xy   (yaw-aligned body frame, m/s)
    [2]     root_pos_z          (height, m)
    [3]     roll                (rad)
    [4]     pitch               (rad)
    [5]     root_ang_vel_z      (yaw rate, rad/s)
    [6:12]  feet_pos_local      (L xyz, R xyz; yaw-local, base_link-anchor-relative)
    [12:24] feet_ori_rot6d      (L Rot6D, R Rot6D; yaw-local)
    [24]    waist_yaw_ref       (rad, WaistYaw = last of ALL_JOINT_NAMES)
"""

import argparse
import pickle

import numpy as np
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray

# Bodies referenced by the sparse command (must match mimic_env.py tracked cfg).
ANCHOR_BODY = "base_link"
FOOT_BODIES = ["L_Foot_Link", "R_Foot_Link"]
NUM_SPARSE_CMD = 25


def quat_rotate_inverse(q_wxyz, v):
    """Rotate vector by inverse of quaternion (wxyz convention)."""
    w, x, y, z = q_wxyz
    t = 2.0 * np.cross(np.array([-x, -y, -z]), v)
    return v + w * t + np.cross(np.array([-x, -y, -z]), t)


def extract_yaw_quat(q_wxyz):
    """Extract yaw-only quaternion from full quaternion (wxyz)."""
    w, x, y, z = q_wxyz
    yaw = np.arctan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z))
    return np.array([np.cos(yaw / 2), 0.0, 0.0, np.sin(yaw / 2)])


def quat_conjugate(q_wxyz):
    w, x, y, z = q_wxyz
    return np.array([w, -x, -y, -z])


def quat_mul(a_wxyz, b_wxyz):
    """Hamilton product a * b (wxyz)."""
    aw, ax, ay, az = a_wxyz
    bw, bx, by, bz = b_wxyz
    return np.array([
        aw * bw - ax * bx - ay * by - az * bz,
        aw * bx + ax * bw + ay * bz - az * by,
        aw * by - ax * bz + ay * bw + az * bx,
        aw * bz + ax * by - ay * bx + az * bw,
    ])


def quat_to_rot6d(q_wxyz):
    """(4,) wxyz -> (6,) Rot6D (first two rotation-matrix columns). Matches env._quat_to_rot6d."""
    w, x, y, z = q_wxyz
    col0 = np.array([1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y)])
    col1 = np.array([2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x)])
    return np.concatenate([col0, col1])


def euler_from_quat(q_wxyz):
    """Euler angles (roll, pitch, yaw) from quaternion (wxyz)."""
    w, x, y, z = q_wxyz
    sinr_cosp = 2.0 * (w * x + y * z)
    cosr_cosp = 1.0 - 2.0 * (x * x + y * y)
    roll = np.arctan2(sinr_cosp, cosr_cosp)
    sinp = 2.0 * (w * y - z * x)
    sinp = np.clip(sinp, -1.0, 1.0)
    pitch = np.arcsin(sinp)
    siny_cosp = 2.0 * (w * z + x * y)
    cosy_cosp = 1.0 - 2.0 * (y * y + z * z)
    yaw = np.arctan2(siny_cosp, cosy_cosp)
    return roll, pitch, yaw


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

    # feet pos (yaw-local, anchor-relative): [6:9] L xyz, [9:12] R xyz
    for k in range(2):
        cmd[6 + 3 * k: 9 + 3 * k] = quat_rotate_inverse(yaw_quat, feet_pos_w[k] - anchor_pos_w)
    # feet ori (yaw-local Rot6D): [12:18] L, [18:24] R
    for k in range(2):
        q_local = quat_mul(yaw_inv, feet_quat_w[k])
        cmd[12 + 6 * k: 18 + 6 * k] = quat_to_rot6d(q_local)
    # waist yaw ref
    cmd[24] = waist_yaw
    return cmd


class MotionCmdPublisher(Node):
    def __init__(self, pkl_path, loop=True, start_frame=0):
        super().__init__("motion_cmd_publisher")

        # Load motion data
        with open(pkl_path, "rb") as f:
            data = pickle.load(f)

        self.fps = int(data["fps"])
        self.root_pos = np.array(data["root_pos"], dtype=np.float64)
        root_rot_raw = np.array(data["root_rot"], dtype=np.float64)
        # xyzw → wxyz
        self.root_rot = np.zeros_like(root_rot_raw)
        self.root_rot[:, 0] = root_rot_raw[:, 3]  # w
        self.root_rot[:, 1] = root_rot_raw[:, 0]  # x
        self.root_rot[:, 2] = root_rot_raw[:, 1]  # y
        self.root_rot[:, 3] = root_rot_raw[:, 2]  # z
        self.dof_pos = np.array(data["dof_pos"], dtype=np.float64)
        self.num_frames = len(self.root_pos)
        self.loop = loop

        # Body-level data for the sparse (feet) command.
        body_pos_w = np.array(data["body_pos_w"], dtype=np.float64)     # (T, B, 3)
        body_quat_w = np.array(data["body_quat_w"], dtype=np.float64)   # (T, B, 4) wxyz
        conv = str(data.get("body_quat_convention", "wxyz"))
        if conv != "wxyz":
            raise ValueError(f"Expected body_quat_convention 'wxyz', got {conv!r}")
        body_list = [str(b) for b in data["body_link_list"]]
        self.anchor_idx = body_list.index(ANCHOR_BODY)
        self.feet_idx = [body_list.index(fn) for fn in FOOT_BODIES]
        self.body_pos_w = body_pos_w
        self.body_quat_w = body_quat_w

        # Compute root velocities via finite differences (at motion fps) — same
        # convention as the validated motion deployment ([0:6] is shared with the
        # 19D motion_cmd).
        dt_motion = 1.0 / self.fps
        self.root_vel = np.zeros_like(self.root_pos)
        self.root_vel[1:] = (self.root_pos[1:] - self.root_pos[:-1]) / dt_motion
        self.root_vel[0] = self.root_vel[1]

        self.root_ang_vel = np.zeros((self.num_frames, 3))
        for i in range(1, self.num_frames):
            r0, p0, y0 = euler_from_quat(self.root_rot[i - 1])
            r1, p1, y1 = euler_from_quat(self.root_rot[i])
            self.root_ang_vel[i] = np.array([r1 - r0, p1 - p0, y1 - y0]) / dt_motion
        self.root_ang_vel[0] = self.root_ang_vel[1]

        # Standing reference (published before CC mode starts): frame 0 pose with
        # zero root velocity so it is in-distribution for the student.
        self.standing_cmd = compute_sparse_cmd(
            self.root_pos[0], self.root_rot[0],
            np.zeros(3), np.zeros(3),
            self.body_pos_w[0, self.anchor_idx],
            self.body_pos_w[0, self.feet_idx],
            self.body_quat_w[0, self.feet_idx],
            self.dof_pos[0, -1],
        )

        # Publishers
        self.pub = self.create_publisher(Float64MultiArray, "/p73/motion_cmd", 10)
        self.ghost_pub = self.create_publisher(Float64MultiArray, "/p73/ghost_state", 10)

        # Subscribe to taskCommand for CC mode trigger
        try:
            from p73_msgs.msg import TaskCmd
            self.task_sub = self.create_subscription(TaskCmd, "/p73/taskCommand", self._task_cb, 10)
            self.get_logger().info("Subscribed to /p73/taskCommand (p73_msgs/TaskCmd)")
        except ImportError:
            # Fallback: UInt32 on same topic
            from std_msgs.msg import UInt32
            self.task_sub = self.create_subscription(UInt32, "/p73/taskCommand", self._task_uint_cb, 10)
            self.get_logger().warn("p73_msgs not found, using UInt32 fallback for /p73/taskCommand")

        # Timer at 50Hz (policy rate)
        self.policy_dt = 0.02
        self.timer = self.create_timer(self.policy_dt, self.timer_callback)

        # State: wait for CC mode before playing motion
        self.cc_active = False
        self.time_acc = 0.0

        self.get_logger().info(
            f"Sparse cmd publisher (25D): {pkl_path} ({self.num_frames} frames @ {self.fps}fps, "
            f"duration={self.num_frames/self.fps:.1f}s, loop={loop})"
        )
        self.get_logger().info(
            f"anchor='{ANCHOR_BODY}'(idx {self.anchor_idx}), feet={FOOT_BODIES}(idx {self.feet_idx})"
        )
        self.get_logger().info("Waiting for CC mode (task_mode 5~9) to start motion playback...")

    def _task_cb(self, msg):
        """Handle p73_msgs/TaskCmd."""
        mode = msg.task_mode
        if 5 <= mode < 10 and not self.cc_active:
            self.cc_active = True
            self.time_acc = 0.0
            self.get_logger().info(f"CC mode {mode} activated — motion playback starting from frame 0")
        elif mode < 5 or mode >= 10:
            if self.cc_active:
                self.cc_active = False
                self.get_logger().info(f"CC mode deactivated (task_mode={mode})")

    def _task_uint_cb(self, msg):
        """Fallback for UInt32 topic."""
        mode = msg.data
        if 5 <= mode < 10 and not self.cc_active:
            self.cc_active = True
            self.time_acc = 0.0
            self.get_logger().info(f"CC mode {mode} activated — motion playback starting from frame 0")
        elif mode < 5 or mode >= 10:
            if self.cc_active:
                self.cc_active = False
                self.get_logger().info(f"CC mode deactivated (task_mode={mode})")

    def timer_callback(self):
        if not self.cc_active:
            # Before CC mode: publish standing reference, no ghost
            msg = Float64MultiArray()
            msg.data = self.standing_cmd.tolist()
            self.pub.publish(msg)
            return

        # Advance time and compute frame
        self.time_acc += self.policy_dt
        frame_float = self.time_acc * self.fps
        frame = int(frame_float) % self.num_frames if self.loop else min(int(frame_float), self.num_frames - 1)

        if not self.loop and int(frame_float) >= self.num_frames:
            frame = self.num_frames - 1

        # Publish sparse command (25D)
        cmd = compute_sparse_cmd(
            self.root_pos[frame],
            self.root_rot[frame],
            self.root_vel[frame],
            self.root_ang_vel[frame],
            self.body_pos_w[frame, self.anchor_idx],
            self.body_pos_w[frame, self.feet_idx],
            self.body_quat_w[frame, self.feet_idx],
            self.dof_pos[frame, -1],
        )
        msg = Float64MultiArray()
        msg.data = cmd.tolist()
        self.pub.publish(msg)

        # Publish ghost_state (20D absolute pose for MuJoCo ghost)
        ghost = np.zeros(20)
        ghost[0:3] = self.root_pos[frame]       # absolute position
        ghost[3:7] = self.root_rot[frame]        # quaternion (wxyz)
        ghost[7:20] = self.dof_pos[frame, :13]   # joint positions
        ghost_msg = Float64MultiArray()
        ghost_msg.data = ghost.tolist()
        self.ghost_pub.publish(ghost_msg)


def main():
    parser = argparse.ArgumentParser(description="Publish sparse student command (25D)")
    import os
    _default_pkl = os.path.join(os.path.dirname(__file__), "..", "motion_data", "p73_walk1_subject5_full.pkl")
    if not os.path.isfile(_default_pkl):
        _default_pkl = "/home/piene/isaaclab5.2/isaaclab_walker_motion/source/isaaclab_walker_motion/isaaclab_walker_motion/assets/data/p73_walker/motion_data/p73_walk1_subject5_full.pkl"
    parser.add_argument(
        "--pkl",
        type=str,
        default=_default_pkl,
        help="Path to motion pkl file",
    )
    parser.add_argument("--loop", action="store_true", default=True, help="Loop motion")
    parser.add_argument("--no-loop", dest="loop", action="store_false")
    parser.add_argument("--start", type=int, default=0, help="Start frame")
    args = parser.parse_args()

    rclpy.init()
    node = MotionCmdPublisher(args.pkl, loop=args.loop, start_frame=args.start)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
