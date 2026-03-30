# p73_sys_ws-p73_lower 분석 및 필요성 판단

## 결론: 필요함

**이유**: Walker_ws의 `p73_controller`와 `mjc_ros2` 모두 `br_driver`에 의존성이 있음.
이 패키지가 없으면 빌드 자체가 불가능함.

---

## 개요

| 항목 | 내용 |
|------|------|
| 위치 | `/home/piene/Github_Code/p73_sys_ws-p73_lower` |
| 역할 | P73 Walker 로봇의 하드웨어 추상화 계층 |
| ROS 버전 | ROS2 (ament_cmake) |
| 핵심 기능 | EtherCAT 모터 제어, IMU 센서, 공유 메모리 IPC |

---

## 구성 패키지

### 1. br_driver (로봇 제어 라이브러리) - **핵심 의존성**

- Walker_ws에서 `p73_controller`, `p73_lib`, `mjc_ros2`가 모두 참조
- C++ API: `Robot` 클래스 (get_data(), send_command())
- Python bindings: pybind11 기반
- 공유 메모리 통신 (POSIX IPC, lock-free SPSC 링 버퍼)

**주요 헤더**:
- `br_driver.hpp`: 메인 API
- `ecat_data.h`: EtherCAT 데이터 구조 (관절 상태/명령)
- `imu_data.h`: IMU 데이터 구조
- `param.h`: 로봇 파라미터 (관절명, 모터 사양, 기어비, 한계)
- `shm.h`: 공유 메모리 API

**제어 모드**: IDLE, POSITION_CONTROL, VELOCITY_CONTROL, TORQUE_CONTROL

### 2. br_ecat (EtherCAT 마스터 데몬)

- 4kHz 실시간 제어 루프 (Xenomai)
- 29개 Elmo Gold 드라이브 제어
- **실제 로봇에서만 필요** (시뮬레이션에서는 불필요)

### 3. br_imu (IMU 센서 데몬)

- 2kHz IMU 데이터 수집 (MicroStrain GV7)
- **실제 로봇에서만 필요**

### 4. br_monitor (모니터링 도구)

- ncurses 기반 실시간 관절 상태 모니터
- 디버깅/진단용

---

## 빌드 모드

### 시뮬레이션 모드 (Xenomai 없이)
- br_driver 라이브러리만 빌드
- Walker_ws 시뮬레이션에 충분
- 실시간 커널 불필요

### 실제 로봇 모드 (Xenomai 필요)
- 전체 시스템 빌드 (br_ecat, br_imu 포함)
- Xenomai 패치 커널 필요
- RTnet 실시간 네트워킹

---

## 하드웨어 사양

| 항목 | 값 |
|------|-----|
| 관절 수 | 13 (+ 3 junction = 29 축) |
| EtherCAT 주기 | 4kHz (250μs) |
| IMU 샘플링 | 2kHz |
| 엔코더 해상도 | 76,000 CPR |
| 기어비 | 50:1 (표준), 80:1 (상체) |
| 명령 타임아웃 | 0.1s (안전 장치) |

---

## 설치 방법

```bash
cd /home/piene/Github_Code/p73_sys_ws-p73_lower

# 서브모듈 초기화
git submodule update --init --recursive

# 빌드 (시뮬레이션 모드 - Xenomai 없이)
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# 시스템 설치
sudo make install
# → /usr/local/lib/ (라이브러리)
# → /usr/local/include/br_driver/ (헤더)
```

**주의**: Walker_ws를 빌드하기 전에 br_driver가 시스템에 설치되어 있어야 함.

---

## Walker_ws와의 연결 포인트

```
p73_sys_ws (br_driver)
    ↕ 공유 메모리 (/p73_ecat_shm, /p73_imu_shm)
Walker_ws
    ├── p73_controller: br_driver로 관절 데이터 읽기/쓰기
    ├── p73_lib: br_driver 데이터 타입 참조
    └── mjc_ros2: br_driver 공유 메모리로 시뮬레이션 데이터 교환
```
