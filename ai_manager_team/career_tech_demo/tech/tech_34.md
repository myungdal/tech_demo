# 34. 메모리 풀 시스템 - 용도별 최적화된 3종 메모리 풀

작성자: 안명달 (mooondal@gmail.com)

## 개요

게임 서버의 고빈도 객체 할당/해제를 최적화하기 위해 용도별로 특화된 3종 메모리 풀을 제공한다. 각 풀은 사용 패턴에 따라 동적 확장, Lock-free 멀티스레드, 캐시 친화적 연속 메모리 중 하나에 최적화되어 있다.

| 풀 타입 | 특징 | 스레드 모델 | 사용처 |
|---------|------|-------------|--------|
| **DynamicPool** | 동적 확장 (1.5배), 상한 없음 | 단일 스레드 | WorkerTask, 가변 크기 객체 |
| **FixedPool** | 고정 크기 (2^N), Lock-free | 스레드 안전 | Socket, SendBuffer 등 고빈도 멀티스레드 객체 |
| **CompactionPool** | 연속 메모리, 캐시 친화적, 키 기반 | 단일 스레드 | 게임 엔티티, 시뮬레이션 객체 |

---

## DynamicPool - 동적 확장 풀

**단일 스레드** 환경에서 사용하며, 풀 크기가 부족하면 **자동으로 1.5배 확장**된다.

### 특징
- **동적 확장**: 초기 크기 부족 시 1.5배씩 자동 확장
- **상한 설정 가능**: `_MaxCapacity`로 최대 용량 제한 가능
- **단일 스레드**: Lock이 없어 오버헤드 최소화
- **용도**: Worker Task, 임시 객체 등

### 사용 예시

```cpp
// 단일 스레드 환경, 동적 확장
DynamicPool<WorkerTask, 1024> taskPool;

// 할당
WorkerTask* task = taskPool.Pop();  // 풀에서 가져오기
if (task == nullptr) {
    // 풀 초과 시 1.5배 자동 확장
}

// 반환
taskPool.Push(task);
```

### 장점
- **메모리 효율**: 초기 크기를 작게 시작 가능
- **유연성**: 부하 증가 시 자동 확장
- **성능**: Lock 없음

---

## FixedPool - Lock-free 멀티스레드 풀

**멀티스레드** 환경에서 사용하며, **Lock-free 알고리즘**으로 스레드 안전성을 보장한다.

### 특징
- **고정 크기**: 2^N 크기로 고정 (예: 2^10 = 1024개)
- **Lock-free**: `memory_order_acquire/release`로 안전한 핸드오프
- **멀티스레드 안전**: atomic 연산으로 경합 최소화
- **용도**: Socket, SendBuffer 등 고빈도 멀티스레드 객체

### 사용 예시

```cpp
// 멀티스레드 환경, Lock-free
FixedPool<Socket, 10> socketPool;  // 2^10 = 1024개

// 할당 (atomic exchange)
Socket* socket = socketPool.Pop();
if (socket == nullptr) {
    // 풀 초과 시 동적 할당 (new)
}

// 반환 (atomic CAS)
socketPool.Push(socket);
// 슬롯이 점유되어 있으면 delete 후 버림
```

### 동작 원리

```cpp
// 간략화된 구현 개념
template<typename T, size_t N>
class FixedPool {
    std::atomic<T*> mSlots[1 << N];  // 2^N 슬롯
    std::atomic<size_t> mIndex{0};   // 순환 인덱스
    
    T* Pop() {
        size_t idx = mIndex.fetch_add(1, memory_order_relaxed) & ((1 << N) - 1);
        return mSlots[idx].exchange(nullptr, memory_order_acquire);
    }
    
    void Push(T* ptr) {
        size_t idx = mIndex.fetch_add(1, memory_order_relaxed) & ((1 << N) - 1);
        T* expected = nullptr;
        if (!mSlots[idx].compare_exchange_strong(expected, ptr, memory_order_release)) {
            delete ptr;  // 슬롯 점유 시 해제
        }
    }
};
```

### 장점
- **Lock-free**: 스레드 블로킹 없음
- **확장성**: 멀티코어 환경에서 높은 처리량
- **안정성**: 토큰 기반 ABA 문제 방지

---

## CompactionPool - 캐시 친화적 연속 메모리 풀

**단일 스레드** 환경에서 사용하며, **연속 메모리 레이아웃**으로 캐시 효율이 좋다.

### 특징
- **연속 메모리**: 모든 객체가 연속된 메모리에 배치
- **키 기반 접근**: 키(인덱스)로 객체 참조
- **캐시 친화적**: 순회 시 캐시 적중률 향상
- **직렬화 지원**: 스냅샷 저장/복원 가능
- **용도**: 게임 엔티티, 시뮬레이션 객체

