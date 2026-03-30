# catkin_ws (tocabi) 참조 분석 - ONNX 실행 파이프라인

이 문서는 Walker_ws에서 동일 기능을 구현할 때 참고할 핵심 구조를 정리한 것입니다.

## 핵심 패키지: tocabi_cc (ONNX Policy 실행)

**위치**: `/home/piene/catkin_ws/src/tocabi_cc/`

### ONNX 파일 위치
```
/home/piene/catkin_ws/src/tocabi_cc/policy/
├── policy.onnx
├── policy2.onnx
├── policy3.onnx
├── policy4.onnx
└── policy(hw1).onnx
```

### ONNX 로딩 (cc.cpp - loadNetwork())

```cpp
// 1. ONNX Runtime 세션 생성
Ort::Session session = Ort::Session(env, "policy/policy.onnx", session_options);

// 2. 입출력 텐서 정보 자동 파악
// Input:  "obs" tensor (1, 108*H)  H=history_length
// Output: "actions" (1, 33), "value", "latent"

// 3. 검증용 forward pass 실행
```

### 관측값 계산 (computeObservation()) - 48차원 프레임

```
[0:3]   Base linear velocity (pelvis frame)
[3:6]   Base angular velocity (pelvis frame)
[6:9]   Projected gravity
[9:12]  Velocity command (vx, vy, wz)
[12:24] Joint positions relative to initial (12 leg joints)
[24:57] Joint velocities (33 joints)
[57:90] Last action (33)
```

**정규화 주의**:
- 관절 속도: clip [-30, 30] rad/s → scale by 1/30
- 히스토리 버퍼: 순환 버퍼, 가장 오래된 프레임 좌측 시프트

### Action 처리 파이프라인

```
a_raw (ONNX output)
  → Δq = a_raw × 0.5          (action scale)
  → Δq = clamp(Δq, -1, 1)     (action clip)
  → q_des = q_init + Δq       (target position)
  → τ = (Kp/9)(q_des - q) - (Kd/3)q̇   (PD control)
  → τ = clamp(τ, -τ_max, +τ_max)        (torque limit)
```

**중요**: Scale BEFORE Clip 순서 필수! 순서 바꾸면 유효 Δq 범위 변경됨.

### 실행 주기
- 메인 컨트롤러: 2kHz
- ONNX 추론: 100Hz (computeSlow에서 실행)
- 모드 전환 시 첫 100ms spline transition (부드러운 토크 램프)

---

## Sim-to-Real 핵심 매칭 포인트

tocabi에서 검증된 **반드시 일치시켜야 하는 항목들**:

### 1. 관절 속도 정규화
- IsaacLab: clip [-30, 30] → scale 1/30
- MuJoCo: **동일하게** 적용 필수
- 미적용 시 policy 입력 분포 붕괴

### 2. Action Scale 순서
- IsaacLab: `scale=0.5` → `clip=[-1,1]`
- MuJoCo: `Δq = clamp(0.5 * a_raw, -1, 1)` (동일 순서)

### 3. 토크 클램핑
- IsaacLab: `torch.clamp(tau, -limits, +limits)`
- MuJoCo: 관절별 토크 한계 적용 필수
- 미적용 시 초기 토크 폭주

### 4. Last Action 관측값
- raw ONNX output (이전 스텝)
- 처리된 토크/PD 타겟이 **아님**

### 5. PD 게인 스케일링
- Kp/9, Kd/3 (IsaacLab 제어 스케일링과 매칭)

---

## catkin_ws 실행 방법 (참조)

```bash
# 1. Distrobox 진입
distrobox enter ros_noetic

# 2. Conda 환경 활성화
conda activate tocabirl

# 3. 워크스페이스 소싱
source /home/piene/catkin_ws/devel/setup.bash

# 4. (필요 시) 공유 메모리 초기화
rosrun tocabi_controller shm_reset

# 5. 시뮬레이션 실행
roslaunch tocabi_controller simulation.launch hand:=false gui:=true

# 6. GUI에서 Mode 7 선택 → ONNX Policy 실행
```

---

## 관련 디버그 문서

**위치**: `/home/piene/catkin_ws/debug_tools/`
- `TOCABI_IsaacLab_to_MuJoCo_policy_interface_update_2026-01-30.md`
- `IsaacLab_TO_MuJoCo_sim_to_sim_policy_manual.md`
- `TOCABI_MuJoCo_monitor_rqt_multiplot.md`

이 문서들에 policy 인터페이스 매칭에 대한 상세 디버깅 기록이 있음.

---

## Walker에서 구현 시 tocabi_cc에서 가져올 것

1. **ONNX Runtime 통합 코드**: `cc.cpp`의 loadNetwork(), feedforwardPolicy()
2. **관측값 계산 로직**: computeObservation() - DOF 차이에 맞게 수정 (33→13)
3. **Action 처리 파이프라인**: scale → clip → PD → torque limit
4. **히스토리 버퍼 관리**: 순환 버퍼 구현
5. **모드 전환 spline transition**: 안전한 토크 램프
