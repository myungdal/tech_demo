# C++20 Module 빌드 설정 가이드

## 개요

이 문서는 ServerEngine과 서버 프로젝트들을 C++20 Module로 전환하기 위한 빌드 설정 가이드입니다.

## 모듈 구조

```
ServerEngine (module) - 핵심 엔진
├─ :Core          - 기본 유틸리티
├─ :Worker        - Worker 시스템
├─ :Network       - 네트워크
├─ :Socket        - 소켓
├─ :Lock          - 동기화
├─ :Log           - 로깅
├─ :Thread        - 스레드
├─ :DB            - 데이터베이스
├─ :Clock         - 시간
├─ :Util          - 유틸리티
├─ :Exception     - 예외 처리
├─ :Http          - HTTP
├─ :AppBase       - 애플리케이션 기반
└─ :PacketTransactor - 패킷 트랜잭션

MainServer (module) → import ServerEngine
FrontServer (module) → import ServerEngine
... (기타 서버들)
```

## Visual Studio 2022 프로젝트 설정

### 1. ServerEngine 프로젝트 설정

#### 1-1. 프로젝트 속성 설정

```
프로젝트 우클릭 → 속성

[구성: 모든 구성, 플랫폼: x64]

1. C/C++ → 일반
   - C++ 언어 표준: ISO C++20 표준(/std:c++20)
   
2. C/C++ → 고급
   - "스캔 소스에서 모듈 종속성 검색": 예
   (주의: /interface는 프로젝트 전체에 적용하지 않음!)
```

#### 1-2. .ixx 파일별 개별 설정 (중요!)

**주의**: `/interface` 옵션은 .ixx 파일에만 적용해야 합니다!

```
각 .ixx 파일 우클릭 → 속성

1. 일반
   - 항목 형식: C/C++ 컴파일러

2. C/C++ → 고급
   - "모듈 인터페이스로 컴파일": 예 (/interface)
   - 모듈 출력 파일명: $(IntDir)%(Filename).ifc
```

**절대 하지 말아야 할 것**:
- 프로젝트 전체에 `/interface` 적용 → 모든 .cpp가 인터페이스로 처리됨!

### 2. 서버 프로젝트 설정

동일하게 C++20 활성화하고, 각 서버의 .ixx 파일에만 `/interface` 적용.

**main.cpp 수정** (각 서버 프로젝트):
```cpp
// Before
#include "pch.h"
#include "MainServer/App/MainServerApp.h"

// After
#include "pch.h"           // PCH는 반드시 맨 앞에 유지!
import ServerEngine;
import MainServer;
// 필요한 매크로만 include 유지
```

### 3. Test 프로젝트 설정

Test 프로젝트는 ServerEngine을 참조하므로 C++20 설정 필요:

```
프로젝트 속성:
- C/C++ → 일반 → C++ 언어 표준: /std:c++20
- C/C++ → 고급 → 스캔 소스에서 모듈 종속성 검색: 예

프로젝트 종속성:
- serverEngine 프로젝트에 종속성 설정

Test 코드 수정:
#include "pch.h"           // 1. PCH 맨 앞
import ServerEngine;       // 2. import는 PCH 다음
#include "Debug.h"         // 3. 매크로 헤더는 마지막
```

## PCH (Precompiled Header) 처리

### .ixx 파일에서 PCH 비활성화 (필수!)

모듈 인터페이스 파일(.ixx)은 **반드시 PCH를 비활성화**해야 합니다.

```
각 .ixx 파일 속성:
C/C++ → 일반 → 미리 컴파일된 헤더: 사용 안 함
```

**이유**:
- .ixx는 Global Module Fragment(GMF)를 사용하여 필요한 헤더 포함
- PCH와 GMF를 동시에 사용하면 충돌 발생

### GMF에서 필요한 헤더 포함

```cpp
// .ixx 파일 예시
module;

// GMF - 필요한 헤더만 포함 (PCH 대체)
#include <vector>
#include <memory>
#include <atomic>
// 또는 기존 PCH 포함 (단, 모듈 효과 감소)
#include "Packet/PCH/pch_packet.h"

export module ServerEngine:Worker;
```

### 일반 .cpp 파일은 PCH 유지

모듈을 import하는 일반 .cpp 파일은 **PCH를 그대로 사용**합니다.

```cpp
// main.cpp - import/include 순서
#include "pch.h"           // 1. PCH는 반드시 맨 앞!
import ServerEngine;       // 2. 모듈 import (PCH 다음)
import MainServer;
#include "Debug.h"         // 3. 매크로 필요한 헤더 (선택)
```

**순서 규칙**:
1. PCH가 있으면 **PCH가 맨 앞**
2. `import` 문은 PCH 다음
3. 매크로가 필요한 헤더는 마지막에 `#include`

## 중요 주의사항

### 1. 매크로는 모듈로 전파되지 않음!

