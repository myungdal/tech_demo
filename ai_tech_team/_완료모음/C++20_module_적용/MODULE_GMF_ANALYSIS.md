# Global Module Fragment (GMF) 분석 및 최적화 가이드

## 현재 상황 분석

### 문제점

현재 모든 .ixx 파일이 `pch_packet.h`를 GMF에 포함하고 있습니다.
이는 **모듈의 효과를 크게 약화**시킵니다.

```cpp
// 현재 방식 (문제 있음)
module;
#include "Packet/PCH/pch_packet.h"  // 수백 개의 헤더가 끌려옴!
export module ServerEngine:Worker;
```

### 파티션별 GMF 헤더 현황

| 파티션 | GMF 헤더 수 | 위험도 |
|--------|-------------|--------|
| ServerEngine.ixx | 2 (pch_packet + pch_serverEngine) | 높음 |
| :DB | 13 | 높음 |
| :Util | 15 | 높음 |
| :Worker | 10 | 중간 |
| :Thread | 5 | 중간 |
| :Network | 5 | 중간 |
| :Socket | 5 | 중간 |
| :AppBase | 6 | 중간 |
| :Lock | 3 | 낮음 |
| :Log | 2 | 낮음 |
| :Exception | 1 | 낮음 |
| :Http | 1 | 낮음 |
| :Clock | 1 | 낮음 |
| :PacketTransactor | 2 | 낮음 |
| :Core | 0 | 없음 |

### pch_packet.h가 포함하는 것들 (예상)

```
- <windows.h>
- <vector>, <map>, <string>, <memory> 등 STL
- 프로젝트 전역 타입 정의
- 매크로들
- 기타 공통 헤더들
```

이 모든 것이 **모듈 경계를 넘어 전파**되어 모듈화의 장점이 사라집니다.

---

## 최적화 전략

### 전략 1: 점진적 GMF 축소 (권장)

**1단계**: 현재 방식으로 빌드 가능 여부 확인
**2단계**: 파티션별로 실제 필요한 헤더만 남기기
**3단계**: 공통 헤더는 별도 모듈로 분리

### 전략 2: 파티션별 GMF 최소화 예시

#### :Exception (가장 쉬움)

```cpp
// Before
module;
#include "Packet/PCH/pch_packet.h"
#include "ServerEngine/Exception/Exception.h"
export module ServerEngine:Exception;

// After
module;
#include <exception>
#include <string>
export module ServerEngine:Exception;

export class Exception : public std::exception {
    // 구현
};
```

#### :Clock (쉬움)

```cpp
// Before
module;
#include "Packet/PCH/pch_packet.h"
#include "ServerEngine/Clock/TlsClock.h"
export module ServerEngine:Clock;

// After
module;
#include <cstdint>
#include <chrono>
export module ServerEngine:Clock;

export class TlsClock {
    // 구현
};
```

#### :Lock (쉬움)

```cpp
// Before
module;
#include "Packet/PCH/pch_packet.h"
#include "ServerEngine/Lock/Lock.h"
...
export module ServerEngine:Lock;

// After
module;
#include <atomic>
#include <shared_mutex>
export module ServerEngine:Lock;

export class Lock { ... };
export class ReadLock { ... };
export class WriteLock { ... };
```

---

## GMF 최소화 우선순위

### 1순위 (쉬움, 효과 큼)

| 파티션 | 필요한 표준 헤더 | 예상 시간 |
|--------|-----------------|-----------|
| :Exception | `<exception>`, `<string>` | 15분 |
| :Clock | `<cstdint>`, `<chrono>` | 15분 |
| :Lock | `<atomic>`, `<shared_mutex>` | 20분 |

### 2순위 (중간 난이도)

| 파티션 | 의존성 | 예상 시간 |
|--------|--------|-----------|
| :Log | LogQueue만 export | 30분 |
| :Http | HttpClient만 export | 30분 |
| :PacketTransactor | 베이스 클래스만 | 30분 |

### 3순위 (복잡함)

| 파티션 | 복잡도 이유 | 예상 시간 |
|--------|-------------|-----------|
| :Worker | 많은 클래스, 템플릿 | 1시간 |
| :Network | Windows API 의존 | 1시간 |
| :Socket | Network 의존 | 1시간 |
| :Thread | Windows API 의존 | 1시간 |
| :DB | ODBC API 의존 | 1.5시간 |
| :Util | 많은 유틸리티 | 1.5시간 |
| :AppBase | 다른 파티션 의존 | 1시간 |

---

## 매크로 분석

### 모듈 외부로 나가야 하는 매크로 (헤더 유지)

```cpp
// 이런 매크로들은 import로 전파 안 됨 → 헤더 유지
#define _DEBUG_LOG(...)  
#define _ASSERT(x)
#define _ASSERT_CRASH(x)
#define OUT
#define IN
```

### constexpr/inline으로 변환 가능

```cpp
// Before (매크로)
#define POOL_SIZE (1 << 14)
#define POOL_SIZE_MASK (POOL_SIZE - 1)
#define _YIELD_SPIN std::this_thread::yield()

// After (모듈 호환)
export constexpr size_t POOL_SIZE = (1 << 14);
export constexpr size_t POOL_SIZE_MASK = POOL_SIZE - 1;
export inline void YieldSpin() { std::this_thread::yield(); }
```

### 로그 매크로 대체 방안

```cpp
// Before
#define _DEBUG_LOG(color, format, ...) \
    gLogQueue->Push(color, __FUNCTIONW__, __LINE__, format, ##__VA_ARGS__)

// After (C++20 source_location 사용)
export inline void DebugLog(
    LogColor color,
    const wchar_t* format,
    std::source_location loc = std::source_location::current()
) {
    gLogQueue->Push(color, loc.function_name(), loc.line(), format);
}
```

---

## 권장 작업 순서

### Phase 1: 현재 방식으로 빌드 테스트 (2-4시간)
- MODULE_STEP_BY_STEP.md 따라 진행
- GMF에 기존 헤더 포함한 상태로 빌드
- 서버 실행 확인

### Phase 2: GMF 최소화 (선택, 4-8시간)
- 1순위 파티션부터 GMF 정리
- 파티션별 커밋
- 빌드 테스트

### Phase 3: 매크로 정리 (선택, 2-4시간)
- 매크로 → constexpr/inline 변환
- 필수 매크로는 별도 헤더로 분리
- Macros.h 또는 Debug.h 유지

---

## 결론

### 현재 상태의 한계
- GMF에 PCH 포함으로 모듈 효과 약화
- 매크로가 import로 전파 안 됨
- 인터페이스 변경 시 전체 재빌드 위험

### 권장 접근법
1. **일단 현재 방식으로 빌드 테스트** (동작 확인)
2. **점진적으로 GMF 최소화** (성능 향상)
3. **매크로 정리** (완전한 모듈화)

### 예상 최종 효과
- GMF 최소화 후: 빌드 시간 30-50% 단축
- 매크로 정리 후: 깔끔한 모듈 인터페이스

