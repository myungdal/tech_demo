# 30. TlsClock - Thread Local 시간 캐싱과 '시간 역전' 대응

작성자: 안명달 (mooondal@gmail.com)

## 1. 개요

시간을 다루는 방법이 코드베이스에 여러 가지 존재하면 **성능 손실과 수많은 착오/혼선**이 발생하는 경우가 많았다.

서버/클라의 메인 루프(핫패스)에서 `now()`를 자주 호출하면 **시스템 콜 + 변환 비용**이 누적되어 성능에 영향을 준다.
또한 "실제 시스템 시간(벽시계)"는 운영 환경에서 **되감기(시간 역전)**가 발생할 수 있어,
시간 기반 로직(타임아웃/쿨다운/정렬/로그/DB 타임스탬프)에 치명적인 버그를 만들 수 있다.

`TlsClock`은 스레드 루프에서 **1회 Update()로 시간을 갱신**하고,
그 루프 안에서는 **thread_local(TLS) 캐시된 시간**을 재사용하도록 만든 시간 유틸리티이다.

---

## 2. 한 루프에서 시간을 여러 번 구하는 비효율성

### 2.1 핫패스에서의 비용 누적
대표적으로 다음이 누적된다.
- `clock::now()` 자체의 비용(플랫폼에 따라 시스템 콜/커널 경유)
- `time_point -> time_t` 변환
- `time_t -> tm` 변환 (`gmtime`/`localtime` 계열)
- DB용 `TIMESTAMP_STRUCT` 같은 포맷 변환

루프/패킷 처리 핫패스에서 “로그 찍기/타임아웃 검사/스케줄/메트릭”마다 시간을 새로 구하면,
**같은 루프에서 시간 값이 미세하게 달라지는 것**까지 더해져 디버깅도 어려워집니다.

### 2.2 TlsClock이 하는 일(요약)
루프 시작에 `Update()`를 한 번 호출하고,
그 루프에서는 `GetLocalNow()`/`GetGlobalNow()` 등으로 **동일한 기준 시간**을 빠르게 얻는다.

이 프로젝트의 워커 스레드 루프에서 실제로 아래 패턴으로 사용하고 있다:

```26:45:server/serverEngine/ServerEngine/Thread/ThreadImpl/WorkerThread.cpp
while (true)
{
	TlsCommonBuffer::ResetTlsCommonBufferIdx();

	clock.Update();

	mIocp->DispatchCompletion(1);

	threadWorker->Run();

	gCoWorker->Run();
}
```

---

## 3. TlsClock 구조 (TLS 캐시)

### 3.1 TLS 저장소
`TlsClock`은 thread_local 저장소를 사용한다.

```46:58:server/serverEngine/ServerEngine/TLS/TlsGlobals.h
// 시간 관련 TLS
thread_local inline TlsClock tClock;
thread_local inline TlsClockInternalMembers tClockInternalMembers; // TlsClock 내부 접근 전용
```

### 3.2 Update()의 의미(Contract)
- 스레드 루프(또는 tick)에서 **주기적으로** 호출한다.
- Update 이후에는:
  - `ClockPoint`와 `ClockMs`는 즉시 사용 가능
  - `time_t`, `tm`, `TIMESTAMP_STRUCT`는 “요청 시점에 1회만” lazy 계산 후 캐시된다

이 lazy-cache 구조는 `TlsClock.cpp`에서 `mLocalNowTt/mLocalNowTm/...` 포인터가 `nullptr`인지로 판별한다.

### 3.3 LocalNow vs GlobalNow
`TlsClock`은 두 종류의 시간을 제공한다.
- **LocalNow**: 스레드가 루프에서 갱신한 기준 시간(현재 구현은 `CLOCK_NOW` 기반)
- **GlobalNow**: 네트워크 매니저가 제공하는 전역 시간(없으면 LocalNow로 폴백)

```19:31:server/serverEngine/ServerEngine/Clock/TlsClock.cpp
tClockInternalMembers.mLocalNow = CLOCK_NOW;
// ...
tClockInternalMembers.mGlobalNow = gNetworkManager ? gNetworkManager->GetGlobalNow() : tClockInternalMembers.mLocalNow;
```

