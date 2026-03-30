# Walker (ROS2) vs TOCABI (ROS1) 구현 상태 비교 분석

> **목적**: catkin_ws(TOCABI, yh-basic 브랜치)의 전체 파이프라인을 기준으로, Walker_ws(P73)에서 무엇이 구현되었고 무엇이 빠져있는지 하나하나 분석한다.
> **현재 상태**: MuJoCo 환경 + 로봇 모델 렌더링은 되지만, init yaw / state estimate / task mode 7 모두 동작하지 않음.

---

## 결론 요약 (TL;DR)

| 기능 | TOCABI (catkin_ws) | Walker (Walker_ws) | 상태 |
|---|---|---|---|
| MuJoCo 시뮬레이션 | mujoco_ros (ROS1, SHM) | mjc_ros2 (ROS2, SHM) | **구현됨** |
| 로봇 모델 (URDF/XML) | tocabi_obj_JL | p73_walker | **구현됨** |
| 메인 컨트롤러 | tocabi_controller (2kHz) | p73_controller (2kHz) | **구현됨** |
| State Estimator | state_manager.cpp | state_estimator.cpp | **구현됨** (LKF 포함) |
| ONNX Policy (cc) | tocabi_cc (33DOF, 1032줄) | p73_walker_cc (12DOF, 569줄) | **구현됨** |
| GUI | tocabi_gui (Qt) | p73_walker_gui (rqt plugin) | **구현됨** |
| Init Yaw | GUI → guiCommand → InitYaw() | GUI → guiCommand → InitYaw() | **코드 존재, 실행 안됨** |
| State Estimate | GUI → stateEstimate 토글 | GUI → stateEstimate 토글 | **코드 존재, 실행 안됨** |
| Task Mode 7 | GUI → taskCommand(7) → cc | GUI → taskCommand(5~9) → cc | **코드 존재, 실행 안됨** |
| Keyboard Teleop (MuJoCo 창) | Ctrl+T → /joy_gui | **미구현** | **누락** |
| ROS2 Teleop | 없음 | p73/cmd_vel 구독 (하드코딩 override) | **부분** |
| rqt_multiplot | 자동 launch | **미구현** | **누락** |

**핵심 문제**: 코드는 대부분 존재하지만, **실행 시 연결이 끊어진 부분**이 있어서 init yaw / state estimate / task mode가 동작하지 않는 것.

---

## 1. 패키지별 1:1 대응 관계

| TOCABI (catkin_ws) | Walker (Walker_ws) | 비고 |
|---|---|---|
| `dyros_tocabi_v2/tocabi_controller` | `p73_walker_controller/p73_controller` | 메인 컨트롤러 |
| `dyros_tocabi_v2/tocabi_lib` | `p73_walker_controller/p73_lib` | 로봇 라이브러리 |
| `dyros_tocabi_v2/tocabi_description` | `p73_walker_description` | URDF/XML/설정 |
| `dyros_tocabi_v2/tocabi_msgs` | `p73_walker_controller/p73_msgs` | 메시지 정의 |
| `tocabi_cc` | `p73_walker_cc` | ONNX 정책 컨트롤러 |
| `tocabi_gui` | `p73_walker_gui` | Qt GUI |
| `mujoco_ros_sim/mujoco_ros` | `mujoco_ros2_sim/mjc_ros2` | MuJoCo 인터페이스 |
| `dyros_tocabi_v2/tocabi_ecat` | `br_driver` (외부 패키지) | 하드웨어 인터페이스 |

---

## 2. 기능별 상세 비교

### 2.1 MuJoCo 시뮬레이션

