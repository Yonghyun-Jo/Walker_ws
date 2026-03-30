# 12. Walker Sim-to-Sim 디버깅 & 해결 기록

> **날짜**: 2026-03-30 ~ 03-31
> **상태**: 해결 완료 (IsaacLab -> MuJoCo 보행 성공)
> **핵심 원인**: obs history 레이아웃 불일치 (term-major vs frame-major)

---

## 1. 문제 요약

IsaacLab에서 학습한 ONNX policy를 MuJoCo(Walker_ws)에서 실행하면 **지속적인 진동 후 넘어짐**. 같은 방식으로 TOCABI(catkin_ws)에서는 sim-to-sim이 바로 성공했었음.

---

## 2. 근본 원인: History 레이아웃 불일치

### 2.1 Frame-major vs Term-major

Policy obs는 47D 프레임이 5개 쌓인 235D 벡터. 이를 펼치는 방식이 두 가지:

**Frame-major (IsaacLab 실제 방식, 정답):**
```
[frame0 전체 47D] [frame1 전체 47D] ... [frame4 전체 47D]

인덱스 예시:
  [0:3]    frame0.ang_vel
  [3:6]    frame0.gravity
  ...
  [35:47]  frame0.last_action
  [47:50]  frame1.ang_vel
  ...
  [188:235] frame4 전체 (newest)
```

**Term-major (cc.cpp가 사용하던 잘못된 방식):**
```
[ang_vel 5프레임분 15D] [gravity 15D] [cmd 15D] [sin 5D] [cos 5D] [jpos 60D] [jvel 60D] [act 60D]

인덱스 예시:
  [0:3]   frame0.ang_vel
  [3:6]   frame1.ang_vel
  ...
  [15:18] frame0.gravity
  ...
```

같은 235개 숫자지만 **순서가 완전히 다르다.** ONNX 모델은 frame-major를 기대하는데 term-major를 주면 모든 feature가 뒤섞여서 policy가 엉뚱한 action을 출력.

### 2.2 왜 이런 오해가 생겼나

IsaacLab `ObservationManager`는 각 term마다 별도 `CircularBuffer`를 만들고 `flatten_history_dim=True`로 reshape 후 `torch.cat`으로 concat한다. 소스 코드만 보면 term-major처럼 보이지만, **`P73ObservationManager` (커스텀)가 전체 47D를 하나의 단위로 처리**하여 실제로는 frame-major 출력을 만든다.

### 2.3 검증 방법 (결정적 증거)

IsaacLab에서 덤프한 235D obs를 두 방식으로 파싱:

```python
# Frame-major 해석 (정답):
Frame 0 [0:47]:  gravity=[0, 0, -1.0]  sin=0  cos=1  -- 물리적으로 맞음

# Term-major 해석 (오답):
gravity=[0, 0, 0]  sin=0.274  cos=0  jpos=[0, 1, ...]  -- 엉망
```

gravity가 [0,0,-1] (직립 로봇)이어야 하는데 term-major로 읽으면 [0,0,0]이 됨.

---

## 3. 추가로 수정한 항목

### 3.1 obs 관절속도: 수치미분 -> 직접 qvel

| | 변경 전 | 변경 후 |
|---|---|---|
| 위치 obs | `s_q_noise` (노이즈 추가) | `rd_.q_` (직접) |
| 속도 obs | `s_q_vel_noise` (수치미분) | `rd_.q_dot_` (MuJoCo qvel 직접) |

TOCABI catkin_ws에서도 `q_vel_ = rd_cc_.q_dot_virtual_.segment<33>(6)` 으로 시뮬레이터 qvel을 직접 사용. 수치미분은 불필요한 차이를 만든다.

### 3.2 노이즈 코드 제거

TOCABI cc.cpp에는 노이즈 주입이 없음. Walker에 있던 `s_noise_gen`, `s_noise_dis` 등 static 변수와 매 tick 업데이트 블록 전부 제거.

### 3.3 빌드 주의사항

`p73_walker_cc`는 **정적 라이브러리** (`libp73_walker_cc.a`)이므로, 이것을 링크하는 `p73_controller`도 함께 빌드해야 변경이 반영됨:

```bash
source /opt/ros/jazzy/setup.bash
source ~/Walker_ws/install/setup.bash
colcon build --packages-select p73_walker_cc p73_controller \
  --cmake-args -DCMAKE_BUILD_TYPE=Release
```