> “GlobalNow”가 어떤 방식으로 동기화되는지(서버 시간 동기, NTP 보정, 보간 등)는 `NetworkManager` 구현 정책에 따릅니다.

---

## 4. 시간 역전(Time Reversal) 이야기

### 4.1 시간 역전이 실제로 생기는 이유
벽시계(실제 시스템 시간)는 "단조 증가(monotonic)"를 보장하지 않는다. 예:
- **NTP 보정**: 시간이 앞으로/뒤로 튀는 보정(step adjustment)
- **VM/컨테이너 환경**: 호스트 시간 변화, suspend/resume
- **관리자 수동 변경**: 운영 중 시계 수정
- **서머타임/타임존 설정 변경**
- (플랫폼에 따라) clock 소스 변경, 동기화 서비스 영향

### 4.2 시간 역전이 만드는 버그 시나리오(대표)
- **타임아웃이 끝나지 않음**: `deadline = now + X`인데 `now`가 뒤로 가서 영원히 도달하지 않음
- **쿨다운이 되돌아감**: 남은 시간이 갑자기 늘어남
- **정렬/로그 순서가 꼬임**: 같은 세션의 이벤트 시간이 역순으로 찍힘
- **“음수 duration” 발생**: `now - last`가 음수가 되어 언더플로/예외/잘못된 통계
- **DB timestamp 불일치**: 서버 내부 시간과 DB 기록 시간의 순서/정합이 깨짐

### 4.3 Clock 선택 기준 (핵심 원칙)
- **duration/timeout/스케줄링**: 단조 시간(예: `steady_clock`) 기반이 안전
- **로그/DB 타임스탬프/유저 표시 시간**: 벡시계 기반이 필요할 수 있음(단, 역전/보정 고려)

`TlsClock`의 설계 의도는 "루프 내 다중 now() 비용"을 줄이고,
"같은 루프에서의 시간 기준을 고정"해 로직/디버깅을 안정화하는 데 있다.
하지만 **시간 역전 자체를 완전히 제거**하려면, 사용하는 측에서 "어떤 용도로 어떤 clock을 쓸지"를 계약으로 정해야 한다.

### 4.4 TlsClock의 시간 소스 (코드 레벨)

#### 현재 구현: utc_clock (C++20)

본 프로젝트는 C++20의 `utc_clock`을 `system_clock`으로 변환하여 사용한다:

```cpp
// Common/Public/Common/PCH/pch_common.h
using ClockPoint = system_clock::time_point;

#define CLOCK_NOW clock_cast<system_clock>(utc_clock::now())
#define CLOCK_POINT_TO_TT(x) system_clock::to_time_t(x)
```

```cpp
// TlsClock.cpp - Update()
void TlsClock::Update()
{
    tClockInternalMembers.mLocalNow = CLOCK_NOW;  // utc_clock -> system_clock
    tClockInternalMembers.mLocalNowMs = CLOCK_MS_FROM_DR(tClockInternalMembers.mLocalNow.time_since_epoch());
    // ...
}
```

**utc_clock의 특징:**
- **윤초(Leap Second) 보정**: 국제 원자시(TAI) 기반으로 윤초를 자동 처리
- **시간대 독립**: UTC 기준이므로 타임존/서머타임 영향 없음
- **NTP 동기화 고려**: system_clock보다 안정적이지만 여전히 벽시계 기반

> **주의**: `utc_clock`도 여전히 "벽시계"이므로, 시스템 관리자가 시간을 수동으로 변경하거나 VM suspend/resume 시 시간 역전이 발생할 수 있다.

#### 시간 역전 방지: steady_clock 사용 권장

**타임아웃/쿨다운/duration 계산**에는 `steady_clock`을 사용해야 한다:

```cpp
// [비권장] 벽시계 기반 타임아웃 (시간 역전 취약)
ClockPoint deadline = tClock.GetLocalNow() + std::chrono::seconds(30);
if (tClock.GetLocalNow() >= deadline) {
    // 시간 역전 시 타임아웃이 영원히 발생하지 않을 수 있음!
}

// [권장] steady_clock 기반 타임아웃 (단조 증가 보장)
using steady_time_point = std::chrono::steady_clock::time_point;
steady_time_point deadline = std::chrono::steady_clock::now() + std::chrono::seconds(30);
if (std::chrono::steady_clock::now() >= deadline) {
    // 시간이 절대 뒤로 가지 않으므로 항상 도달함
}
```

