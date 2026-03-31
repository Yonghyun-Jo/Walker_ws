# 13. Walker (P73) Real Robot 실험 가이드

> **로봇 컴퓨터 유저**: dyros (또는 해당 유저)
> **로봇 Walker_ws 경로**: `/home/dyros/Walker_ws/`
> **ONNX policy 경로**: `/home/dyros/Walker_ws/src/p73_walker_cc/policy/policy.onnx`
> **ROS2**: Jazzy
> **내 노트북과 로봇이 같은 네트워크에 있어야 함**

---

## 개요

TOCABI 실험과 동일한 패턴:
- 로봇 컴퓨터에 `p73_walker_cc`만 교체 → 빌드 → 실행
- 내 노트북에서 GUI + teleop 제어

**차이점**: Walker는 ROS2 (TOCABI는 ROS1), 별도 rosparam 설정 불필요 (cc.cpp에서 `is_on_robot_` 직접 변경)

---

## STEP 1. SSH로 로봇 컴퓨터 접속

```bash
# 터미널 A (빌드 + 실행용)
ssh dyros@<로봇IP>
```

---

## STEP 2. p73_walker_cc만 교체

```bash
# 임시 위치에 clone
cd ~/Downloads
git clone https://github.com/Yonghyun-Jo/Walker_ws.git walker_ws_new

# 기존 p73_walker_cc 백업
cp -r ~/Walker_ws/src/p73_walker_cc ~/Walker_ws/src/p73_walker_cc.bak

# p73_walker_cc 내용만 덮어쓰기 (다른 패키지는 절대 건드리지 않음)
cp -r ~/Downloads/walker_ws_new/src/p73_walker_cc/* ~/Walker_ws/src/p73_walker_cc/

# 임시 폴더 삭제
rm -rf ~/Downloads/walker_ws_new
```

> **주의**: `p73_walker_cc` 이외 폴더(`p73_controller`, `p73_lib`, `mujoco_ros2_sim` 등)는 건드리지 않는다. 로봇 컴퓨터에 이미 빌드된 환경이 깨질 수 있다.

---

## STEP 3. is_on_robot 설정

cc.h에서 `is_on_robot_`를 `true`로 변경:

```bash
cd ~/Walker_ws/src/p73_walker_cc
sed -i 's/bool is_on_robot_ = false/bool is_on_robot_ = true/' include/cc.h
```

확인:
```bash
grep "is_on_robot_" include/cc.h
# → bool is_on_robot_ = true;
```

**`is_on_robot_ = true`일 때 processNoise 동작:**
- joint position: 센서값 직접 사용 (노이즈 없음)
- joint velocity: 센서값 직접 사용 + 4Hz LPF
- (sim에서는 노이즈 추가 + 수치미분 + LPF)

---

## STEP 4. ONNX 파일 배치

학습한 policy.onnx를 로봇 컴퓨터로 복사:

```bash
# 내 노트북에서 (별도 터미널)
scp ~/Walker_ws/src/p73_walker_cc/policy/policy.onnx dyros@<로봇IP>:~/Walker_ws/src/p73_walker_cc/policy/policy.onnx
```

확인 (로봇 컴퓨터에서):
```bash
python3 -c "
import onnxruntime as ort
s = ort.InferenceSession('/home/dyros/Walker_ws/src/p73_walker_cc/policy/policy.onnx')
for inp in s.get_inputs(): print(f'{inp.name}: {inp.shape}')
"
# obs: [1, 235]  (H=5) 또는 [1, 470] (H=10) 이어야 함
```

ONNX 경로는 cc.cpp에서 `$HOME/Walker_ws/src/p73_walker_cc/policy/policy.onnx`로 하드코딩되어 있으므로, 로봇 유저의 HOME이 `/home/dyros`이면 자동으로 맞음.

---

## STEP 5. 빌드 (p73_walker_cc + p73_controller)

```bash
cd ~/Walker_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash

# p73_walker_cc는 정적 라이브러리 → p73_controller도 함께 빌드 필수!
colcon build --packages-select p73_walker_cc p73_controller \
  --cmake-args -DCMAKE_BUILD_TYPE=Release

# p73_controller가 캐시되면 강제 재빌드
touch src/p73_walker_controller/p73_controller/src/*.cpp
colcon build --packages-select p73_controller \
  --cmake-args -DCMAKE_BUILD_TYPE=Release
```

