# p73_walker_cc 외부 코드 수정 사항 정리

> p73_walker_cc는 개인 코드이므로 제외. 아래는 **연구실 공용 코드** 수정분.
> 총 3개 파일, 3가지 수정. 모두 기존 mode 0-4, position control, gravity comp 동작에 **영향 없음**.

---

## 1. `p73.h` — JOINT_NAME 순서 통일 + Pinocchio 매핑

**파일**: `p73_walker_controller/p73_lib/include/p73_lib/p73.h`

### 1-1. JOINT_NAME 순서 변경

**변경 전** (Yaw, Roll, Pitch 순서):
```cpp
"L_HipYaw_Joint", "L_HipRoll_Joint", "L_HipPitch_Joint", ...
"R_HipYaw_Joint", "R_HipRoll_Joint", "R_HipPitch_Joint", ...
```

**변경 후** (Roll, Pitch, Yaw 순서):
```cpp
"L_HipRoll_Joint", "L_HipPitch_Joint", "L_HipYaw_Joint", ...
"R_HipRoll_Joint", "R_HipPitch_Joint", "R_HipYaw_Joint", ...
```

**왜 바꿨는가?**

Walker에는 3가지 서로 다른 관절 순서가 존재했음:

| 시스템 | 순서 |
|---|---|
| MuJoCo XML (body tree) | Roll, Pitch, Yaw |
| MuJoCo actuator | Roll, Pitch, Yaw |
| launch file joint_names | Roll, Pitch, Yaw |
| IsaacLab _LOWER_JOINT_NAMES | Roll, Pitch, Yaw |
| YAML PD gains | Roll, Pitch, Yaw |
| **p73.h (변경 전)** | **Yaw, Roll, Pitch** ← 유일하게 다름 |

p73.h만 다른 순서를 사용하면:
- SHM에서 온 joint 데이터(Roll,Pitch,Yaw)를 p73.h 순서(Yaw,Roll,Pitch)로 해석 → **관절 값 뒤섞임**
- MuJoCo `d->ctrl[]`에 토크를 넣을 때도 **잘못된 관절에 토크 적용**

**TOCABI(catkin_ws)에서는 문제 없었던 이유:**
TOCABI의 MuJoCo XML body tree 순서와 tocabi.h JOINT_NAME 순서가 **모두 동일** (Yaw, Roll, Pitch).
Walker에서는 MuJoCo XML이 다른 순서 (Roll, Pitch, Yaw)를 사용하므로 p73.h를 맞춰야 했음.

**영향 범위:**
- cc (mode 5-9): 관절 데이터 순서가 올바르게 됨
- mode 0-4: `rd_.q_`, `rd_.Kp_j`, YAML gain 등이 모두 동일한 순서이므로 영향 없음
- GUI joint state 표시: JOINT_NAME 순서가 바뀌므로 표시 순서만 변경 (값은 동일)

### 1-2. PINOCCHIO_IDX_FOR_CODE 매핑 추가

```cpp
const int PINOCCHIO_IDX_FOR_CODE[MODEL_DOF] = {
    2, 4, 6, 8, 10, 12,    // L leg
    1, 3, 5, 7,  9, 11,    // R leg
    0                       // WaistYaw
};
```

**왜 필요한가?**

Pinocchio는 URDF를 BFS 탐색하여 관절 순서를 결정함. Walker URDF의 BFS 순서:
```
[0]WaistYaw, [1]R_HipRoll, [2]L_HipRoll, [3]R_HipPitch, [4]L_HipPitch,
[5]R_HipYaw, [6]L_HipYaw, [7]R_Knee, [8]L_Knee, ...
```

이것은 p73.h(= MuJoCo) 순서와 다름. Pinocchio에 관절 데이터를 넣을 때/꺼낼 때 매핑 필요.

**TOCABI에서는 문제 없었던 이유:**
TOCABI URDF의 BFS 순서 = tocabi.h 순서 = MuJoCo 순서 (모두 동일).

---

## 2. `state_estimator.cpp` — 3가지 수정

**파일**: `p73_walker_controller/p73_controller/src/state_estimator.cpp`

### 2-1. GetRobotData(): 코드→Pinocchio 순서 매핑

**변경 전:**
```cpp
q_virtual_local_.segment(7, MODEL_DOF) = q_;
q_dot_virtual_local_.segment(6, MODEL_DOF) = q_dot_;
```

**변경 후:**
```cpp
for (int i = 0; i < MODEL_DOF; i++) {
    q_virtual_local_(7 + P73::PINOCCHIO_IDX_FOR_CODE[i]) = q_(i);
    q_dot_virtual_local_(6 + P73::PINOCCHIO_IDX_FOR_CODE[i]) = q_dot_(i);
}
```

