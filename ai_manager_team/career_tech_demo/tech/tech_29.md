# 29. tcmalloc 주요 특징과 튜닝

작성자: 안명달 (mooondal@gmail.com)

## 1. 개요

tcmalloc(Thread Caching Malloc)은 **스레드별(thread-local) 할당 캐시**를 중심으로 설계된 메모리 할당기이다.
멀티스레드 서버/DB 환경에서 `malloc/free`가 빈번할 때, 기본 할당기 대비 성능을 끌어올릴 수 있다.
서버의 주요 시스템은 메모리풀을 사용하되, 소소한 힙 할당도 성능 향상을 위하여 tcmalloc을 사용한다.

---

## 2. MySQL에 tcmalloc 설정 방법

`mysqld_safe`의 `malloc-lib`..
그런데, Ubuntu용 MySQL 8.4 버전부터 tcmalloc이 기본 탑재되어 별도 설정이 필요 없다

---

## 3. C++ 프로젝트에 tcmalloc 설정 방법

- 리눅스: 링크 옵션에 `-ltcmalloc_minimal` 또는 `-ltcmalloc`을 추가
- 윈도우: `.lib`를 링크하고, 필요 시 `.dll` 배포

데모 프로젝트에는, Release(Non-Unreal) 구성에서 tcmalloc을 적용했다:

client/Client/Source/Common/Public/Common/PCH/pch_common.h

#if !defined(__UNREAL__) && !defined(_DEBUG)
#	pragma comment(lib, "libtcmalloc_minimal")
#endif
```

## 4. tcmalloc 메모리 크기 설정/튜닝 방법

tcmalloc의 핵심 성능은 **thread별 캐시 크기**에 의해 좌우된다. gperftools/tcmalloc 소스 코드에서 정의된 주요 상수는 다음과 같다.

#### 주요 상수 (src/thread_cache.h, src/common.h)

```cpp
// src/thread_cache.h (gperftools/tcmalloc)

// 1. 스레드당 최대 캐시 크기
static const size_t kMaxThreadCacheSize = 4 << 20;  // 4MB
// - 각 스레드가 보유할 수 있는 최대 캐시 메모리
// - 이 값을 넘으면 Central Free List로 반환

// 2. 전역 스레드 캐시 총량 기본값
static const size_t kDefaultOverallThreadCacheSize = 8u * kMaxThreadCacheSize;  // 32MB
// - 모든 스레드의 캐시 총합 상한
// - 스레드 수가 많을 경우 개별 스레드 캐시가 줄어들 수 있음

// 3. 스레드 캐시 최소 크기
static const size_t kMinThreadCacheSize = kMaxSize * 2;  // 약 512KB
// - 스레드 캐시가 줄어들 수 있는 최소 크기
// - 너무 작으면 Central Free List 접근 빈도 증가

// 4. 개별 Size Class당 최대 객체 수
static const int kMaxDynamicFreeListLength = 8192;
// - 특정 크기의 객체를 thread cache에 최대 몇 개까지 보관할지
// - 작은 객체(예: 16바이트)는 이 개수만큼 캐싱 가능
```

#### 환경 변수를 통한 런타임 튜닝

tcmalloc은 컴파일 타임 상수 외에도 **환경 변수**로 동적 조정이 가능한다:

```bash
# 1. 전역 스레드 캐시 총량 (기본: 32MB)
export TCMALLOC_MAX_TOTAL_THREAD_CACHE_BYTES=67108864  # 64MB

# 2. 샘플링 간격 (메모리 프로파일링 용, 기본: 512KB)
export TCMALLOC_SAMPLE_PARAMETER=524288  # 512KB마다 샘플링

# 3. 메모리 릴리즈 빈도 (기본: 1.0초)
export TCMALLOC_RELEASE_RATE=5.0  # 5초마다 OS로 메모리 반환
```
