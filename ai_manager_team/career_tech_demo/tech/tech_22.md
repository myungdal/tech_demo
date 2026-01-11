# 22. 다양한 목적별 랜덤 생성 클래스 고도화

작성자: 안명달 (mooondal@gmail.com)

> **목차로 돌아가기**: [tech.md](tech.md)

---

## 개요

목적별 랜덤 생성기를 제공하여 게임 로직의 다양한 요구사항을 충족한다.

| 클래스 | 용도 | 핵심 특징 |
|--------|------|----------|
| **Mt19937Random** | 일반 랜덤 | 고품질, 하드웨어 엔트로피 시드 |
| **DeterministicRandom** | 재현 가능 랜덤 | 시드 기반, 디버깅/리플레이 |
| **RatioTable** | 가중치 랜덤 | 비율 기반 아이템 선택 |
| **NonReplacementSampler** | 비복원 랜덤 | 한 번 뽑은 아이템 제거하며 확률 관리 |
| **AdaptiveRandom** | 적응형 확률 | 실패 횟수에 따라 확률 증가 |

---

## 1. Mt19937Random - Mersenne Twister 고품질 랜덤

### 개요

**표준 라이브러리 `std::mt19937`** 기반의 고품질 랜덤 생성기이다.

### 핵심 특징

```cpp
class Mt19937Random32
{
private:
    std::mt19937 mRandomEngine;  // Mersenne Twister 엔진
    
public:
    Mt19937Random32()
    {
        // 하드웨어 엔트로피로 시드 초기화
        std::random_device rd;
        mRandomEngine.seed(rd());
    }
    
    uint32_t operator()();               // 랜덤 값 (전체 범위)
    uint32_t operator()(uint32_t range); // 0 ~ range-1
    int32_t operator()(int32_t min, int32_t max); // min ~ max-1
};
```

### 사용 예시

```cpp
Mt19937Random32 rng;

// 주사위 굴리기 (1-6)
int dice = rng(1, 7);  // 1 ~ 6

// 크리티컬 확률 (10%)
bool isCritical = (rng(100) < 10);

// 랜덤 아이템 인덱스
int itemIndex = rng(itemList.size());
```

### 특징

- **고품질**: Mersenne Twister 알고리즘 (주기 2^19937-1)
- **하드웨어 엔트로피**: `std::random_device`로 시드 초기화
- **균등 분포**: `std::uniform_int_distribution` 사용
- **비결정적**: 매 실행마다 다른 시퀀스

---

## 2. DeterministicRandom - 재현 가능한 랜덤

### 개요

**선형 합동 생성기(LCG)** 기반의 결정론적 랜덤 생성기로, **같은 시드는 항상 같은 시퀀스**를 생성한다.

### 핵심 알고리즘

```cpp
class DeterministicRandom32
{
private:
    static constexpr uint32_t MULTIPLIER = 1103515245u;  // LCG 곱셈 상수
    static constexpr uint32_t INCREMENT = 12345u;        // LCG 증가 상수
    uint32_t mSeed;
    
public:
    explicit DeterministicRandom32(uint32_t seed) : mSeed(seed) {}
    
    uint32_t operator()()
    {
        // 선형 합동 생성기(LCG): seed = seed * multiplier + increment
        mSeed = mSeed * MULTIPLIER + INCREMENT;
        return mSeed;
    }
    
    uint32_t operator()(uint32_t range)
    {
        if (range == 0) return 0;
        
        // 모듈로 편향 제거
        const uint32_t threshold = static_cast<uint32_t>(-static_cast<int32_t>(range));
        uint32_t result;
        do
        {
            result = (*this)();
        } while (result >= threshold);
        
        return result % range;
    }
};
```

### 64비트 버전

```cpp
class DeterministicRandom64
{
private:
    static constexpr uint64_t MULTIPLIER = 6364136223846793005ull;
    static constexpr uint64_t INCREMENT = 1442695040888963407ull;
    uint64_t mSeed;
    
    // 동일한 LCG 알고리즘, 64비트 범위
};
```

### 사용 예시

```cpp
// 같은 시드로 재현 가능
DeterministicRandom32 rng1(12345);
DeterministicRandom32 rng2(12345);

int a = rng1(100);  // 예: 42
int b = rng2(100);  // 항상 42 (동일)

// 디버깅: 특정 상황 재현
DeterministicRandom64 debugRng(savedSeed);
ReplayBattle(debugRng);
```