| 항목 | TOCABI | Walker | 상태 |
|---|---|---|---|
| MuJoCo 버전 | 2.1.0 | 3.3.7 | **Walker가 최신** |
| 물리 시뮬레이션 | mj_step() 루프 | mj_step() 루프 | **동일** |
| Joint command 수신 | ROS topic → d->ctrl[] | SHM → d->ctrl[] | **동일** (방식 다름) |
| 센서 데이터 발행 | SHM (q, qdot, FT) | SHM + ROS2 topic | **동일** |
| Virtual state 발행 | SHM | `p73/mjcVirtualState` (Float64MultiArray) | **동일** |
| Keyframe 로딩 | 수동 | 자동 "stand" keyframe | **Walker 개선** |
| Keyboard teleop | **Ctrl+T → /joy_gui** | **미구현** | **누락** |
| 주기적 teleop 발행 | 20Hz 반복 발행 | N/A | **누락** |

**누락 상세 - Keyboard Teleop**:
TOCABI의 `mjros.cpp`에는 GLFW 키 이벤트 핸들러에서 Ctrl+T 토글 → q/w/e/a/s/d → `/joy_gui` 발행 로직이 있음.
Walker의 `mjc_ros2/main.cpp`에는 이 기능이 **전혀 없음**.
대신 Walker는 `p73/cmd_vel` ROS2 토픽으로 `teleop_twist_keyboard`를 사용할 수 있지만, cc.cpp에서 **하드코딩**으로 override되어 실제 사용 불가.

---

### 2.2 Launch 파일

| 항목 | TOCABI | Walker | 상태 |
|---|---|---|---|
| 시뮬 launch | `simulation.launch` (XML) | `simulation.launch.py` (Python) | **구현됨** |
| URDF 선택 | hand/urdf args | 고정 (p73_walker) | **구현됨** (단순화) |
| PD gain 로드 | YAML rosparam load | YAML → ROS2 parameter | **구현됨** |
| shm_reset | roslaunch에서 실행 | launch에서 실행 | **구현됨** |
| GUI 자동 launch | launch에서 tocabi_gui | launch에서 p73_gui | **구현됨** |
| rqt_multiplot | launch에서 자동 실행 | **미구현** | **누락** |
| joystick node | launch arg로 선택 | **미구현** | **누락** |

---

### 2.3 메인 컨트롤러 (Thread1 - TaskCtrlThread)

| 항목 | TOCABI | Walker | 상태 |
|---|---|---|---|
| 스레드 수 | 4 (State, Ctrl, Thread2, Thread3) | 5 (State, Ctrl, Slow, MPC, Logging) | **구현됨** |
| 제어 주파수 | 2kHz | ~2kHz (trigger 기반) | **구현됨** |
| Position Control mode | cubic interp + PD | cubic interp + PD | **구현됨** |
| Gravity Compensation | WBC::GravityCompensation | Kp/Kd hold (sim), zero (real) | **간소화됨** |
| Task Mode 분기 | mode 0-4 + 6,7,8(CC) | mode 0-4 + 5-9(CC) | **구현됨** |
| CC 호출 | `my_cc.computeSlow()` | `cc_.computeFast()` | **구현됨** |
| SendCommand | atomic copy → SHM | atomic copy → SHM | **구현됨** |
| Thread2 (Slow) | 필요시 사용 | 비어있음 | **동일** |
| Thread3 (MPC) | 필요시 사용 | 비어있음 | **동일** |
| COMPILE_P73_CC | COMPILE_TOCABI_CC | COMPILE_P73_CC (CMake) | **구현됨** |

---

### 2.4 State Estimator

| 항목 | TOCABI | Walker | 상태 |
|---|---|---|---|
| 라이브러리 | RBDL | Pinocchio | **Walker가 최신** |
| 루프 주파수 | 2kHz | 1kHz | **다름** |
| IMU 읽기 | SHM | SHM (br_driver) | **구현됨** |
| Joint 읽기 | SHM (EtherCAT) | SHM (br_driver) | **구현됨** |
| InitYaw() | 구현됨 | **구현됨** | **동일** |
| StateEstimate() | LKF | **LKF 구현됨** | **구현됨** |
| guiCommand 구독 | ROS1 subscriber | ROS2 subscriber (`p73/guiCommand`) | **구현됨** |
| "initYaw" 처리 | dc_.inityawSwitch = true | dc_.inityawSwitch = true | **구현됨** |
| "stateEstimate" 처리 | semode 토글 | se_mode 토글 | **구현됨** |
| Virtual state (MuJoCo) | SHM | `p73/mjcVirtualState` 구독 | **구현됨** |
| q_virtual_ 레이아웃 | (39D) [xyz, qxyzw, q33, qw] | (20D) [xyz, qxyzw, q13] | **다름 (DOF 차이)** |

