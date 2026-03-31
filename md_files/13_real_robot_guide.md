# 13. Walker (P73) Real Robot 실험 가이드

> **로봇 컴퓨터에서 직접 작업** (SSH 아닌 현장 작업)
> **ROS2 Jazzy**, Walker_ws 경로는 로봇 컴퓨터의 `~/Walker_ws/`

---

## STEP 1. 로봇 컴퓨터에서 git clone

```bash
cd ~/Downloads
git clone -b fix https://github.com/Yonghyun-Jo/Walker_ws.git walker_ws_new
```

---

## STEP 2. 기존 p73_cc 백업 + 교체

```bash
# 백업
cp -r ~/Walker_ws/src/p73_cc ~/Walker_ws/src/p73_cc.bak

# 내용만 덮어쓰기 (p73_cc 이외 패키지는 절대 건드리지 않음)
cp -r ~/Downloads/walker_ws_new/src/p73_cc/* ~/Walker_ws/src/p73_cc/

# 임시 폴더 삭제
rm -rf ~/Downloads/walker_ws_new
```

---

## STEP 3. is_on_robot 변경

```bash
cd ~/Walker_ws/src/p73_cc
sed -i 's/bool is_on_robot_ = false/bool is_on_robot_ = true/' include/cc.h
```

확인:
```bash
grep "is_on_robot_" include/cc.h
# → bool is_on_robot_ = true;  이면 OK
```

**이게 하는 일:**
- `false` (sim): 관절위치에 노이즈 추가 + 속도를 수치미분으로 계산
- `true` (real): 센서값 직접 사용 + 4Hz 저역통과 필터(LPF)

---

## STEP 4. ONNX 파일 확인

policy.onnx가 올바른 파일인지 확인:

```bash
python3 -c "
import onnxruntime as ort
s = ort.InferenceSession('$HOME/Walker_ws/src/p73_cc/policy/policy.onnx')
for inp in s.get_inputs(): print(f'{inp.name}: {inp.shape}')
"
```

`obs: [1, 235]` (H=5) 또는 `[1, 470]` (H=10) 이어야 함.

필요시 USB 등으로 새 policy.onnx 복사:
```bash
cp /media/<USB>/policy.onnx ~/Walker_ws/src/p73_cc/policy/policy.onnx
```

---

## STEP 5. 빌드

```bash
cd ~/Walker_ws
source /opt/ros/jazzy/setup.bash

# 1) p73_cc 먼저 빌드 (install에 등록되어야 p73_controller가 찾음)
colcon build --packages-select p73_cc --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash

# 2) p73_controller 캐시 삭제 후 재빌드 (반드시!)
rm -rf build/p73_controller
colcon build --packages-select p73_controller --cmake-args -DCMAKE_BUILD_TYPE=Release
# → ~1분 걸려야 정상. 0.x초면 ONNX 없이 빌드된 것!

# 3) 빌드 후 반드시 source
source install/setup.bash
```

> **왜 이 순서?**
> - `p73_cc`는 정적 라이브러리(.a). `p73_controller`가 이걸 링크해서 실행 파일을 만듦.
> - `p73_controller`가 `find_package(p73_cc QUIET)` → 못 찾으면 **ONNX 코드가 조용히 빠짐**.
> - `build/p73_controller` 캐시에 "못 찾음"이 기록되면 이후 빌드에서도 계속 못 찾음.
> - **`rm -rf build/p73_controller`가 가장 확실한 해결책.**

---

## STEP 6. 실행

```bash
cd ~/Walker_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash

# 실로봇 launch
sudo ros2 launch p73_controller realrobot.launch.py
```

콘솔에서 확인할 것:
```
[p73_cc] Mode started (is_on_robot=1)   ← 1이어야 함
[p73_cc] Inferred policy_obs_dim=235     ← ONNX 정상 로드
```

---

## STEP 7. GUI (별도 터미널)

```bash
source /opt/ros/jazzy/setup.bash
source ~/Walker_ws/install/setup.bash
ros2 launch p73_walker_gui gui.launch.py
```

GUI 순서:
1. **Torque On** → 모터 활성화
2. **Init Pos** → Send → 초기 자세 (이때 로봇 내려놓기)
3. **Init Yaw** → Send
4. **State Estimate** → Send
5. **Task Mode 7** → Send → ONNX policy 시작

---

## STEP 8. Teleop (별도 터미널)

```bash
source /opt/ros/jazzy/setup.bash
source ~/Walker_ws/install/setup.bash
python3 ~/Walker_ws/src/p73_cc/scripts/walker_teleop.py
```

| 키 | 동작 |
|---|---|
| W/S | 전진/후진 (vx ±0.1) |
| A/D | 좌/우 이동 (vy ±0.1) |
| Q/E | 좌/우 회전 (wz ±0.1) |
| Space | 정지 (전부 0) |
| Ctrl+C | 종료 |

---

## STEP 9. 비상 정지

- GUI에서 **Torque Off** 또는 **Task Mode 0** → Send
- 로봇 터미널에서 `Ctrl+C`
- E-stop 물리 버튼

---

## 문제 발생 시 되돌리기

```bash
# 백업에서 복구
rm -rf ~/Walker_ws/src/p73_cc
mv ~/Walker_ws/src/p73_cc.bak ~/Walker_ws/src/p73_cc

# 재빌드
cd ~/Walker_ws
colcon build --packages-select p73_cc p73_controller --cmake-args -DCMAKE_BUILD_TYPE=Release
touch src/p73_walker_controller/p73_controller/src/*.cpp
colcon build --packages-select p73_controller --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash
```

---

## 체크리스트

- [ ] `grep is_on_robot_ include/cc.h` → `true`
- [ ] policy.onnx shape 확인 (235 or 470)
- [ ] p73_controller 빌드 ~1분 (0.x초면 캐시됨, touch 후 재빌드)
- [ ] `source install/setup.bash` 했는지
- [ ] 콘솔에 `is_on_robot=1` 확인