**C++20 모듈의 핵심 특성**: 매크로는 `import`로 전파되지 않습니다.

```cpp
// 잘못된 예시 - 동작하지 않음!
export module MyModule;
export {
    #define MY_MACRO 123  // 이 매크로는 import하는 쪽에서 안 보임!
}
```

**해결 방법**:

| 매크로 유형 | 해결책 |
|-------------|--------|
| 상수 매크로 | `constexpr` 변수로 변환 |
| 함수형 매크로 | `inline` 함수로 변환 |
| 조건부 컴파일 | 헤더 파일 유지 (include 방식) |
| 로그 매크로 | 함수 + source_location으로 대체 |

```cpp
// 변환 예시
// Before (매크로)
#define MAX_BUFFER_SIZE 4096
#define SQUARE(x) ((x) * (x))

// After (모듈 호환)
export constexpr size_t MAX_BUFFER_SIZE = 4096;
export inline auto Square(auto x) { return x * x; }
```

**모듈 외부로 나가야 하는 매크로**:
- `_DEBUG_LOG`, `_ASSERT` 등 디버그 매크로 → 별도 헤더로 유지
- `_NEW`, `_DELETE` 등 → 헤더 유지 또는 함수로 변환

### 2. Global Module Fragment(GMF) 최소화

**현재 문제점**: GMF에 PCH 전체를 포함하면 모듈 효과가 크게 약화됩니다.

```cpp
// 문제가 있는 현재 방식
module;
#include "Packet/PCH/pch_packet.h"  // PCH 전체를 끌어옴!
#include "ServerEngine/PCH/pch_serverEngine.h"  // 더 많은 헤더!
export module ServerEngine;
```

**권장 방식**: GMF에는 최소한의 헤더만 포함

```cpp
// 개선된 방식
module;
// 표준 라이브러리만 포함
#include <memory>
#include <vector>
#include <atomic>

export module ServerEngine:Worker;

// 실제 구현은 모듈 내부에서
```

### 3. export 범위 최소화

**문제**: 인터페이스 변경 시 모든 의존 모듈 재빌드

**전략**:
- 외부에 필요한 API만 export
- 내부 구현은 모듈 내부에 숨기기
- 파티션을 활용하여 구현 분리

```cpp
// ServerEngine-Worker.ixx
export module ServerEngine:Worker;

export {
    // 외부에 필요한 것만!
    class Worker;
    class CoWorker;
}

// 내부 구현 클래스는 export하지 않음
class WorkerInternal;  // 이건 외부에서 안 보임
```

### 4. Thread Local 변수

모듈에서 `thread_local` 변수를 export하려면:

```cpp
export {
    inline thread_local int tThreadId = 0;  // inline 필수!
}
```

## 빌드 성능 향상

모듈화 후 기대 효과:

```
빌드 시간 단축: 30-50% 예상
- ServerEngine을 한 번만 컴파일
- 각 서버는 .ifc 파일만 참조
- 병렬 빌드 효율 증가

재빌드 시간 단축: 50-70% 예상
- 모듈 인터페이스 변경 없으면 재컴파일 불필요
- 헤더 변경에 따른 연쇄 재컴파일 방지
```

**단, GMF 최소화가 전제조건!**

## 현재 진행 상황

- [x] ServerEngine.ixx 생성
- [x] ServerEngine 파티션 모듈들 생성 (14개)
- [x] 각 서버 프로젝트 .ixx 생성 (8개)
- [x] 헤더 경로 검증 완료
- [ ] GMF 최소화 작업 (권장)
- [ ] 매크로 → constexpr/inline 변환 (권장)
- [ ] 프로젝트 설정 변경
- [ ] 컴파일 및 테스트

## 점진적 전환 전략 (권장)

### Phase A: 현재 방식으로 시작 (빠른 검증)
- GMF에 기존 헤더 포함 (현재 상태)
- 빌드 가능 여부 확인
- 서버 실행 테스트

### Phase B: GMF 최소화 (성능 최적화)
- 파티션별로 GMF 헤더 정리
- 필요 없는 헤더 제거
- 표준 라이브러리만 GMF에 유지

### Phase C: 매크로 정리
- 매크로 사용처 파악
- constexpr/inline 함수로 변환
- 불가피한 매크로는 헤더로 분리

## 참고 자료

- [C++20 Modules - Microsoft Docs](https://docs.microsoft.com/cpp/cpp/modules-cpp)
- [Module 파티션 - MSDN](https://docs.microsoft.com/cpp/cpp/module-partition)

## 문의사항

모듈 전환 중 문제가 발생하면:
1. 컴파일 오류 메시지 확인
2. .ifc 파일 생성 여부 확인 ($(IntDir) 폴더)
3. /interface가 .ixx에만 적용되었는지 확인
4. GMF 헤더 목록 점검