---

### 2.5 ONNX Policy Controller (CC)

| 항목 | TOCABI (yh-basic) | Walker | 상태 |
|---|---|---|---|
| 파일 크기 | 1032줄 | 569줄 | Walker 간결 |
| 제어 DOF | 33 (전신) | 12 (다리) + WaistYaw PD hold | **구현됨** |
| Obs 차원 | 108D (single frame) | 47D (single frame) | **다름 (로봇 구조)** |
| History layout | term-major | term-major | **동일** |
| History 길이 | ONNX에서 추론 | ONNX에서 추론 | **동일** |
| Gait phase | 없음 (TOCABI) | sin/cos (2D) | **Walker 추가** |
| 관절 순서 변환 | kTocabiToIsaac / kIsaacToTocabi | kP73ToIsaac / kIsaacToP73 | **구현됨** |
| Action scale | 0.5 | 0.5 | **동일** |
| Action 처리 | scale → clip → delta_default | scale → clip → delta_default | **동일** |
| PD gain | Kp/9, Kd/3 | Kp, Kd (직접 사용, /9 /3 안함) | **다름 (로봇별)** |
| Torque clamp | torque_bound_ | torque_bound_p73_ | **구현됨** |
| 초기 스플라인 | 0.1초 cubic | 0.1초 cubic | **동일** |
| Velocity cmd | /joy_gui → joyCallback | p73/cmd_vel → velCmdCallback | **구현됨** |
| Velocity 사용 | 토픽에서 수신 → obs에 반영 | **하드코딩 [0.5, 0, 0]** | **미연결** |
| joint_vel 정규화 | clip(-30,30) → /30 | **없음 (raw)** | **누락 가능** |
| Noise injection | 미세 노이즈 + 수치미분 | **없음** | **누락 (심각도 낮음)** |
| JSONL 디버그 덤프 | 지원 | **미구현** | **누락 (편의)** |
| Latent 출력 | head_vel_est, foot_force_est | **미구현** | **누락 (편의)** |
| weight_dir rosparam | 지원 | **하드코딩 경로** | **누락** |
| enable_timing_gates | 지원 (policy_dt, pd_dt) | policy_dt만 사용 | **부분 구현** |

---

### 2.6 GUI

| 항목 | TOCABI | Walker | 상태 |
|---|---|---|---|
| 프레임워크 | Qt standalone | rqt plugin (Qt) | **구현됨** |
| Torque On/Off | 있음 | 있음 | **구현됨** |
| Safety Reset | 있음 | 있음 | **구현됨** |
| Init Yaw 버튼 | 있음 | 있음 | **구현됨** |
| State Estimate 버튼 | 있음 | 있음 | **구현됨** |
| Task Mode 선택 | 있음 (0-14) | 있음 (0-14) | **구현됨** |
| Position Control | 있음 | 있음 | **구현됨** |
| Gravity Comp | 있음 | 있음 | **구현됨** |
| Joint State 표시 | 있음 | 있음 | **구현됨** |
| Status Log 표시 | 있음 | 있음 | **구현됨** |
| Velocity slider | 있음 | **없음** | **누락** |
| 토픽 발행 | 토픽별 직접 발행 | `p73/guiCommand` (String) | **구현됨** |

---

## 3. "동작하지 않는 원인" 추적

### 3.1 Init Yaw가 안 되는 이유