### 특징

- **재현 가능**: 같은 시드 -> 같은 시퀀스
- **빠름**: LCG는 매우 빠른 연산
- **디버깅 친화**: 버그 재현, 리플레이 구현
- **모듈로 편향 제거**: threshold 방식으로 공정한 분포
- **보안 부적합**: 예측 가능 (암호화 용도 X)

---

## 3. RatioTable - 가중치 기반 복원 추출

### 개요

**비율(가중치)에 따라 아이템을 랜덤 선택**하는 테이블로, **복원 추출**(같은 아이템 반복 선택 가능)이다.

### 핵심 알고리즘

```cpp
template<typename _ItemType>
class RatioTable
{
private:
    using ItemRatioPair = std::pair<_ItemType, size_t>;
    std::vector<ItemRatioPair> mItemRatioList;  // 아이템-비율 쌍
    size_t mTotalRatio = 0;                      // 전체 비율 합
    
public:
    void Add(const _ItemType& item, size_t ratio)
    {
        mItemRatioList.push_back({item, ratio});
        mTotalRatio += ratio;
    }
    
    template<class _Urng>
    bool Pick(OUT _ItemType& item, _Urng&& rng) const
    {
        // 1 ~ mTotalRatio 범위에서 랜덤 선택
        const size_t randValue = 1 + rng(mTotalRatio);
        
        size_t sum = 0;
        for (const auto& [itm, ratio] : mItemRatioList)
        {
            sum += ratio;
            if (randValue <= sum)
            {
                item = itm;
                return true;
            }
        }
        return false;
    }
};
```

### 사용 예시

```cpp
DeterministicRandom64 rng(12345);
RatioTable<int> itemTable;

// 아이템 등급별 확률 설정
itemTable.Add(1, 10);  // 일반: 10%
itemTable.Add(2, 20);  // 레어: 20%
itemTable.Add(3, 30);  // 에픽: 30%
itemTable.Add(4, 40);  // 레전더리: 40%

// 10,000번 뽑기 시뮬레이션
std::map<int, int> counts;
for (int i = 0; i < 10000; ++i)
{
    int grade;
    itemTable.Pick(OUT grade, rng);
    counts[grade]++;
}

// 결과: 1(10%), 2(20%), 3(30%), 4(40%) 비율 유지
```


### 특징

- **가중치 기반**: 비율에 따라 확률 조정
- **복원 추출**: 같은 아이템 반복 선택 가능
- **유연한 확률**: 비율 합이 100일 필요 없음 (상대 비율만 중요)
- **템플릿**: 모든 타입 지원 (int, enum, struct 등)
- ⏱️ **O(N)**: 아이템 개수에 비례 (선형 탐색)

---

## 4. NonReplacementSampler - 비복원 추출

### 개요

**한 번 선택된 아이템은 제거**되는 비복원 추출 샘플러이다. 비율이 최소값 이하로 떨어지면 **자동 재생성**된다.

### 핵심 알고리즘

```cpp
template<typename _ItemType>
class NonReplaceSampler
{
private:
    using ItemRatioPair = std::pair<_ItemType, size_t>;
    std::vector<ItemRatioPair> mItemRatioListSrc;  // 원본 (재생성용)
    std::vector<ItemRatioPair> mItemRatioList;     // 현재 (선택 시 감소)
    size_t mTotalRatio = 0;                         // 현재 전체 비율
    size_t mMinTotalRatio = 0;                      // 최소 임계값
    
public:
    explicit NonReplaceSampler(size_t minTotalRatio) 
        : mMinTotalRatio(minTotalRatio) {}
    
    void Add(const _ItemType& item, size_t ratio)
    {
        mItemRatioListSrc.push_back({item, ratio});
    }
    
    void Clear()
    {
        // 원본으로 복원
        mItemRatioList = mItemRatioListSrc;
        mTotalRatio = 0;
        for (const auto& [_, ratio] : mItemRatioListSrc)
            mTotalRatio += ratio;
    }
    
    template<class _Urng>
    bool Pick(OUT _ItemType& item, _Urng&& rng)
    {
        // 비율이 최소값 이하로 떨어지면 자동 재생성
        if (mTotalRatio <= mMinTotalRatio)
            Clear();
        
        if (mTotalRatio == 0)
            return false;
        
        const size_t randValue = 1 + rng(mTotalRatio);
        
        size_t sum = 0;
        for (auto& [itm, ratio] : mItemRatioList)
        {
            sum += ratio;
            if (randValue <= sum)
            {
                item = itm;
                
                // 선택된 아이템의 비율 감소 (비복원)
                if (ratio > 0)
                {
                    --ratio;
                    --mTotalRatio;
                }
                
                return true;
            }
        }
        return false;
    }
};
```