**steady_clock의 보장:**
```cpp
// C++ 표준이 보장하는 속성
static_assert(std::chrono::steady_clock::is_steady);  // true
// - 단조 증가(monotonic): 시간이 절대 뒤로 가지 않음
// - NTP/시스템 시간 변경/VM suspend에 영향받지 않음
// - duration 계산에 안전
```

#### 실전 패턴: 용도별 clock 선택

| 용도 | 권장 Clock | 이유 |
|------|-----------|------|
| **타임아웃** | `steady_clock` | 단조 증가 보장, 시간 역전 없음 |
| **쿨다운/주기** | `steady_clock` | duration 계산 안전 |
| **로그 타임스탬프** | `TlsClock::GetLocalNowTt()` | DB 기록용, 사람이 읽을 수 있음 |
| **DB INSERT** | `TlsClock::GetGlobalNowTs()` | TIMESTAMP_STRUCT 포맷 |
| **유저 표시 시간** | `TlsClock::GetGlobalNowTm()` | 타임존 변환 가능 |
| **패킷 전송 시각** | `TlsClock::GetGlobalNow()` | 서버 간 시간 동기화 |

#### 예시: 타임아웃 체커 클래스 (steady_clock 기반)

```cpp
class TimeoutChecker
{
private:
    using steady_point = std::chrono::steady_clock::time_point;
    steady_point mDeadline;

public:
    // 타임아웃 설정 (duration 기반)
    void SetTimeout(std::chrono::milliseconds timeout)
    {
        mDeadline = std::chrono::steady_clock::now() + timeout;
    }

    // 타임아웃 체크 (시간 역전 영향 없음)
    bool IsTimeout() const
    {
        return std::chrono::steady_clock::now() >= mDeadline;
    }

    // 남은 시간 (음수 duration 발생 없음)
    std::chrono::milliseconds GetRemaining() const
    {
        auto now = std::chrono::steady_clock::now();
        if (now >= mDeadline) return std::chrono::milliseconds(0);
        return std::chrono::duration_cast<std::chrono::milliseconds>(mDeadline - now);
    }
};

// 사용 예시
TimeoutChecker checker;
checker.SetTimeout(std::chrono::seconds(30));

while (!checker.IsTimeout())
{
    // 타임아웃까지 작업 수행
    // 시스템 시간이 변경되어도 정확히 30초 후 타임아웃됨
}
```

#### 정리: TlsClock과 steady_clock의 역할 분담

| 항목 | TlsClock (utc_clock/system_clock) | steady_clock |
|------|-----------------------------------|--------------|
| **목적** | 로그/DB/유저 표시용 "실제 시간" | duration/타임아웃용 "경과 시간" |
| **시간 역전** | 가능 (NTP, 수동 변경 등) | **불가능** (단조 증가 보장) |
| **성능 최적화** | TLS 캐시로 루프당 1회 갱신 | 직접 호출 (캐시 불필요) |
| **사용 장소** | DB INSERT, 로그 기록, 패킷 타임스탬프 | 타임아웃, 쿨다운, 주기 체크 |
| **표준 보장** | `is_steady == false` | `is_steady == true` |

---

## 5. 실전 사용 가이드(권장)

### 5.1 이렇게 쓰면 좋다
- 핫패스 루프 시작에 `tClock.Update()` 1회
- 루프 내부에서:
  - 타임아웃/주기 계산은 “LocalNowMs” 같은 단일 기준으로 처리
  - DB 기록/외부 노출 시간이 필요하면 “GlobalNowTs” 같은 포맷을 사용

### 5.2 주의사항(Contract/Edge-case)
- 루프마다 `Update()`를 호출하지 않으면 "현재 시간"이 갱신되지 않는다.
- GlobalNow는 네트워크 매니저의 정책에 따라 역전/보정이 발생할 수 있다.
- 시간이 역전될 수 있는 경로(벽시계)를 duration 로직에 사용하면, 4.2의 버그가 그대로 재현될 수 있다.