### 사용 예시

```cpp
// 캐시 적중률 최적화, 연속 메모리
CompactionPool<Entity, int16_t, 256> entityPool;

// 할당 (키 반환)
auto key = entityPool.Pop();           // 키 반환 (예: 5)

// 접근
Entity& entity = entityPool.GetData(key);
entity.position = {100, 200};

// 반환 (키로 해제)
entityPool.Push(key);
```

### 동작 원리

```cpp
// 간략화된 구현 개념
template<typename T, typename KeyType, size_t N>
class CompactionPool {
    T mData[N];                  // 연속 메모리
    KeyType mFreeList[N];        // 빈 슬롯 목록
    size_t mFreeCount = N;       // 빈 슬롯 개수
    
    KeyType Pop() {
        if (mFreeCount == 0) return INVALID_KEY;
        return mFreeList[--mFreeCount];
    }
    
    void Push(KeyType key) {
        mFreeList[mFreeCount++] = key;
    }
    
    T& GetData(KeyType key) {
        return mData[key];
    }
};
```

### 장점
- **캐시 효율**: 순회 시 캐시 미스 최소화
- **컴포넌트 기반 적합**: 컴포넌트 기반 아키텍처에 최적
- **직렬화 용이**: 연속 메모리로 덤프/로드 간편
- **메모리 압축**: 삭제 후 메모리 단편화 없음

---

## 성능 비교

| 항목 | DynamicPool | FixedPool | CompactionPool |
|------|-------------|-----------|----------------|
| **할당 속도** | O(1) | O(1) atomic | O(1) |
| **해제 속도** | O(1) | O(1) atomic | O(1) |
| **순회 속도** | 보통 | 보통 | **매우 빠름** (캐시 친화) |
| **멀티스레드** | X | O (Lock-free) | X |
| **메모리 효율** | 높음 (동적) | 보통 (고정) | 매우 높음 (연속) |
| **확장성** | 자동 1.5배 | 고정 | 고정 |

---

## 실전 시나리오

### 시나리오 1: 고빈도 패킷 처리 (FixedPool)

```cpp
// Socket 풀 (멀티스레드 환경)
FixedPool<Socket, 12> gSocketPool;  // 2^12 = 4096개

void AcceptCallback(SOCKET s) {
    Socket* socket = gSocketPool.Pop();
    if (socket == nullptr) {
        socket = new Socket();  // 풀 초과 시 동적 할당
    }
    socket->Attach(s);
    // ... 패킷 처리
}

void OnSocketClose(Socket* socket) {
    socket->Close();
    gSocketPool.Push(socket);  // 풀로 반환
}
```

### 시나리오 2: 게임 엔티티 관리 (CompactionPool)

```cpp
// 엔티티 풀 (단일 스레드, 캐시 친화)
CompactionPool<Entity, EntityId, 1024> gEntityPool;

EntityId SpawnEntity() {
    auto id = gEntityPool.Pop();
    Entity& entity = gEntityPool.GetData(id);
    entity.Initialize();
    return id;
}

void UpdateAllEntities() {
    // 연속 메모리 순회 -> 캐시 적중률 향상
    for (size_t i = 0; i < gEntityPool.GetUsedCount(); ++i) {
        Entity& entity = gEntityPool.GetDataByIndex(i);
        entity.Update();
    }
}
```

### 시나리오 3: Worker Task 풀 (DynamicPool)

```cpp
// Worker Task 풀 (단일 스레드, 동적 확장)
DynamicPool<WorkerTask, 256> gTaskPool;

void EnqueueTask(Worker* worker, Func func) {
    WorkerTask* task = gTaskPool.Pop();
    if (task == nullptr) {
        // 풀 자동 확장 (256 -> 384 -> 576 ...)
    }
    task->Set(func);
    worker->Enqueue(task);
}

void OnTaskComplete(WorkerTask* task) {
    task->Reset();
    gTaskPool.Push(task);  // 풀로 반환
}
```

---

## 구현 시 고려사항

### 1. 풀 크기 선정
- **DynamicPool**: 초기 크기를 평균 사용량의 80%로 설정
- **FixedPool**: 최대 동시 사용량 + 여유분 20%
- **CompactionPool**: 최대 엔티티 개수로 고정

### 2. 메모리 정렬
- 모든 풀은 `alignof(T)` 정렬 보장
- SIMD 사용 시 16바이트 정렬 필요

### 3. 디버깅
- UsageMeter로 사용량 모니터링
- 풀 고갈 시 로그 출력
- 릴리스 빌드에서는 디버깅 코드 제거