### 사용 예시

```cpp
DeterministicRandom64 rng(12345);

// 최소 비율 50 이하로 떨어지면 재생성
NonReplaceSampler<int> sampler(50);

sampler.Add(1, 10);  // 아이템 1: 10개
sampler.Add(2, 20);  // 아이템 2: 20개
sampler.Add(3, 30);  // 아이템 3: 30개
sampler.Add(4, 40);  // 아이템 4: 40개
sampler.Clear();     // 초기화 (전체 비율: 100)

// 50번 추출 (전체 100개)
for (int i = 0; i < 50; ++i)
{
    int item;
    sampler.Pick(OUT item, rng);
    // 선택된 아이템의 비율이 1 감소
}

// 전체 비율: 100 - 50 = 50 (임계값 도달)
// 다음 Pick() 호출 시 자동 재생성 -> 다시 100
```

### 비복원 과정

```
초기 상태:
아이템:    1    2    3    4
비율:     10   20   30   40
전체: 100

1번 선택 후 (아이템 3 선택):
아이템:    1    2    3    4
비율:     10   20   29   40
전체: 99

50번 선택 후:
전체: 50 (임계값 도달)

51번 선택 시:
-> 자동 재생성 -> 전체: 100 (원본 복원)
```

### 특징

- **비복원**: 한 번 선택된 아이템의 비율 감소
- **자동 재생성**: 임계값 이하로 떨어지면 원본 복원
- **직렬화 지원**: 게임 저장/로드 가능
- **유연한 임계값**: 재생성 시점 조절 가능
- **사용 시나리오**: 가챠 천장 시스템, 보상 박스

---

## 5. AdaptiveRandom - 적응형 확률

### 개요

**실패 횟수에 따라 확률이 동적으로 증가**하는 적응형 랜덤 생성기이다. **"천장 시스템"** 구현에 최적화되어 있다.

### 핵심 알고리즘

```cpp
class AdaptiveRandom
{
private:
    size_t mThreshold = 0;        // 임계값 (이 값 이하는 시작 확률)
    size_t mRamp = 1;             // 램프 단계 수 (확률 변화 단계)
    size_t mStartProb = 0;        // 시작 확률 [0, SCALE]
    size_t mEndProb = 1000000;    // 최종 확률 [0, SCALE]
    
    static constexpr size_t SCALE = 1000000;  // 확률 해상도 (100만 = 0.0001%)
    
public:
    AdaptiveRandom(size_t threshold, size_t ramp, size_t startProb, size_t endProb);
    
    size_t GetCurrentProbability(size_t failCount) const
    {
        // 임계값 이하는 시작 확률
        if (failCount <= mThreshold)
            return mStartProb;
        
        // 선형 보간: start에서 end로 mRamp 단계에 걸쳐 변화
        const size_t over = failCount - mThreshold;
        const size_t t = std::min(over, mRamp);
        
        const ptrdiff_t diff = static_cast<ptrdiff_t>(mEndProb) - static_cast<ptrdiff_t>(mStartProb);
        const ptrdiff_t result = static_cast<ptrdiff_t>(mStartProb) + (static_cast<ptrdiff_t>(t) * diff) / static_cast<ptrdiff_t>(mRamp);
        
        return static_cast<size_t>(std::clamp(result, 0LL, static_cast<ptrdiff_t>(SCALE)));
    }
    
    template<class _Urng>
    bool Try(size_t failCount, _Urng& rng) const
    {
        size_t p = GetCurrentProbability(failCount);
        p = std::clamp(p, 0ULL, SCALE);
        
        const size_t hit = rng(SCALE);  // 0 ~ SCALE-1
        return hit < p;
    }
};
```

### 사용 예시

```cpp
Mt19937Random32 rng;

// 가챠 천장 시스템
// threshold=5, ramp=10, startProb=0.1 (10%), endProb=0.9 (90%)
AdaptiveRandom gacha(5, 10, 100000, 900000);

int failCount = 0;
while (true)
{
    double prob = gacha.GetCurrentProbability(failCount) / 10000.0;
    std::cout << "시도 " << failCount << ": 확률 " << prob << "%" << std::endl;
    
    if (gacha.Try(failCount, rng))
    {
        std::cout << "성공! (총 " << failCount << "번 실패 후)" << std::endl;
        break;
    }
    
    ++failCount;
}
```

