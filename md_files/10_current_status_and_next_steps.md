# Walker_ws 현재 상태 및 다음 단계 (2026-03-30)

## 해결된 문제

| 문제 | 원인 | 해결 |
|---|---|---|
| RL 토크 미적용 | `control_time_us_` 미설정 | state_estimator.cpp에 추가 |
| mode 재진입 실패 | `cc_init_` 미리셋 | taskCmdCallback에서 리셋 |
| 관절 순서 불일치 (3곳 다 다름) | p73.h가 MuJoCo/IsaacLab과 다름 | p73.h를 MuJoCo 순서로 변경 + Pinocchio 매핑 추가 |
| 전신 진동 | `copyRobotData` memcpy가 std::vector 손상 | rd_ 직접 사용, copyRobotData 제거 |
| ONNX output_tensors 메모리 손상 | 멤버변수 Ort::Value 반복 교체 시 힙 손상 | 로컬 변수로 변경 |
| velocity command 미연결 | 하드코딩 override | processObservation에서 override (임시) |

## 현재 상태

- ONNX policy 실행됨 ✓
- 첫 action은 합리적 (obs 순서 대략 맞음) ✓
- **로봇이 즉시 넘어짐** — action이 시간이 지나며 발산 (양성 피드백)
- PD만으로는 안정 (action=0이면 안정적으로 서있음)

## 발산 원인: sim-to-sim 물리 차이

IsaacLab(PhysX)과 MuJoCo의 물리가 다름:

| 파라미터 | IsaacLab | MuJoCo | 영향 |
|---|---|---|---|
| Physics dt | 0.005s | 0.001s | PD 응답 특성 다름 |
| PD 적용 주파수 | 200Hz (hold 4 step) | 1000Hz (매 tick) | 더 빠른 PD → 다른 동역학 |
| Joint damping | PhysX 내부 | 1.0~2.5 (추가 damping) | 과도한 damping |
| Friction loss | PhysX 내부 | 2.0~5.0 | 마찰 차이 |
| 접촉 모델 | PhysX | MuJoCo elliptic cone | 발 접촉 다름 |

## 다음 단계 (우선순위)

### 1. PD 주파수 맞추기 (IsaacLab처럼 200Hz hold)
cc.cpp에서 PD를 매 tick(1000Hz) 재계산하지 말고, 200Hz마다 계산하고 사이에 hold.
TOCABI의 `enable_timing_gates` + `pd_dt` 패턴 참고.

### 2. MuJoCo damping/friction 차이 (XML 수정 필요)
IsaacLab P73_CFG (p73_walker.py) 확인 결과:

| 파라미터 | IsaacLab | MuJoCo XML | 비고 |
|---|---|---|---|
| actuator stiffness | 0.0 | 0.0 | ✓ 일치 |
| actuator damping | **1.0 (전체)** | 1.0~**2.5** (관절별) | ✗ 불일치 |
| armature | per joint | per joint | ✓ 일치 |
| frictionloss | N/A | **2.0~5.0** | ✗ MuJoCo에만 존재 |

MuJoCo XML에서 damping을 전부 1.0으로, frictionloss를 0으로 맞춰야 함.
(현재 건들지 않음 — 추후 수정)

### 3. obs 값 덤프 비교
cc.cpp에서 첫 몇 프레임의 obs를 파일로 덤프하고,
IsaacLab에서 같은 초기 상태의 obs와 비교.

### 4. ONNX export 검증
IsaacLab에서 동일한 obs를 넣었을 때 같은 action이 나오는지 확인.

---

## 수정된 파일 목록

| 파일 | 수정 내용 |
|---|---|
| `p73.h` | JOINT_NAME MuJoCo 순서로 변경, PINOCCHIO_IDX_FOR_CODE 추가 |
| `state_estimator.cpp` | control_time_us_ 설정, Pinocchio 매핑, 진단 로그 |
| `p73_controller.cpp` | cc_init_ 리셋 |
| `cc.cpp` | copyRobotData 제거, rd_ 직접 사용, output_tensors 로컬화, 관절순서 통일 |
| `cc.h` | q_init_hold_ 추가 |
| `simulation.launch.py` | (현재 원래대로 유지) |
| `mjc_ros2/main.cpp` | 진단 로그 추가 |