**왜:** `q_`는 SHM에서 온 MuJoCo 순서 데이터. `q_virtual_local_`은 Pinocchio에 전달됨.
Pinocchio는 URDF BFS 순서를 기대하므로 매핑 필요. 이것이 없으면 `forwardKinematics` 결과가 틀림.
(mode 3, 4의 IK 계산에 영향. mode 5-9의 cc는 Pinocchio를 직접 사용하지 않으므로 무관.)

### 2-2. StoreState(): Pinocchio→코드 순서 역매핑

**변경 전:**
```cpp
memcpy(&rd_global_.q_virtual_, &q_virtual_, sizeof(VectorQVQd));
memcpy(&rd_global_.q_dot_virtual_, &q_dot_virtual_, sizeof(VectorVQd));
```

**변경 후:**
```cpp
{
    VectorQVQd q_virtual_code = q_virtual_;
    VectorVQd q_dot_virtual_code = q_dot_virtual_;
    for (int i = 0; i < MODEL_DOF; i++) {
        q_virtual_code(7 + i) = q_virtual_(7 + P73::PINOCCHIO_IDX_FOR_CODE[i]);
        q_dot_virtual_code(6 + i) = q_dot_virtual_(6 + P73::PINOCCHIO_IDX_FOR_CODE[i]);
    }
    memcpy(&rd_global_.q_virtual_, &q_virtual_code, sizeof(VectorQVQd));
    memcpy(&rd_global_.q_dot_virtual_, &q_dot_virtual_code, sizeof(VectorVQd));
}
```

**왜:** `q_virtual_`은 Pinocchio 순서. `rd_global_.q_virtual_`은 cc와 controller가 사용.
cc는 MuJoCo/IsaacLab 순서를 기대하므로 역매핑 필요.
base pos/quat (index 0-6)은 매핑 불필요 (joint만 7+ 인덱스 매핑).

### 2-3. StoreState(): control_time_us_ 설정

**추가:**
```cpp
rd_global_.control_time_us_ = static_cast<int64_t>(control_time_ * 1e6);
```

**왜:** cc.cpp의 모든 타이밍 로직이 `control_time_us_`를 사용 (spline 전환, policy 주기 등).
원래 설정되지 않아서 항상 0 → cc의 spline이 영원히 초기 토크만 출력.
TOCABI(catkin_ws)에서는 `rd_gl_.control_time_us_ = dur_start_`로 설정되어 있음.

---

## 3. `p73_controller.cpp` — cc_init_ 리셋

**파일**: `p73_walker_controller/p73_controller/src/p73_controller.cpp`
**위치**: `taskCmdCallback()` 함수

**추가:**
```cpp
#ifdef COMPILE_P73_CC
    cc_.cc_init_ = true;
#endif
```

**왜:** task mode 전환 시 cc의 내부 상태(action history, observation buffer, start_time 등)를
재초기화해야 함. 없으면 이전 세션의 잘못된 상태가 남아서 발산.
TOCABI에서는 `rd_cc_.tc_init` 플래그로 동일한 기능 수행.
`#ifdef` 가드로 cc 미사용 시 컴파일 오류 방지.

---

## 요약

| # | 파일 | 수정 | 영향 | 없으면? |
|---|---|---|---|---|
| 1-1 | p73.h | JOINT_NAME 순서 변경 | 전체 관절 데이터 흐름 | 관절 뒤섞임, 잘못된 토크 적용 |
| 1-2 | p73.h | PINOCCHIO_IDX_FOR_CODE 추가 | Pinocchio kinematics | IK 결과 틀림 (mode 3,4) |
| 2-1 | state_estimator.cpp | 코드→Pinocchio 매핑 | Pinocchio FK/dynamics | 위와 동일 |
| 2-2 | state_estimator.cpp | Pinocchio→코드 역매핑 | rd_.q_virtual_ | cc obs/PD에 잘못된 순서 전달 |
| 2-3 | state_estimator.cpp | control_time_us_ 설정 | cc 타이밍 전체 | RL 토크 절대 적용 안 됨 |
| 3 | p73_controller.cpp | cc_init_ 리셋 | mode 재진입 | 재초기화 불가 |

모두 기존 동작(mode 0-4, position control, gravity comp)에 **영향 없음**.
cc를 사용하지 않는 경우에도 안전.

---

> 작성일: 2026-03-31
> 참고: TOCABI(catkin_ws yh-basic)에서는 MuJoCo/URDF/코드 순서가 전부 동일하여 매핑 불필요.
> Walker에서는 MuJoCo XML body tree 순서가 p73.h와 달라서 매핑이 필요했음.
