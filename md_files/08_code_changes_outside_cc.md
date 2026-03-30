# p73_walker_cc 외부 코드 수정 사항 정리

> p73_walker_cc는 개인 코드이므로 제외. 아래는 연구실 공용 코드 수정분.

---

## 1. `state_estimator.cpp` — control_time_us_ 미설정 버그

**파일**: `p73_walker_controller/p73_controller/src/state_estimator.cpp`
**위치**: `StoreState()` 함수 (line ~710)

### 문제
`control_time_` (초 단위)는 `rd_global_`에 저장하지만, `control_time_us_` (마이크로초 단위)는 **한 번도 설정하지 않음**.
`RobotEigenData`의 `control_time_us_`는 `std::atomic<int64_t>`로 선언되어 있고 초기값은 0.

`p73_walker_cc`의 모든 타이밍 로직이 `control_time_us_`를 사용:
- spline 전환 시간 판단 (`control_time_us_ < start_time_ + 0.1초`)
- policy 업데이트 주기 판단 (`control_time_us_ - time_inference_pre_`)

`control_time_us_`가 항상 0이면:
- `start_time_` = 0
- spline 조건 `0 < 0 + 100000` → **항상 true** → 영원히 spline 모드
- spline의 t=0 → 항상 `torque_init_` 출력 → **RL 토크가 절대 적용 안 됨**

### 수정
```cpp
// 변경 전:
rd_global_.control_time_ = control_time_;

// 변경 후:
rd_global_.control_time_ = control_time_;
rd_global_.control_time_us_ = static_cast<int64_t>(control_time_ * 1e6);
```

### 근거
- `control_time_`은 이미 정상적으로 설정되고 있음 (rcv_cnt / 1000.0)
- `control_time_us_`는 같은 값의 마이크로초 변환일 뿐
- TOCABI(catkin_ws)에서는 `rd_gl_.control_time_us_ = dur_start_`로 설정함
- 이 수정 없이는 cc 모듈이 **어떤 코드를 넣어도** 정상 동작 불가

---

## 2. `p73_controller.cpp` — cc_init_ 리셋 누락

**파일**: `p73_walker_controller/p73_controller/src/p73_controller.cpp`
**위치**: `taskCmdCallback()` 함수 (line ~552)

### 문제
`CustomController::cc_init_`는 mode 5-9 최초 진입 시 `false`로 설정.
이후 다른 모드(0-4, position control 등)로 갔다가 다시 mode 5-9로 돌아오면 `cc_init_`이 `false`인 채로 남아 **재초기화가 안 됨**.

결과:
- 이전 세션의 `start_time_`, `rl_action_`, `policy_hist_` 등이 그대로 남음
- observation history가 리셋 안 되어 policy에 잘못된 과거 데이터 입력
- 토크 spline이 재시작 안 되어 갑작스러운 토크 변화 가능

### 수정
```cpp
void P73Controller::taskCmdCallback(const p73_msgs::msg::TaskCmd::SharedPtr msg)
{
    dc_.pc_mode = false;
    dc_.tc_mode = true;
    dc_.ik_mode = false;
    dc_.task_cmd_ = *msg;
#ifdef COMPILE_P73_CC
    cc_.cc_init_ = true;  // 모드 전환 시 CC 재초기화 보장
#endif
    cout << "CNTRL : task signal received mode :" << dc_.task_cmd_.task_mode << endl;
    stm_.StatusPub("CNTRL : task Control mode : %d", dc_.task_cmd_.task_mode);
}
```

### 근거
- TOCABI(catkin_ws)에서는 `rd_cc_.tc_init` 플래그를 사용하며, task mode 전환 시 항상 `true`로 리셋
- mode 전환 없이 같은 mode를 다시 보내는 경우에도 안전하게 재초기화됨
- `#ifdef COMPILE_P73_CC` 가드로 cc 미사용 시 영향 없음

---

## 요약

| 수정 | 파일 | 영향 | 없으면? |
|---|---|---|---|
| control_time_us_ 설정 | state_estimator.cpp | cc 타이밍 전체 | RL 토크 절대 적용 안 됨 |
| cc_init_ 리셋 | p73_controller.cpp | mode 재진입 | 재초기화 불가, 잘못된 상태 |

두 수정 모두 기존 동작(mode 0-4, position control, gravity comp)에 **영향 없음**.
cc를 사용하지 않는 경우에도 안전.

---

> 작성일: 2026-03-30