**코드 경로 분석:**
```
GUI "Init Yaw" 클릭
  → window.cpp:25: guiCommandSend("initYaw")
  → p73/guiCommand 토픽 발행 (String "initYaw")
  → state_estimator.cpp:861: GuiCmdCallback → dc_.inityawSwitch = true
  → state_estimator.cpp:279: InitYaw() 호출 (StateEstimator 루프 내)
  → state_estimator.cpp:442: if (dc_.inityawSwitch) → yaw_init 저장
```

**가능한 실패 지점:**
1. **GuiCmdCallback이 호출되지 않음** - 구독자가 `p73/guiCommand`를 수신하지 못함
2. **StateEstimator 루프가 돌지 않음** - `robot.is_initialized()` 조건 미충족
3. **IMU 데이터가 없음** - MuJoCo에서 IMU 센서 데이터가 SHM으로 전달되지 않음
4. **q_virtual_local_ 미초기화** - InitYaw()가 호출되어도 quaternion이 0

**확인 사항:**
- `ros2 topic echo p73/guiCommand` 로 GUI 버튼 클릭 시 "initYaw" 메시지가 나오는지
- 콘솔에 `STATE : Yaw Initialized :` 메시지가 나오는지
- StateEstimator 스레드가 시작되었는지 (`THREAD0 : StateEstimator start` 로그)

---

### 3.2 State Estimate가 안 되는 이유

**코드 경로 분석:**
```
GUI "State Estimate" 클릭
  → guiCommandSend("stateEstimate")
  → state_estimator.cpp:863: dc_.se_mode = true, dc_.stateEstimateModeSwitch = true
  → state_estimator.cpp:469: if (dc_.se_mode) → LKF 초기화 + 업데이트
```

**가능한 실패 지점:**
1. **위 Init Yaw와 동일한 연결 문제**
2. **LKF 초기화 시 link_local_ 데이터 부재** - Pinocchio 모델 로드 실패
3. **imu_lin_acc 등 센서 데이터가 0** - MuJoCo에서 SHM 경유로 전달 안됨

---

### 3.3 Task Mode 7(5~9)가 안 되는 이유

**코드 경로 분석:**
```
GUI에서 Task Mode 선택 → "Send Task" 버튼 클릭
  → window.cpp:160-163: taskCommand 토픽 발행 (task_mode = currentIndex)
  → p73_controller.cpp:552-559: taskCmdCallback → dc_.tc_mode = true, dc_.task_cmd_ = msg
  → p73_controller.cpp:427: if (task_mode >= 5 && < 10) → cc_.computeFast()
```

**가능한 실패 지점:**
1. **COMPILE_P73_CC가 정의되지 않음** - p73_walker_cc 패키지가 빌드되지 않음
2. **ONNX 모델 로드 실패** - policy.onnx 경로 문제
3. **rd_cc_.q_virtual_가 비어있음** - StateEstimator가 정상 동작하지 않으면 관절 데이터 0
4. **cc_init_ 플래그가 리셋되지 않음** - 여러 번 진입 시 재초기화 안됨
5. **State Estimate가 선행되지 않음** - q_virtual_ 데이터가 올바르지 않으면 obs가 잘못됨

---

## 4. 구현 완료된 부분 (상세)

### 4.1 MuJoCo 시뮬레이션 (mjc_ros2) - 완료
- `mjc_ros2/main.cpp`: MuJoCo 3.3.7 사용, GLFW 렌더링
- `controlCallback()`: SHM에서 joint_command 읽어서 d->ctrl[] 적용
- 센서 데이터(IMU, joint state) SHM으로 발행
- `p73/mjcVirtualState` 토픽으로 base 위치/속도 발행
- "stand" keyframe 자동 로딩

### 4.2 P73 Controller (p73_controller) - 완료
- 5스레드 구조 (State, Ctrl, Slow, MPC, Logging)
- Position control mode: cubic interpolation + PD
- Task mode 0 (Joint Tune), 1 (Friction Comp), 3 (IK Float), 4 (IK Contact)
- Task mode 5-9: `cc_.computeFast()` 호출 (`COMPILE_P73_CC` 조건부)
- `SendCommand()`: atomic spinlock으로 토크 명령 전달

