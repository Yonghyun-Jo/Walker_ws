# Walker sim-to-sim 최종 분석 (2026-03-30)

## 코드 검증 완료 — 모든 버그 수정됨

| 항목 | 상태 | 상세 |
|---|---|---|
| ONNX inference | ✓ 정확 | Python과 max diff 4.5e-7 |
| Obs 순서 (47D) | ✓ 일치 | ang_vel, gravity, cmd, gait, pos, vel, action 전부 IsaacLab과 동일 |
| ang_vel | ✓ 수정됨 | gyro 센서 = body frame, quatRotateInverse 제거 |
| joint_vel clip/scale | ✓ 적용 | clip(-30,30)/30 (새 학습 config와 일치) |
| Action 처리 | ✓ 일치 | scale(0.5) → clip(-1,1) → delta_default → PD → torque clamp |
| PD formula | ✓ 일치 | Kp*(target-q) - Kd*qdot (IsaacLab과 동일) |
| 관절 순서 | ✓ 통일 | MuJoCo XML = launch = IsaacLab = p73.h |
| Noise injection | ✓ 추가 | TOCABI-style processNoise (obs에만 적용) |
| History (term-major) | ✓ 일치 | 47D × 5 frames, shift-and-append |
| output_tensors 메모리 | ✓ 수정 | 로컬 변수 사용 (heap 손상 방지) |
| copyRobotData | ✓ 제거 | std::vector memcpy 메모리 손상 방지 |
| control_time_us_ | ✓ 수정 | state_estimator에서 설정 |
| cc_init_ 리셋 | ✓ 추가 | mode 전환 시 재초기화 |
| Pinocchio 매핑 | ✓ 추가 | URDF BFS ↔ MuJoCo 순서 변환 |

## 발산 원인: MuJoCo vs PhysX 물리 차이

첫 action은 합리적 (±0.44). 하지만 **1 policy step (20ms) 후** 관절 속도가 급격히 증가:
- Step 1: q_vel ≈ 0
- Step 3 (40ms): q_vel ≈ 0.4 rad/s
- Step 5 (80ms): q_vel ≈ 5.4 rad/s ← PD 토크 포화

MuJoCo에서 같은 토크에 다른 물리 응답 → obs가 학습 분포 이탈 → 더 큰 action → 발산

### 테스트된 물리 파라미터:
- damping=원본(1.0~2.5): 발산
- damping=1.0, frictionloss=0: 발산 (약간 개선)
- damping=0, frictionloss=0: 발산 (action 크기 감소 확인)

## TOCABI와의 핵심 차이

| | TOCABI | Walker |
|---|---|---|
| MuJoCo vel 소스 | `d->qvel[3:6]` (world frame) | gyro sensor (body frame) |
| ang_vel 처리 | quatRotateInverse 필요 | 직접 사용 (수정됨) |
| PD gain 스케일 | /9, /3 | 없음 (학습과 일치) |
| joint_vel obs | clip(-30,30)/30 | clip(-30,30)/30 (추가됨) |
| Noise | processNoise (obs+PD) | processNoise (obs만) |
| MuJoCo joint 순서 | = 코드 순서 | 다름 → 매핑 추가됨 |

## 다음 단계 제안

1. **IsaacLab에서 MuJoCo DR 강화** — MuJoCo의 물리 특성을 DR 범위에 포함
2. **MuJoCo XML 튜닝** — damping=0, frictionloss=0으로 재학습
3. **MuJoCo에서 직접 학습** — IsaacGym/PhysX 대신 MuJoCo 사용
4. **action clipping/smoothing** — cc.cpp에서 action에 EMA 적용해서 급격한 변화 완화
