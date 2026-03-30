# Walker_ws 관절 순서 불일치 분석

> catkin_ws(TOCABI)에서는 MuJoCo XML / URDF / 코드 관절 순서가 **전부 동일**하여 문제가 없음.
> Walker_ws에서는 **3곳의 순서가 전부 다름** → 발산의 근본 원인.

---

## 1. 4가지 관절 순서 비교

### A. P73 코드 순서 (p73.h JOINT_NAME)
```
[0]  L_HipYaw        [6]  R_HipYaw
[1]  L_HipRoll       [7]  R_HipRoll
[2]  L_HipPitch      [8]  R_HipPitch
[3]  L_Knee          [9]  R_Knee
[4]  L_AnklePitch    [10] R_AnklePitch
[5]  L_AnkleRoll     [11] R_AnkleRoll
[12] WaistYaw
```

### B. MuJoCo XML / IsaacLab / launch file 순서
```
[0]  L_HipRoll       [6]  R_HipRoll
[1]  L_HipPitch      [7]  R_HipPitch
[2]  L_HipYaw        [8]  R_HipYaw
[3]  L_Knee          [9]  R_Knee
[4]  L_AnklePitch    [10] R_AnklePitch
[5]  L_AnkleRoll     [11] R_AnkleRoll
[12] WaistYaw
```

### C. Pinocchio (URDF BFS 탐색 순서)
```
[0]  WaistYaw
[1]  R_HipRoll       [2]  L_HipRoll
[3]  R_HipPitch      [4]  L_HipPitch
[5]  R_HipYaw        [6]  L_HipYaw
[7]  R_Knee          [8]  L_Knee
[9]  R_AnklePitch    [10] L_AnklePitch
[11] R_AnkleRoll     [12] L_AnkleRoll
```

### D. TOCABI (catkin_ws) - 참고: 전부 동일
```
MuJoCo XML = URDF = tocabi.h 코드 = Pinocchio 순서
모두 L_HipYaw → L_HipRoll → L_HipPitch → ... 동일 순서
→ 매핑 불필요, 인덱스 직접 복사로 동작
```

---

## 2. 데이터 흐름별 영향

### 읽기: MuJoCo → SHM → state_estimator → q_virtual_

```
mjc_ros2 controlCallback():
  for each joint in launch_joint_names:     // B순서 (Roll,Pitch,Yaw)
    joint_status.position_external[idx] = d->qpos[qpos_adr]  // 이름으로 매핑

state_estimator GetRobotData():
  q_ = Map<VectorQd>(robot_data.joint.position_external)  // B순서 그대로
  q_virtual_local_.segment(7, MODEL_DOF) = q_              // B순서가 q_virtual_에 들어감

  pinocchio::forwardKinematics(model_local_, data_local_, q_virtual_local_, ...)
  // Pinocchio는 C순서를 기대함 → B순서 데이터가 들어감 → 잘못된 kinematics!
```

**결과**: `link_[].xpos`, `link_[].rotm`, `link_[].jac` 등 **모든 kinematics가 틀림**
→ projected_gravity, base_ang_vel 등 observation도 영향받음

### 쓰기: torque_desired → SHM → MuJoCo d->ctrl

```
cc.cpp computeFast():
  torque_rl_[i] = Kp*(target-q) - Kd*vel   // 현재 MuJoCo/Isaac 순서(B)로 계산

p73_controller SendCommand():
  dc_.command_[i] = torque_desired[i]       // B순서

state_estimator SendCommand():
  joint_command.target_torque[i] = command[i]  // B순서

mjc_ros2:
  d->ctrl[i] = joint_command.target_torque[i]  // B순서 → actuator[i] (B순서)
  // actuator 순서 = MuJoCo XML 순서 = B순서 → 일치!
```

**쓰기 방향은 cc.cpp가 B순서로 계산하면 맞음** (현재 수정 후 상태).

---

## 3. 문제 요약

| 경로 | 현재 상태 | 문제? |
|---|---|---|
| MuJoCo → SHM (읽기) | 이름 매핑으로 B순서 | OK |
| SHM → q_ | 직접 복사 (B순서) | OK |
| q_ → Pinocchio | B순서 데이터를 C순서 모델에 넣음 | **문제!** |
| q_ → cc.cpp obs | B순서 데이터를 B순서로 사용 | OK (수정 후) |
| cc.cpp torque → SHM → d->ctrl | B순서 → B순서 actuator | OK (수정 후) |

**Pinocchio에 잘못된 관절 데이터가 들어가는 것**이 가장 큰 문제.
그러나 cc.cpp가 Pinocchio 결과(kinematics)를 직접 사용하는가?

---

## 4. cc.cpp가 Pinocchio에 의존하는 부분

cc.cpp의 processObservation()에서:
- `rd_cc_.q_virtual_` → 관절 위치/속도 직접 사용 (Pinocchio 무관)
- `rd_cc_.q_dot_virtual_` → 관절 속도 직접 사용 (Pinocchio 무관)
- quaternion: `rd_cc_.q_virtual_(3:6)` → IMU에서 직접 옴 (Pinocchio 무관)

**cc.cpp는 Pinocchio kinematics를 직접 사용하지 않음!**
obs에 사용하는 값은 전부 `q_virtual_`, `q_dot_virtual_`에서 직접 읽음.

따라서 Pinocchio 순서 불일치는 cc.cpp의 policy 실행에 **직접적 영향 없음**.
(다만 task mode 3, 4의 IK 모드에는 영향 있음)

---

## 5. 그러면 발산 원인은?

cc.cpp가 Pinocchio를 안 쓰고, 관절 데이터도 B순서로 통일했다면,
남은 가능성:
1. **ONNX 모델 자체 문제**: 학습이 충분하지 않거나, export 과정 오류
2. **gait_phase 파라미터**: period_steps = 50 (cc.cpp) vs 학습 시 다른 값?
3. **물리 파라미터 차이**: MuJoCo dt, damping, armature 등이 IsaacLab과 다름
4. **초기 자세**: MuJoCo keyframe과 IsaacLab default pose가 다름
5. **obs 값 범위**: IsaacLab에서는 noise를 추가하고 학습했지만 MuJoCo에서는 raw
6. **velocity command가 0**: teleop 없이 cmd=0이면 gait_phase도 0으로 고정됨

---

## 6. catkin_ws와의 구조적 차이 요약

| 항목 | catkin_ws (TOCABI) | Walker_ws (P73) | 영향 |
|---|---|---|---|
| MuJoCo/URDF/코드 순서 | **전부 동일** | **전부 다름** | ★ |
| SHM 읽기 | 인덱스 직접 복사 | 이름 매핑 (launch) | 읽기는 OK |
| SHM 쓰기 (ctrl) | 인덱스 직접 복사 | 인덱스 직접 복사 | cc가 B순서로 하면 OK |
| Pinocchio | 순서 일치 | 순서 불일치 | IK mode 문제 (cc 무관) |
| control_time_us_ | 설정됨 | **누락** (수정 완료) | ★ 수정됨 |
| cc_init_ 리셋 | tc_init으로 자동 | **누락** (수정 완료) | ★ 수정됨 |
| cc permutation | TOCABI↔Isaac 변환 | **불필요** (SHM=Isaac) | ★ 수정됨 |

---

> 작성일: 2026-03-30