### 4.3 State Estimator - 완료
- Pinocchio 기반 forward kinematics/dynamics
- IMU 데이터 처리 (quaternion → RPY)
- `InitYaw()`: yaw 기준 보정
- `StateEstimate()`: Linear Kalman Filter (LKF) 기반 base 위치/속도 추정
- `GuiCmdCallback()`: initYaw, stateEstimate, torqOn/Off, safetyReset 처리
- Four-bar linkage Jacobian 계산 (실제 로봇용)

### 4.4 ONNX Policy Controller (p73_walker_cc) - 완료
- ONNX Runtime 로드 + 자동 shape 추론 (history_length)
- 47D observation: ang_vel(3) + gravity(3) + cmd(3) + gait_sin(1) + gait_cos(1) + joint_pos(12) + joint_vel(12) + last_action(12)
- Term-major history buffer
- P73 ↔ IsaacLab 관절 순서 변환
- Action: scale(0.5) → clip(-1,1) → delta_default → PD → torque clamp
- 50Hz policy 업데이트
- 0.1초 cubic spline 초기화
- `p73/cmd_vel` ROS2 subscriber (별도 스레드)

### 4.5 GUI (p73_walker_gui) - 완료
- rqt 플러그인으로 구현
- Init Yaw, State Estimate, Task Mode, Position Control, Gravity Comp 버튼
- Joint state, Elmo state, System state 실시간 표시
- `p73/guiCommand`, `p73/taskCommand`, `p73/posCommand`, `p73/ctrlMode`, `p73/ikTaskmode` 발행

---

## 5. 구현되지 않은 부분 (상세)

### 5.1 MuJoCo 창 Keyboard Teleop - **미구현** (중요도: 중)
TOCABI에서는 `mjros.cpp`에 Ctrl+T 토글 → q/w/e/a/s/d 키 → `/joy_gui` 발행 기능이 있음.
Walker의 `mjc_ros2/main.cpp`에는 이 기능이 **전혀 없음**.

**대안**: `ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -r cmd_vel:=p73/cmd_vel`를 별도 터미널에서 실행하면 되지만, 현재 cc.cpp에서 하드코딩으로 override됨.

**필요 작업**: cc.cpp의 velocity command 부분에서 하드코딩 제거, 또는 MuJoCo 창에 teleop 키 핸들러 추가.

### 5.2 Velocity Command 하드코딩 - **미연결** (중요도: 높)
`cc.cpp:282-284`:
```cpp
// velocity_commands (3) — hardcoded for testing
policy_frame_[idx++] = 0.5f;  // local_vel_x  ← 하드코딩!
policy_frame_[idx++] = 0.0f;  // local_vel_y
policy_frame_[idx++] = 0.0f;  // local_vel_yaw
```
`local_vel_x`, `local_vel_y`, `local_vel_yaw` 변수가 이미 준비되어 있지만 사용되지 않음.

**필요 작업**: 하드코딩 값을 `local_vel_x`, `local_vel_y`, `local_vel_yaw`로 교체.

### 5.3 weight_dir rosparam 미지원 - **하드코딩** (중요도: 중)
TOCABI: `nh_.getParam("/tocabi_cc/weight_dir", weight_dir_)` → rosparam으로 ONNX 경로 동적 변경.
Walker: `weight_dir_ = "$HOME/Walker_ws/src/p73_walker_cc/policy/policy.onnx"` → **하드코딩**.

**필요 작업**: ROS2 parameter로 weight_dir 지원 추가.

### 5.4 joint_vel 정규화 - **누락 가능** (중요도: 확인 필요)
TOCABI (yh-basic): `clip(-30, 30) → /30` 정규화 적용.
Walker: `q_vel_isaac` **raw 값 그대로 사용** (cc.cpp:297-298).

