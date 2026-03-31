# 실행 방법 가이드

## 사전 준비

### 1. 의존성 설치

#### 시스템 라이브러리
```bash
# Pinocchio (역기구학/동역학)
sudo apt install ros-<distro>-pinocchio

# OSQP + OsqpEigen (QP 솔버)
# OSQP: https://github.com/osqp/osqp
# OsqpEigen: https://github.com/robotology/osqp-eigen

# Eigen3
sudo apt install libeigen3-dev

# GLFW + OpenGL (MuJoCo 시각화)
sudo apt install libglfw3-dev libgl-dev

# Qt5 (GUI)
sudo apt install qtbase5-dev
```

#### br_driver 설치 (필수)
```bash
cd /home/piene/Github_Code/p73_sys_ws-p73_lower
git submodule update --init --recursive
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

### 2. Walker_ws 빌드

```bash
cd /home/piene/Walker_ws

# ROS2 환경 소싱
source /opt/ros/<distro>/setup.bash

# 빌드
colcon build --symlink-install

# 환경 소싱
source install/setup.bash
```

---

## 시뮬레이션 실행

```bash
# 1. ROS2 환경 + Walker_ws 소싱
source /opt/ros/<distro>/setup.bash
source /home/piene/Walker_ws/install/setup.bash

# 2. 시뮬레이션 launch 실행
ros2 launch p73_controller simulation.launch.py
```

이 launch 파일은 다음을 동시에 실행:
- **mjc_ros2**: MuJoCo 시뮬레이터 (p73_walker.xml 로드)
- **p73_controller**: 메인 컨트롤러 노드
- **p73_gui**: RQt GUI

---

## 실제 로봇 실행

```bash
# 사전 조건: Xenomai 커널, br_ecat/br_imu 데몬 실행 중

# 1. 공유 메모리 초기화 (이전 실행 잔여 데이터 제거)
ros2 run p73_controller shm_reset

# 2. 실제 로봇 launch 실행
sudo ros2 launch p73_controller realrobot.launch.py
```

---

## ONNX Policy 실행 (sim2sim 성공)

```bash
# 1. 시뮬레이션 실행
ros2 launch p73_controller simulation.launch.py

# 2. GUI에서 Task Mode 7 선택 → ONNX policy 자동 실행
#    policy.onnx 위치: ~/Walker_ws/src/p73_cc/policy/policy.onnx

# 3. 텔레오프 (별도 터미널)
python3 ~/Walker_ws/src/p73_cc/scripts/walker_teleop.py
#   w/s: 전후 | a/d: 좌우 | q/e: 회전 | space: 정지
```

빌드 주의: `p73_cc`는 정적 라이브러리이므로 `p73_controller`도 함께 빌드해야 반영됨:
```bash
colcon build --packages-select p73_cc p73_controller --cmake-args -DCMAKE_BUILD_TYPE=Release
```

sim2sim 디버깅 기록은 `12_sim2sim_debug_resolution.md` 참조.

---

## 주요 설정 파일

| 파일 | 용도 | 수정 시점 |
|------|------|----------|
| `setting_sim_PDgain.yaml` | 시뮬레이션 PD 게인 | 시뮬레이션 튜닝 시 |
| `setting_realrobot_PDgain.yaml` | 실제 로봇 PD 게인 | 실제 로봇 튜닝 시 |
| `mujoco/p73_walker.xml` | MuJoCo 물리 모델 | 시뮬레이션 파라미터 변경 시 |
| `urdf/p73_walker.urdf` | 로봇 기구학 모델 | 로봇 구조 변경 시 |

---

## 트러블슈팅

### 빌드 실패: br_driver not found
```
→ p73_sys_ws-p73_lower 먼저 빌드 및 설치
→ sudo make install 확인
→ /usr/local/lib/libbr_driver.so 존재 확인
```

### 공유 메모리 오류 (br_driver)
```bash
# System V 공유 메모리 정리
ros2 run p73_controller shm_reset
# 또는
ipcrm -M $(ipcs -m | grep "0x41" | awk '{print $2}')
```

### MuJoCo 창이 안 뜨거나 노드가 멈추는 경우

가장 흔한 원인은 **FastRTPS(DDS) 공유 메모리 오염**입니다.
이전 ROS2 실행이 비정상 종료되면 `/dev/shm/` 에 stale 파일이 남아
`rclcpp::Node` 생성 시 무한 블록됩니다.

```bash
# FastRTPS stale 공유 메모리 전부 삭제
rm -f /dev/shm/fastrtps_* /dev/shm/sem.fastrtps_*
```

삭제 후 다시 `ros2 launch p73_controller simulation.launch.py`를 실행하면
MuJoCo 창이 정상적으로 나타납니다.

### MuJoCo 시각화 안 됨
```
→ GLFW, OpenGL 설치 확인: sudo apt install libglfw3-dev libgl-dev
→ DISPLAY 환경변수 확인: echo $DISPLAY (보통 :0 또는 :1)
→ GPU 드라이버 확인: glxinfo | head -5
```
