# Walker_ws 프로젝트 개요

## 목표
IsaacLab에서 학습한 RL policy (ONNX)를 MuJoCo 시뮬레이션에서 실행하고,
최종적으로 실제 Walker(P73) 로봇에 sim-to-real 전이하는 ROS2 기반 시스템 구축.

## 참조 시스템 (성공 사례)
- **catkin_ws (tocabi)**: ROS1 Noetic 기반, conda `tocabirl` 환경
  - MuJoCo에서 ONNX policy 실행 성공
  - Sim-to-Real 전이 성공
  - 핵심 패키지: `tocabi_cc` (ONNX 추론), `mujoco_ros_sim`, `dyros_tocabi_v2`

## 현재 워크스페이스 구성

```
Walker_ws/
├── src/
│   ├── p73_walker_controller/          # 메인 제어 시스템
│   │   ├── p73_msgs/                   # ROS2 커스텀 메시지
│   │   ├── p73_lib/                    # 역학/제어 라이브러리
│   │   ├── p73_controller/             # 메인 컨트롤러 노드
│   │   └── p73_walker_description/     # 로봇 모델 (URDF, MuJoCo XML, meshes)
│   ├── p73_walker_gui/                 # RQt 기반 GUI
│   └── mujoco_ros2_sim/                # MuJoCo-ROS2 브리지
│       ├── mjc_ros2/                   # MuJoCo 시뮬레이터 노드
│       └── mujoco-3.3.7/              # MuJoCo 엔진 (v3.3.7)
└── md_files/                           # 문서
```

## 관련 외부 리포지토리

| 경로 | 역할 | 필요 여부 |
|------|------|-----------|
| `/home/piene/catkin_ws` | tocabi 참조 코드 (ROS1) | 참조용 (직접 사용 X) |
| `/home/piene/Github_Code/p73_sys_ws-p73_lower` | 하드웨어 제어 계층 (br_driver) | **필요** - 실제 로봇 및 시뮬레이션 빌드 의존성 |

## 시스템 아키텍처 (목표)

```
┌──────────────────────────────────────────────────────┐
│                IsaacLab (학습 환경)                    │
│         Policy 학습 → ONNX 파일 export                │
└──────────────┬───────────────────────────────────────┘
               │ policy.onnx
               ▼
┌──────────────────────────────────────────────────────┐
│              Walker_ws (ROS2)                         │
│                                                      │
│  ┌─────────────┐  ┌──────────────┐  ┌────────────┐  │
│  │ MuJoCo Sim  │◄─│  Controller  │──│   GUI      │  │
│  │ (mjc_ros2)  │  │(p73_control) │  │(p73_gui)   │  │
│  └─────────────┘  └──────┬───────┘  └────────────┘  │
│                          │                            │
│                   ┌──────┴───────┐                    │
│                   │  ONNX Policy │ ← 아직 미구현      │
│                   │  (tocabi_cc  │                    │
│                   │   equivalent)│                    │
│                   └──────────────┘                    │
│                          │                            │
│                   ┌──────┴───────┐                    │
│                   │  br_driver   │ ← p73_sys_ws      │
│                   │ (하드웨어)    │                    │
│                   └──────────────┘                    │
└──────────────────────────────────────────────────────┘
```

## 핵심 차이점: tocabi (ROS1) vs Walker (ROS2)

| 항목 | tocabi (catkin_ws) | Walker (Walker_ws) |
|------|-------------------|-------------------|
| ROS 버전 | ROS1 Noetic | ROS2 |
| 역학 라이브러리 | RBDL | Pinocchio |
| DOF | 33 (전신) | 13 (하체 + 허리) |
| QP 솔버 | qpOASES | OSQP + OsqpEigen |
| MuJoCo 버전 | 2.1+ | 3.3.7 |
| 하드웨어 인터페이스 | tocabi_ecat (SOEM) | br_driver (p73_sys_ws) |
| ONNX 추론 | tocabi_cc (구현 완료) | **미구현** |
| Python 환경 | conda tocabirl | 미설정 |