IsaacLab 학습 환경에서 joint_vel에 clip/scale을 적용했는지 확인 필요.
만약 학습 시 정규화를 사용했다면, Walker에서도 동일하게 적용해야 함.

**필요 작업**: IsaacLab Walker 학습 config 확인 후, 필요하면 cc.cpp에 정규화 추가.

### 5.5 rqt_multiplot 자동 실행 - **미구현** (중요도: 낮)
TOCABI: launch에서 rqt_multiplot 자동 실행 + XML 설정.
Walker: 없음.

### 5.6 Latent 출력 (est topics) - **미구현** (중요도: 낮)
TOCABI: `head_vel_est`, `foot_force_est_left/right` 토픽 발행 (디버깅/비교용).
Walker: 없음.

### 5.7 JSONL 디버그 덤프 - **미구현** (중요도: 낮)
TOCABI: `dump_io_json`, `dump_full_io` rosparam으로 policy IO 덤프.
Walker: 없음.

### 5.8 cc_init_ 리셋 문제 - **잠재적 버그** (중요도: 높)
`cc.cpp:434`: `cc_init_`가 false로 설정된 후, 다른 모드로 갔다가 다시 mode 5-9로 돌아오면 **재초기화되지 않음**.
TOCABI: `rd_cc_.tc_init`을 사용하며, 모드 진입 시마다 `rd_.tc_init = false`로 리셋.

**필요 작업**: task mode가 바뀔 때마다 `cc_init_ = true`로 리셋하는 로직 필요.
→ `taskCmdCallback()`에서 `cc_init_ = true` 설정 추가.

---

## 6. 데이터 구조 비교

### 6.1 q_virtual_ 레이아웃

| | TOCABI (39D) | Walker (20D) |
|---|---|---|
| [0:3] | pos (x, y, z) | pos (x, y, z) |
| [3:6] | quat (x, y, z) | quat (x, y, z, w) |
| [6] | qw (또는 joint 시작) | joint 시작 (7) |
| [7:39/20] | joints (33DOF) | joints (13DOF) |

**Walker 특이점**: quaternion이 `(x, y, z, w)` 순서로 인덱스 3,4,5,6에 저장.
```cpp
// cc.cpp:224-227
q.x() = rd_cc_.q_virtual_(3);
q.y() = rd_cc_.q_virtual_(4);
q.z() = rd_cc_.q_virtual_(5);
q.w() = rd_cc_.q_virtual_(6);  // w는 인덱스 6
```

### 6.2 q_dot_virtual_ 레이아웃

| | TOCABI (39D) | Walker (19D) |
|---|---|---|
| [0:3] | lin_vel (vx, vy, vz) | lin_vel (vx, vy, vz) |
| [3:6] | ang_vel (wx, wy, wz) | ang_vel (wx, wy, wz) |
| [6:] | joint_vel (33DOF) | joint_vel (13DOF) |

### 6.3 관절 순서

**P73 순서 (13DOF)**:
```
L_HipYaw(0), L_HipRoll(1), L_HipPitch(2), L_Knee(3), L_AnklePitch(4), L_AnkleRoll(5),
R_HipYaw(6), R_HipRoll(7), R_HipPitch(8), R_Knee(9), R_AnklePitch(10), R_AnkleRoll(11),
WaistYaw(12)
```

**IsaacLab 순서 (12DOF, 하체만)**:
```
L_HipRoll(0), L_HipPitch(1), L_HipYaw(2), L_Knee(3), L_AnklePitch(4), L_AnkleRoll(5),
R_HipRoll(6), R_HipPitch(7), R_HipYaw(8), R_Knee(9), R_AnklePitch(10), R_AnkleRoll(11)
```

**Permutation**: P73의 HipYaw(0,6)이 IsaacLab에서 HipYaw(2,8)로 이동.

---

## 7. ROS2 토픽/파라미터 정리

### 7.1 Walker 토픽