`p73_walker_cc`만 빌드하면 **이전 바이너리가 계속 실행**된다!

---

## 4. 시도했으나 원인이 아니었던 것들

| 시도 | 결과 | 비고 |
|------|------|------|
| MuJoCo timestep 0.001 -> 0.005 | 약간 나아졌으나 여전히 넘어짐 | 물리 차이는 부차적 |
| MuJoCo timestep 0.0005 (TOCABI 동일) | 여전히 넘어짐 | |
| noslip_iterations 30 -> 0 | 여전히 넘어짐 | |
| balanceinertia="true" 추가 | 여전히 넘어짐 | |
| gyro noise/cutoff 제거 | 여전히 넘어짐 | |
| PD 200Hz 제한 (ZOH) | 주저앉으며 넘어짐 | mode7 진입 시 순간 토크 빠짐 |
| PD 매 tick (TOCABI 동일) | 진동 지속 | 근본 원인이 아니었음 |

**결론: MuJoCo XML 물리 파라미터나 PD 주기는 원인이 아니었고, obs history 레이아웃이 유일한 근본 원인이었다.**

---

## 5. 최종 cc.cpp 핵심 코드

### 5.1 Frame-major history 구현

```cpp
// Frame-major: [frame0(47D), frame1(47D), ..., frame4(47D)]
const int H = history_length_;  // 5
const int F = num_single_obs;   // 47

if (!policy_hist_initialized_) {
    // 첫 프레임으로 모든 H 슬롯 채움
    for (int t = 0; t < H; ++t)
        std::memcpy(buf + t * F, policy_frame_.data(), sizeof(float) * F);
    policy_hist_initialized_ = true;
} else {
    // 왼쪽으로 1 프레임 shift (oldest 제거), 끝에 newest 추가
    std::memmove(buf, buf + F, sizeof(float) * F * (H - 1));
    std::memcpy(buf + F * (H - 1), policy_frame_.data(), sizeof(float) * F);
}
```

이것은 TOCABI catkin_ws의 방식과 동일:
```cpp
// TOCABI cc.cpp line 208-210:
copy(obs_buffer_.begin() + state_current_num_, obs_buffer_.end(), obs_buffer_.begin());
copy(state_cur_.begin(), state_cur_.end(), obs_buffer_.end() - state_current_num_);
```

### 5.2 47D 프레임 구성 (obs 순서)

```
idx  dim  term           source
0:3   3   ang_vel        rd_.q_dot_virtual_.segment<3>(3) (gyro body-frame)
3:6   3   gravity        quatRotateInverse(q, [0,0,-1])
6:9   3   cmd            [vel_x, vel_y, vel_yaw]
9     1   gait_sin       sin(2pi * phase)
10    1   gait_cos       cos(2pi * phase)
11:23 12  joint_pos_rel  rd_.q_ - q_default_isaac_
23:35 12  joint_vel      clip(rd_.q_dot_, -30, 30) / 30
35:47 12  last_action    clip(prev_action * 0.5, -1, 1)
```

---

## 6. Obs 덤프 & 비교 도구 (추후 디버깅용)

진동/발산 문제 재발 시 IsaacLab vs MuJoCo obs를 정밀 비교하는 도구.

### 6.1 IsaacLab 덤프

**파일**: `isaaclab_walker/scripts/rsl_rl/play_dump_obs.py`

```bash
cd ~/isaaclab5.2/isaaclab_walker
python scripts/rsl_rl/play_dump_obs.py \
    --task=Walker-Flat-Play --num_envs=1 \
    --checkpoint=<checkpoint_path> \
    --dump_steps 25
```

출력: `/tmp/walker_isaac_obs.jsonl`

### 6.2 MuJoCo 덤프

cc.cpp에 내장. task mode 7 실행 시 **자동으로 첫 25 policy step** 덤프.

출력: `/tmp/walker_mujoco_obs.jsonl`

> **주의**: 현재 cmd가 `(0,0,0)`으로 하드코딩되어 있음. 정상 운용 시 `local_vel_x = 0.5` 등으로 복원 필요 (cc.cpp line ~205).

### 6.3 비교 스크립트

**파일**: `Walker_ws/scripts/compare_obs_dump.py`

```bash
python ~/Walker_ws/scripts/compare_obs_dump.py \
    --isaac /tmp/walker_isaac_obs.jsonl \
    --mujoco /tmp/walker_mujoco_obs.jsonl \
    --max_steps 10 --per_dim
```

