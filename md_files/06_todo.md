# TODO: Walker_ws 완성을 위한 작업 목록

## 현재 상태 요약

| 구성 요소 | 상태 | 비고 |
|-----------|------|------|
| 로봇 모델 (URDF) | 완료 | 13 DOF, 14 mesh |
| MuJoCo 모델 (XML) | 완료 | p73_walker.xml |
| 역학 라이브러리 (p73_lib) | 완료 | WBC, 4-bar, QP |
| 컨트롤러 노드 | 완료 | 5-thread 실시간 구조 |
| MuJoCo-ROS2 브리지 | 완료 | mjc_ros2 |
| GUI | 완료 | RQt 기반 |
| 하드웨어 드라이버 | 완료 | br_driver (별도 repo) |
| ONNX Policy 실행 (walker_cc) | 완료 | tocabi_cc 기반, IsaacLab 매칭 |
| **ONNX 파일** | **없음** | **학습 후 배치 필요** |
| USD 모델 | 완료 | IsaacLab 학습용 |

---

## 핵심 TODO

### 1. ONNX Policy 실행 패키지 구현 (최우선)

tocabi의 `tocabi_cc` 패키지에 해당하는 ROS2 패키지 생성 필요.

**구현 항목**:

#### a) ONNX Runtime 통합
```
- ONNX Runtime C++ API 설치 및 CMake 연동
- Policy 파일 로딩 (loadNetwork)
- Forward pass 추론 (feedforwardPolicy)
```

#### b) 관측값 계산 (computeObservation)
tocabi 48차원을 P73 13 DOF에 맞게 수정:
```
Base linear velocity (3)
Base angular velocity (3)
Projected gravity (3)
Velocity command (3)
Joint positions relative to initial (13)  ← tocabi는 12(legs)
Joint velocities (13)                     ← tocabi는 33
Last action (13)                          ← tocabi는 33
```
→ 정확한 차원은 IsaacLab 학습 설정에서 확인 필요

#### c) Action 처리 파이프라인
```cpp
// tocabi와 동일한 순서 유지
Δq = a_raw × action_scale;     // scale first
Δq = clamp(Δq, -clip, clip);   // then clip
q_des = q_init + Δq;
τ = (Kp/scale_p)(q_des - q) - (Kd/scale_d) * q̇;
τ = clamp(τ, -τ_max, +τ_max);
```

#### d) 히스토리 버퍼
- 관측값 히스토리 관리 (순환 버퍼)
- history_length는 IsaacLab 학습 설정과 일치 필요

#### e) 컨트롤러 모드 연동
- p73_controller에 ONNX 모드 추가 (tocabi의 Mode 7에 해당)
- GUI에 모드 버튼 추가
- 모드 전환 시 spline transition (100ms)

### 2. ONNX 파일 배치

```
src/p73_walker_controller/p73_controller/policy/
└── policy.onnx   ← IsaacLab에서 학습 후 export
```

- IsaacLab에서 P73 Walker 모델(USD)로 학습
- 학습된 policy를 ONNX로 export
- 입출력 차원 확인 및 문서화

### 3. IsaacLab 학습 설정 문서화

IsaacLab에서 학습할 때 사용한 설정을 기록 필요:
- observation 구성 및 차원
- action 구성 및 차원
- action scale, clip 값
- history length
- PD 게인 스케일링
- 정규화 방식
- reward 함수

### 4. 빌드 검증

```bash
# br_driver 설치 후 전체 빌드 테스트
cd /home/piene/Walker_ws
colcon build --symlink-install 2>&1 | tee build.log
```

---

## 선택적 TODO

### 5. rqt_multiplot 설정
- tocabi에서 사용하는 15개 플롯 설정을 Walker용으로 적응
- 관절 위치/속도/토크 모니터링

### 6. 디버그 도구
- tocabi의 `debug_tools/` 디렉토리 참조
- IsaacLab-MuJoCo 인터페이스 검증 스크립트

### 7. 실제 로봇 테스트 준비
- Xenomai 커널 설치/설정
- br_ecat, br_imu 데몬 설정
- 안전 한계 검증
- 긴급 정지 절차 수립

---

## 참고: tocabi_cc → Walker 포팅 체크리스트

| tocabi_cc 항목 | Walker 수정 사항 |
|----------------|-----------------|
| MODEL_DOF=33 | MODEL_DOF=13 |
| action_num_=12 | action_num_=13 (확인 필요) |
| state_current_num_=48 | 재계산 필요 |
| history_length_=10 | IsaacLab 설정 확인 |
| policy_obs_dim_=480 | state_num * history 재계산 |
| Kp/9, Kd/3 스케일링 | IsaacLab 설정 확인 |
| torque_limits_ (33) | P73 모터 사양에 맞게 수정 |
| q_limit_upper/lower | P73 관절 한계에 맞게 수정 |
| ONNX input "obs" | 차원 확인 및 매칭 |
| ONNX output "actions" | 13차원 확인 |