### 4. 스레드 모델 주의
- **DynamicPool/CompactionPool**: 단일 스레드 전용
- **FixedPool**: 멀티스레드 안전

---

## 유틸리티: UsageMeter - 메모리 풀 사용량 측정

메모리 풀의 사용량을 실시간으로 추적하는 모니터링 시스템이다.

### 개요

```cpp
// 타입별 사용량 추적 (메모리 풀, 소켓 등)
UsageMeter<FixedPool, Socket>::OnIncrease();  // 할당 시
UsageMeter<FixedPool, Socket>::OnDecrease();  // 해제 시
```

### 특징

- **템플릿 기반 타입별 통계**: 각 풀 타입과 객체 타입별로 독립적인 통계 관리
- **Meyer's Singleton 패턴**: 전역 맵으로 모든 UsageMeter 관리
- **런타임 모니터링**: 현재 사용량, 최대 사용량, 제한값 추적
- **디버깅 지원**: 메모리 누수 탐지, 풀 고갈 경고

### 사용 예시

```cpp
// FixedPool에서 Socket 할당 시
Socket* socket = gSocketPool.Pop();
if (socket != nullptr) {
    UsageMeter<FixedPool, Socket>::OnIncrease();
}

// 반환 시
gSocketPool.Push(socket);
UsageMeter<FixedPool, Socket>::OnDecrease();

// 사용량 조회
auto& meter = UsageMeter<FixedPool, Socket>::GetInstance();
printf("Current: %d, Max: %d, Limit: %d\n", 
    meter.GetCurrent(), meter.GetMax(), meter.GetLimit());
```

### 구조

```cpp
template<typename PoolType, typename ObjectType>
class UsageMeter {
    std::atomic<int32_t> mCurrent{0};  // 현재 사용량
    std::atomic<int32_t> mMax{0};      // 최대 사용량
    int32_t mLimit{0};                 // 제한값
    
public:
    static UsageMeter& GetInstance() {
        static UsageMeter instance;
        return instance;
    }
    
    static void OnIncrease() {
        auto& meter = GetInstance();
        int32_t current = meter.mCurrent.fetch_add(1) + 1;
        
        // 최대값 갱신
        int32_t max = meter.mMax.load();
        while (current > max) {
            if (meter.mMax.compare_exchange_weak(max, current)) {
                break;
            }
        }
        
        // 제한값 초과 경고
        if (meter.mLimit > 0 && current > meter.mLimit) {
            LOG_WARN("UsageMeter exceeded: %s/%s = %d/%d",
                typeid(PoolType).name(), typeid(ObjectType).name(),
                current, meter.mLimit);
        }
    }
    
    static void OnDecrease() {
        GetInstance().mCurrent.fetch_sub(1);
    }
};
```

### 활용 사례

**1. 메모리 누수 탐지**
```cpp
// 서버 종료 시 사용량 확인
void OnServerShutdown() {
    auto& socketMeter = UsageMeter<FixedPool, Socket>::GetInstance();
    if (socketMeter.GetCurrent() > 0) {
        LOG_ERROR("Socket leak detected: %d sockets not released",
            socketMeter.GetCurrent());
    }
}
```

**2. 풀 크기 조정**
```cpp
// 운영 중 최대 사용량 확인 후 풀 크기 조정
void AnalyzePoolUsage() {
    auto& meter = UsageMeter<FixedPool, Socket>::GetInstance();
    printf("Socket pool max usage: %d\n", meter.GetMax());
    // 최대 사용량이 풀 크기의 80%를 초과하면 확장 고려
}
```

**3. 실시간 모니터링**
```cpp
// MCP를 통한 서버 모니터링
void HandleMemCommand() {
    printf("=== Memory Pool Usage ===\n");
    printf("Socket: %d/%d\n",
        UsageMeter<FixedPool, Socket>::GetCurrent(),
        UsageMeter<FixedPool, Socket>::GetMax());
    printf("WorkerTask: %d/%d\n",
        UsageMeter<DynamicPool, WorkerTask>::GetCurrent(),
        UsageMeter<DynamicPool, WorkerTask>::GetMax());
}
```

---

## 관련 기술

- [6. 고성능 비동기 함수 호출 시스템](tech_06.md) - WorkerTask에서 DynamicPool 사용
- [4. 클라이언트-서버 통합 동기화 엔진](tech_04.md) - 게임 엔티티에서 CompactionPool 사용
- [2. ShellMcpManager - MCP를 활용한 자연어 기반 서버 모니터링](tech_02.md) - UsageMeter로 메모리 사용량 모니터링

---