빌드 후 source:
```bash
source install/setup.bash
```

---

## STEP 6. 실제 로봇 launch (로봇 컴퓨터)

```bash
# 터미널 A (ssh 접속 상태)
cd ~/Walker_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash

# 실로봇 launch (ecat_master, imu 자동 시작)
sudo ros2 launch p73_controller realrobot.launch.py
```

콘솔에서 확인:
```
[p73_walker_cc] Loading network from /home/dyros/Walker_ws/src/p73_walker_cc/policy/policy.onnx
[p73_walker_cc] Inferred policy_obs_dim=235 (history_length=5)
[p73_walker_cc] Mode started (is_on_robot=1)
```

---

## STEP 7. GUI 실행 (내 노트북)

```bash
# 터미널 B (노트북)
# ROS2는 DDS 기반이므로 같은 네트워크면 자동 discovery
# 필요 시 ROS_DOMAIN_ID 설정
export ROS_DOMAIN_ID=0  # 로봇과 동일해야 함

cd ~/Walker_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash

ros2 launch p73_walker_gui gui.launch.py
```

GUI에서 순서:
1. **Torque On** → 모터 활성화
2. **Init Pos** → 초기 자세로 이동 (이때 로봇 내려야 함)
3. **Init Yaw** → yaw 기준 설정
4. **State Estimate** → 상태 추정 시작
5. **Task Mode 7** → Send → ONNX policy 시작

---

## STEP 8. 키보드 teleop (내 노트북)

```bash
# 터미널 C (노트북)
export ROS_DOMAIN_ID=0

cd ~/Walker_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash

python3 src/p73_walker_cc/scripts/walker_teleop.py
```

조작:
| 키 | 동작 |
|---|---|
| **W/S** | 전진/후진 (vx ±0.1) |
| **A/D** | 좌/우 이동 (vy ±0.1) |
| **Q/E** | 좌/우 회전 (wz ±0.1) |
| **Space** | 전부 0 리셋 (제자리 서기) |
| **Ctrl+C** | 종료 |

---

## STEP 9. 비상 정지

- GUI에서 **Torque Off** 또는 **Task Mode 0** → Send
- 또는 로봇 컴퓨터에서 `Ctrl+C`로 launch 종료
- E-stop 물리 버튼 사용

---

## TOCABI vs Walker 실험 절차 대조

| 항목 | TOCABI (catkin_ws) | Walker (Walker_ws) |
|------|-------------------|-------------------|
| ROS | ROS1 Noetic | ROS2 Jazzy |
| 컨트롤러 패키지 | tocabi_cc | p73_walker_cc |
| 빌드 | `cm` (catkin_make) | `colcon build --packages-select p73_walker_cc p73_controller` |
| is_on_robot 설정 | `rosparam set /tocabi_cc/is_on_robot true` | cc.h에서 `is_on_robot_ = true` 수정 후 재빌드 |
| ONNX 경로 설정 | `rosparam set /tocabi_cc/weight_dir <path>` | 하드코딩 (`$HOME/Walker_ws/src/p73_walker_cc/policy/policy.onnx`) |
| timing gate | `rosparam set /tocabi_cc/enable_timing_gates true` | 없음 (매 tick PD) |
| GUI | `roslaunch tocabi_gui gui.launch` | `ros2 launch p73_walker_gui gui.launch.py` |
| teleop | `python3 tocabi_teleop.py` (ROS1, Joy msg) | `python3 walker_teleop.py` (ROS2, Twist msg) |
| 네트워크 설정 | `ROS_MASTER_URI`, `ROS_IP` | `ROS_DOMAIN_ID` (DDS auto-discovery) |
| launch | `roslaunch tocabi_controller realrobot.launch` | `sudo ros2 launch p73_controller realrobot.launch.py` |

---

## 체크리스트 (실험 전 확인)

- [ ] `is_on_robot_ = true` 확인 (`grep is_on_robot_ include/cc.h`)
- [ ] policy.onnx가 올바른 파일인지 확인 (onnxruntime으로 shape 체크)
- [ ] `p73_walker_cc` + `p73_controller` 둘 다 빌드했는지 확인
- [ ] `source install/setup.bash` 했는지 확인
- [ ] 로봇-노트북 같은 네트워크인지 확인
- [ ] `ROS_DOMAIN_ID` 동일한지 확인
- [ ] teleop 토픽 `/p73/cmd_vel` 확인 (`ros2 topic list`)