### 확률 증가 동작

| 구간 | 확률 | 설명 |
|------|------|------|
| 0~5회 | 10% | threshold 구간 (고정) |
| 6~15회 | 10% → 90% | ramp 구간 (선형 증가) |
| 16회~ | 90% | endProb 도달 (고정) |

### 실전 시나리오: 레전더리 아이템 가챠

```cpp
// 기본 확률 1%, 100번 실패 후 100% (천장)
AdaptiveRandom legendaryGacha(
    0,        // threshold: 0 (즉시 증가 시작)
    100,      // ramp: 100번에 걸쳐 증가
    10000,    // startProb: 1%
    1000000   // endProb: 100%
);

int attempts = 0;
while (true)
{
    double prob = legendaryGacha.GetCurrentProbability(attempts) / 10000.0;
    
    if (legendaryGacha.Try(attempts, rng))
    {
        AwardLegendaryItem();
        break;
    }
    
    ++attempts;
    
    // 100번 시도 시 100% 확률 보장 (천장)
}
```

### 특징

- **실패 보상**: 실패할수록 확률 증가
- **선형 보간**: 부드러운 확률 변화
- **천장 시스템**: 최대 시도 횟수 제한
- **유연한 설정**: threshold, ramp로 커브 조정
- **오버플로우 방지**: ptrdiff_t로 안전한 계산
- **해상도**: 100만 (0.0001% 단위)

---

## 성능 비교

| 클래스 | 초기화 | 랜덤 생성 | 메모리 | 특징 |
|--------|--------|-----------|--------|------|
| **Mt19937Random** | O(1) | O(1) | 5KB | 고품질, 하드웨어 엔트로피 |
| **DeterministicRandom** | O(1) | O(1) | 4B/8B | 재현 가능, LCG |
| **RatioTable** | O(N) | O(N) | O(N) | 가중치 복원 추출 |
| **NonReplacementSampler** | O(N) | O(N) | O(2N) | 비복원 추출 |
| **AdaptiveRandom** | O(1) | O(1) | 32B | 동적 확률 조정 |

---

## 실전 시나리오

### 시나리오 1: 몬스터 드롭 아이템

**요구사항:**
- 아이템 등급별 확률 (일반 50%, 레어 30%, 에픽 15%, 레전더리 5%)
- 같은 아이템 반복 드롭 가능 (복원)

**해결:**
```cpp
RatioTable<ItemGrade> dropTable;
dropTable.Add(ItemGrade::NORMAL, 50);
dropTable.Add(ItemGrade::RARE, 30);
dropTable.Add(ItemGrade::EPIC, 15);
dropTable.Add(ItemGrade::LEGENDARY, 5);

Mt19937Random32 rng;
ItemGrade grade;
dropTable.Pick(OUT grade, rng);
```

### 시나리오 2: 가챠 천장 시스템

**요구사항:**
- 기본 확률 1%
- 90번 실패 후 100% 보장 (천장)
- 10번까지는 1% 유지, 그 이후 90번에 걸쳐 증가

**해결:**
```cpp
AdaptiveRandom gachaSystem(
    10,       // threshold: 10번까지 1%
    90,       // ramp: 90번에 걸쳐 증가
    10000,    // startProb: 1%
    1000000   // endProb: 100%
);

int failCount = 0;
while (true)
{
    if (gachaSystem.Try(failCount, rng))
    {
        AwardSSRCharacter();
        break;
    }
    ++failCount;
    // 최대 100번 (10 + 90) 시도 시 100% 보장
}
```

### 시나리오 3: 보상 박스 (10개 중 5개 선택)

**요구사항:**
- 10개 아이템 중 5개 랜덤 선택
- 중복 선택 불가 (비복원)
- 5개 선택 후 자동 재생성

**해결:**
```cpp
NonReplaceSampler<int> rewardBox(50);  // 5개 선택 후 재생성 (100 - 50 = 50)

for (int i = 1; i <= 10; ++i)
    rewardBox.Add(i, 10);  // 각 아이템 비율 10 (총 100)

rewardBox.Clear();

for (int i = 0; i < 5; ++i)
{
    int item;
    rewardBox.Pick(OUT item, rng);
    AwardItem(item);
}
// 5개 선택 후 전체 비율 50 (임계값 도달)
// 다음 Pick() 시 자동 재생성
```