### 6.4 JSONL 레코드 구조

```json
{
  "step": 0,
  "obs_235": [float x 235],
  "actions": [float x 12],
  "frame_47": {
    "ang_vel": [3 floats],
    "gravity": [3 floats],
    "cmd": [3 floats],
    "gait_sin": float,
    "gait_cos": float,
    "joint_pos": [12 floats],
    "joint_vel": [12 floats],
    "last_action": [12 floats]
  },
  "raw": {
    "quat_xyzw": [4 floats],
    "ang_vel_body": [3 floats],
    "joint_pos": [13 floats],
    "joint_vel": [13 floats]
  }
}
```

### 6.5 비교 결과 해석 체크리스트

| 증상 | 1순위 의심 | 2순위 의심 |
|------|-----------|-----------|
| gravity ≈ [0,0,0] 또는 magnitude != 1 | history 레이아웃 틀림 | 쿼터니언 convention |
| gravity z = +1 (부호 반전) | 쿼터니언 (w,x,y,z) vs (x,y,z,w) | quatRotateInverse 공식 |
| joint_vel 전체 mismatch | 속도 소스 (qvel vs 수치미분) | 단위/스케일링 |
| joint_pos 순서 뒤바뀜 | 관절 permutation | IsaacLab joint_names 순서 |
| sin/cos 값 엉뚱 | history 레이아웃 | gait period mismatch |
| actions 작지만 obs 유사 | ONNX normalizer 누락 | action_scale 불일치 |

### 6.6 빠른 레이아웃 검증법

```python
import json
with open("/tmp/walker_isaac_obs.jsonl") as f:
    obs = json.loads(f.readline())["obs_235"]

# Frame-major면: obs[3:6] ≈ [0, 0, -1] (직립 시 gravity)
print("Frame-major gravity:", obs[3:6])

# Term-major면: obs[15:18]이 gravity일 것 (3*H=15 offset)
print("Term-major gravity:", obs[15:18])

# 둘 중 [0, 0, -1]에 가까운 쪽이 정답
```

---

## 7. Walker vs TOCABI 대조표 (검증 완료)

| 항목 | IsaacLab 학습 | cc.cpp (수정 후) | TOCABI catkin_ws | 일치 |
|------|-------------|-----------------|-----------------|------|
| history 레이아웃 | **frame-major** | **frame-major** | frame-major | ✅ |
| obs 속도 | simulator qvel | `rd_.q_dot_` | `rd_cc_.q_dot_virtual_` | ✅ |
| obs 노이즈 | 학습 시만 (inference 없음) | 없음 | 없음 | ✅ |
| PD 실행 | 매 physics step | 매 tick | 매 computeSlow() | ✅ |
| PD 게인 | [1536, 937.5, ...] 직접 | 동일 | kp/9, kd/3 (다른 로봇) | ✅ |
| action scale | 0.5 | 0.5 | range mapping | ✅ |
| policy 주기 | 50Hz (dt=0.005, dec=4) | 50Hz (policy_dt=0.02) | 100Hz | ✅ |
| gait period | 50 steps | 50 steps | N/A | ✅ |
| 쿼터니언 | Pinocchio (x,y,z,w) | q_virtual_(3..6) | q_virtual_(3..6) | ✅ |
| 각속도 | body-frame | gyro (body-frame) | qvel 회전 (body-frame) | ✅ |
| ONNX normalizer | 모델 내장 | 자동 적용 | 자동 적용 | ✅ |

---

## 8. 교훈 & 향후 주의사항

1. **코드 리뷰만으로는 부족하다** — history 레이아웃 문제는 코드상 올바르게 보였으나, 실제 obs 덤프를 비교해야 발견 가능했다.

2. **커스텀 ObservationManager 주의** — `P73ObservationManager`가 표준 IsaacLab과 다른 레이아웃을 만들 수 있다. 항상 실제 출력을 확인.

3. **정적 라이브러리 빌드 체인** — `p73_walker_cc`만 빌드하면 안 되고, 링크하는 `p73_controller`도 반드시 함께 빌드해야 한다.

4. **덤프는 짧게, 비교는 즉시** — 25 step (0.5초)이면 충분. 발산 전에 첫 mismatch를 찾는 것이 핵심.

5. **gravity 값이 1차 진단 도구** — 직립 시 gravity ≈ [0, 0, -1]이 아니면 즉시 레이아웃 또는 쿼터니언 문제.
