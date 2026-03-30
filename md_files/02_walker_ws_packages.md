# Walker_ws 패키지 상세 분석

## 1. p73_msgs (ROS2 메시지 정의)

**위치**: `src/p73_walker_controller/p73_msgs/`

| 메시지 | 필드 | 용도 |
|--------|------|------|
| `TaskCmd.msg` | `uint32 taskcommand` | 제어 모드 전환 명령 |
| `PosCmd.msg` | `float64[32] position`, `float64 traj_time`, `bool gravity` | 관절 위치 명령 |
| `IKTaskCmd.msg` | `bool IK_mode`, `string target_link`, `float64[3] pos`, `float64 traj_time` | 역기구학 명령 |

---

## 2. p73_lib (핵심 제어 라이브러리)

**위치**: `src/p73_walker_controller/p73_lib/`

### 의존성
- Pinocchio (역기구학/역동역학)
- OSQP + OsqpEigen (QP 솔버)
- Eigen3
- br_driver (p73_sys_ws에서 제공)

### 주요 파일

| 파일 | 역할 |
|------|------|
| `include/p73_lib/p73.h` | P73 로봇 상수 정의 (13 DOF, 관절명, 링크명) |
| `include/p73_lib/robot_data.h` | RobotEigenData 구조체 (관절 상태, 동역학 행렬) |
| `include/p73_lib/link.h` | LinkData 클래스 (운동학/동역학 데이터) |
| `include/wholebody_functions.h` | WBC 함수들 (중력보상, 접촉역학, 역기구학) |
| `include/qp.h` | QP 솔버 래퍼 |
| `include/p73_lib/4bar_jac_func.h` | 4절 링크 기구학/야코비안 |

### 로봇 DOF 구성 (13 자유도)

```
Left Leg (6):  HipYaw, HipRoll, HipPitch, Knee, AnklePitch, AnkleRoll
Right Leg (6): HipYaw, HipRoll, HipPitch, Knee, AnklePitch, AnkleRoll
Waist (1):     WaistYaw
```

### WBC 기능
- `ContactCalc()`: 접촉 역학 계산
- `GravityCompensationTorque()`: 중력 보상 토크
- `FrictionCompensationTorque()`: 마찰 보상
- `NullSpaceIK()`: 널공간 역기구학
- `TorqueMapping()`: 관절→모터 토크 매핑
- `ContactForceRedistribution()`: 접촉력 재분배 (마찰 원뿔 제약)

---

## 3. p73_controller (메인 컨트롤러 노드)

**위치**: `src/p73_walker_controller/p73_controller/`

### 의존성
- rclcpp, p73_lib, p73_msgs, br_driver, sensor_msgs

### 실행 구조 (5개 실시간 스레드)

| 스레드 | 우선순위 | CPU 코어 | 역할 |
|--------|---------|----------|------|
| State Estimator | 65 (FIFO) | Core 5 | IMU 융합, 기구학 업데이트 |
| Task Control | 62 (FIFO) | - | 주 제어 루프 |
| Slow Computation | 62 (FIFO) | - | 저속 연산 (WBC 등) |
| MPC/Trajectory | 62 (FIFO) | - | 궤적 계획 |
| Logging | 62 (FIFO) | - | 데이터 로깅 |

### 주요 파일

| 파일 | 역할 |
|------|------|
| `src/main.cpp` | 진입점, 스레드 생성 및 실시간 스케줄링 설정 |
| `src/p73_controller.cpp` | 메인 제어 로직 (태스크 모드별 분기) |
| `src/state_estimator.cpp` | 상태 추정 (IMU 융합, 순기구학, 동역학) |
| `src/shm_reset.cpp` | 공유 메모리 초기화 유틸리티 |

### Launch 파일

```bash
# 시뮬레이션 모드
ros2 launch p73_controller simulation.launch.py

# 실제 로봇 모드
ros2 launch p73_controller realrobot.launch.py
```

**simulation.launch.py**: MuJoCo 시뮬레이터 + 컨트롤러 + GUI 동시 실행

---

## 4. p73_walker_description (로봇 모델)

**위치**: `src/p73_walker_controller/p73_walker_description/`

### URDF 구조
- **Root**: `base_link` (골반)
- **왼쪽 다리**: L_HipRoll → L_HipPitch → L_Thigh → L_Knee → L_AnkleRoll → L_Foot
- **오른쪽 다리**: R_HipRoll → R_HipPitch → R_Thigh → R_Knee → R_AnkleRoll → R_Foot
- **허리**: WaistYaw_Link
- **메시 파일**: 14개 STL (base, waist, hip, thigh, knee, ankle, foot 좌/우)

### MuJoCo 모델 (`mujoco/p73_walker.xml`)

```
Timestep:   0.001s
Solver:     Newton (dense Jacobian)
Contact:    Elliptic cone, noslip=30
Assets:     14 STL mesh files
Actuators:  13 motors (force-limited)
```

**모터 클래스**:
| 클래스 | Damping | Armature | 기어비 |
|--------|---------|----------|--------|
| hip_roll_motor | 2.5 | 0.96 | 80:1 |
| hip_pitch_motor | 2.5 | 0.375 | 50:1 |
| knee_pitch_motor | 2.0 | 0.375 | - |
| ankle motors | 1.0 | 0.0625 | - |
| waist_yaw_motor | 2.5 | 0.16 | - |

### 설정 파일 (YAML)

| 파일 | 내용 |
|------|------|
| `setting/setting_sim_PDgain.yaml` | 시뮬레이션 PD 게인, 마찰 파라미터, 토크 한계 |
| `setting/setting_realrobot_PDgain.yaml` | 실제 로봇 PD 게인 |

**PD 게인 예시 (시뮬레이션)**:
- Hip Roll/Pitch: Kp=4500, Kd=70
- Knee: Kp=4500, Kd=70
- Ankle: Kp=3000, Kd=50
- Waist: Kp=8500, Kd=80

### USD 파일
- `usd/p73_walker.usd`: IsaacSim/IsaacLab 호환 모델

---

## 5. p73_walker_gui (GUI)

**위치**: `src/p73_walker_gui/`

- RQt 기반 플러그인
- Qt5 UI (`resource/p73.ui`)
- p73_msgs 토픽을 통해 컨트롤러와 통신
- 태스크 명령 전송, 로봇 상태 모니터링

---

## 6. mujoco_ros2_sim (MuJoCo-ROS2 브리지)

**위치**: `src/mujoco_ros2_sim/`

### mjc_ros2 패키지
- MuJoCo 3.3.7 기반 시뮬레이터 노드
- ROS2 인터페이스 (관절 상태 발행, 토크 명령 수신)
- GLFW/OpenGL 기반 시각화
- br_driver 의존성 (공유 메모리 통신)

### MuJoCo 라이브러리
- `mujoco-3.3.7/`: 전체 MuJoCo 엔진 포함
  - `include/mujoco/`: 헤더
  - `lib/libmujoco.so.3.3.7`: 라이브러리
  - `simulate/`: 시뮬레이션 UI 유틸리티