### 시나리오 4: 전투 리플레이

**요구사항:**
- 전투 시뮬레이션 재현
- 같은 시드로 같은 결과
- 디버깅 용이

**해결:**
```cpp
// 전투 기록 시
DeterministicRandom64 battleRng(savedSeed);
BattleResult result = SimulateBattle(battleRng);
SaveReplay(savedSeed, result);

// 리플레이 재생 시
DeterministicRandom64 replayRng(savedSeed);
BattleResult replayResult = SimulateBattle(replayRng);
// result == replayResult (완벽히 동일)
```

---

## 테스트 커버리지

### RatioTable 테스트

```cpp
// 가중치 비례 검증
RatioTable<int> table;
table.Add(1, 10);  // 10%
table.Add(2, 20);  // 20%
table.Add(3, 30);  // 30%
table.Add(4, 40);  // 40%

// 10,000번 샘플링 -> 각 비율 ±5% 오차 내 검증
```

### DeterministicRandom 테스트

```cpp
// 재현성 검증
DeterministicRandom32 rng1(12345);
DeterministicRandom32 rng2(12345);

for (int i = 0; i < 1000; ++i)
{
    assert(rng1() == rng2());  // 100% 일치
}
```

### AdaptiveRandom 테스트

```cpp
// 확률 증가 검증
AdaptiveRandom adaptive(5, 10, 100000, 900000);

assert(adaptive.GetCurrentProbability(0) == 100000);   // 10%
assert(adaptive.GetCurrentProbability(5) == 100000);   // 10% (threshold)
assert(adaptive.GetCurrentProbability(10) == 500000);  // 50% (중간)
assert(adaptive.GetCurrentProbability(15) == 900000);  // 90% (end)
```

---

## 장점

| 장점 | 설명 |
|------|------|
| **목적별 최적화** | 각 상황에 맞는 전문화된 랜덤 생성기 |
| **통일된 인터페이스** | `operator()` 연산자로 일관된 사용법 |
| **템플릿 기반** | 모든 타입 지원 (int, enum, struct 등) |
| **재현 가능** | DeterministicRandom으로 디버깅/리플레이 |
| **천장 시스템** | AdaptiveRandom으로 실패 보상 |
| **비복원 추출** | NonReplacementSampler로 중복 방지 |
| **모듈로 편향 제거** | threshold 방식으로 공정한 분포 |
| **오버플로우 방지** | ptrdiff_t, uint64_t로 안전한 계산 |

---

## 통합 사용 예시

```cpp
class LootSystem
{
private:
    Mt19937Random32 mMainRng;                    // 일반 랜덤
    DeterministicRandom64 mReplayRng;           // 리플레이용
    RatioTable<ItemGrade> mGradeTable;          // 등급 확률
    NonReplaceSampler<int> mRewardBox;          // 비복원 보상
    AdaptiveRandom mLegendaryGacha;             // 천장 시스템
    
public:
    LootSystem()
        : mRewardBox(50)
        , mLegendaryGacha(10, 90, 10000, 1000000)
    {
        // 등급 확률 설정
        mGradeTable.Add(ItemGrade::NORMAL, 50);
        mGradeTable.Add(ItemGrade::RARE, 30);
        mGradeTable.Add(ItemGrade::EPIC, 15);
        mGradeTable.Add(ItemGrade::LEGENDARY, 5);
    }
    
    // 몬스터 드롭 (복원)
    ItemGrade RollMonsterDrop()
    {
        ItemGrade grade;
        mGradeTable.Pick(OUT grade, mMainRng);
        return grade;
    }
    
    // 보상 박스 (비복원, 5개 선택)
    std::vector<int> OpenRewardBox()
    {
        std::vector<int> rewards;
        for (int i = 0; i < 5; ++i)
        {
            int item;
            mRewardBox.Pick(OUT item, mMainRng);
            rewards.push_back(item);
        }
        return rewards;
    }
    
    // 가챠 (천장 시스템)
    bool TryLegendaryGacha(int failCount)
    {
        return mLegendaryGacha.Try(failCount, mMainRng);
    }
    
    // 전투 리플레이
    BattleResult ReplayBattle(uint64_t seed)
    {
        mReplayRng.SetSeed(seed);
        return SimulateBattle(mReplayRng);
    }
};
```

---

[목차로 돌아가기](tech.md)