| Topic | Type | 발행자 | 구독자 | 설명 |
|---|---|---|---|---|
| `p73/guiCommand` | String | GUI | StateEstimator | initYaw, stateEstimate, torqOn/Off |
| `p73/ctrlMode` | UInt32 | GUI | P73Controller | 1=posCtrl, 2=gravComp |
| `p73/taskCommand` | TaskCmd | GUI | P73Controller | task_mode 설정 |
| `p73/posCommand` | PosCmd | GUI | P73Controller | 관절 위치 명령 |
| `p73/ikTaskmode` | IKTaskCmd | GUI | P73Controller | IK 타겟 |
| `p73/cmd_vel` | Twist | teleop_twist_keyboard | p73_walker_cc | 속도 명령 |
| `p73/mjcVirtualState` | Float64MultiArray | mjc_ros2 | StateEstimator | MuJoCo base 위치/속도 |
| `p73/ctrlTime` | Float32 | StateEstimator | GUI | 제어 시간 |
| `p73/jointState` | JointState | StateEstimator | GUI | 관절 상태 |
| `p73/statusLog` | String | StateEstimator | GUI | 상태 로그 |
| `p73/pelvState` | Float64MultiArray | StateEstimator | - | 골반 위치/속도 |
| `p73/imuState` | Float64MultiArray | StateEstimator | - | IMU 상태 |
| `p73/imuPose` | Pose | StateEstimator | - | IMU 자세 |

### 7.2 TOCABI에는 있고 Walker에 없는 토픽

| 누락 토픽 | 용도 | 중요도 |
|---|---|---|
| `/joy_gui` | MuJoCo 키보드 teleop → cc | 중 |
| `/tocabi/velcommand_twist` | 속도 명령 플롯 | 낮 |
| `/tocabi/head_vel_est` | latent 추정 머리 속도 | 낮 |
| `/tocabi/foot_force_est_*` | latent 추정 발 힘 | 낮 |
| `/foot_contact_wrench_*` | GT 접촉력 | 중 |

---

## 8. 빌드 및 실행 차이

| 항목 | TOCABI | Walker |
|---|---|---|
| 빌드 시스템 | catkin_make | colcon build |
| ROS 버전 | Noetic (ROS1) | Jazzy (ROS2) |
| 환경 | distrobox ros_noetic + conda | 네이티브 ROS2 |
| 공유 메모리 | SHM (mmap) | SHM (br_driver) |
| 파라미터 | rosparam (전역) | ROS2 parameter (노드별) |
| IPC | ROS1 topic + SHM | ROS2 topic + SHM |
| 실시간 스케줄링 | Xenomai (실물) | SCHED_FIFO (실물) |

---

## 9. 해야 할 작업 우선순위

### P0 - 당장 해결 (동작에 필수)
1. **init yaw / state estimate / task mode가 동작하지 않는 근본 원인 파악**
   - StateEstimator 스레드가 정상 시작되는지 확인
   - `p73/guiCommand` 토픽이 정상 발행/수신되는지 확인
   - MuJoCo → SHM → StateEstimator 데이터 흐름 확인
   - `robot.is_initialized()` 조건 확인

2. **cc_init_ 리셋 로직 추가** - 모드 전환 시 재초기화 보장

### P1 - 바로 이어서 (기능 완성)
3. **Velocity command 하드코딩 제거** - `local_vel_*` 변수 사용으로 변경
4. **weight_dir ROS2 parameter 지원** - ONNX 경로 동적 변경
5. **joint_vel 정규화 확인** - IsaacLab 학습 config 확인 후 필요시 추가

### P2 - 편의 기능
6. **MuJoCo 창 keyboard teleop** - mjc_ros2에 Ctrl+T 핸들러 추가
7. **rqt_multiplot 또는 PlotJuggler** - 실시간 플롯 설정
8. **디버그 덤프 (JSONL)** - policy IO 비교용

---

> **작성일**: 2026-03-30
> **분석 대상**: `/home/piene/Walker_ws` (P73 Walker, ROS2) vs `/home/piene/catkin_ws` (TOCABI, ROS1, yh-basic)
