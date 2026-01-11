# 15. 하이브리드 랭킹 시스템 - 선형보간 기반 메모리 효율 랭킹

작성자: 안명달 (mooondal@gmail.com)

## 개요

대규모 MMORPG에서 수 많은 플레이어 랭킹을 제공할 때, 모든 플레이어의 정확한 순위를 저장하면 메모리와 계산량이 너무 크다. 
상위권은 정확한 순위**, 하위권은 선형보간으로 근사 순위%를 제공해도 된다는 가정으로, 메모리 사용량을 효율적으로 관리하고 사용자 경험을 유지하는 하이브리드 랭킹 시스템이다.

---

## 핵심 아이디어

### 기존 문제점
- **전체 플레이어 순위 저장**: 플레이어 수가 많아지면 메모리 부담이 커진다
- **순위 계산 비용**: 매번 정렬 또는 이진 탐색 필요
- **실시간 업데이트**: 경험치 획득 시마다 순위 재계산

### 하이브리드 랭킹 시스템의 해결책
1. **상위 1000위**: 정확한 순위 저장 (사용자 관심 집중 구간)
2. **1001위 이상**: **레벨별 선형보간**으로 순위% 근사 계산
3. **레벨별 보간 테이블**: 각 레벨의 최소/최대 경험치 보유자의 순위% 저장

---

## 시스템 구조

### 저장 데이터

```cpp
// 랭킹 서버 저장 데이터
struct RankingSystem
{
    // 1. 상위 1000위 정확한 순위
    std::vector<PlayerRankData> mTop1000;  // 1000개
    
    // 2. 레벨별 선형보간 테이블
    std::unordered_map<int, LevelBoundary> mLevelBoundaries;  // ~100개 (최대 레벨)
    
    // 3. 전체 플레이어 수
    int64_t mTotalPlayerCount;
};

// 상위 랭킹 데이터
struct PlayerRankData
{
    UserId userId;
    int level;
    int64_t exp;
    int rank;  // 1~1000
};

// 레벨별 경계 데이터
struct LevelBoundary
{
    int level;
    
    // 이 레벨에서 가장 경험치가 적은 플레이어
    int64_t minExp;
    float minExpRankPercent;  // 순위% (0~100)
    
    // 이 레벨에서 가장 경험치가 많은 플레이어
    int64_t maxExp;
    float maxExpRankPercent;  // 순위% (0~100)
};
```

### 메모리 사용량 비교

| 방식 | 데이터 크기 | 메모리 사용량 (1000만 명) |
|------|------------|--------------------------|
| **전체 순위 저장** | UserId(8) + Rank(4) = 12바이트 | 120 MB |
| **하이브리드 시스템** | Top1000(12KB) + LevelTable(10KB) | **22 KB** |

**예상 효과: 메모리 대폭 절감**

---

## 랭킹 조회 알고리즘

### Case 1: 상위 1000위 이내

```cpp
// GetPlayerRank - 플레이어 순위 조회
RankInfo GetPlayerRank(UserId userId, int level, int64_t exp)
{
    // 1. 상위 1000위 확인
    for (int rank = 0; rank < mTop1000.size(); ++rank)
    {
        if (mTop1000[rank].userId == userId)
        {
            return RankInfo{
                .rank = rank + 1,           // 정확한 순위 (1~1000)
                .rankPercent = (float)(rank + 1) / mTotalPlayerCount * 100.0f,
                .isTop1000 = true
            };
        }
    }
    
    // 2. 1000위 밖 - 선형보간 계산
    return GetRankByInterpolation(level, exp);
}
```

### Case 2: 1001위 이상 - 선형보간

```cpp
// GetRankByInterpolation - 레벨별 선형보간으로 순위% 계산
RankInfo GetRankByInterpolation(int level, int64_t exp)
{
    // 1. 해당 레벨의 경계 데이터 조회
    const LevelBoundary& boundary = mLevelBoundaries[level];
    
    // 2. 경험치가 범위 밖이면 경계값 사용
    if (exp <= boundary.minExp)
    {
        // 이 레벨에서 가장 낮은 경험치
        return RankInfo{
            .rank = -1,  // 정확한 순위 알 수 없음
            .rankPercent = boundary.minExpRankPercent,
            .isTop1000 = false
        };
    }
    
    if (exp >= boundary.maxExp)
    {
        // 이 레벨에서 가장 높은 경험치
        return RankInfo{
            .rank = -1,
            .rankPercent = boundary.maxExpRankPercent,
            .isTop1000 = false
        };
    }
    
    // 3. 선형보간으로 순위% 계산
    float expRatio = (float)(exp - boundary.minExp) / (boundary.maxExp - boundary.minExp);
    
    // [WHY] 경험치가 높을수록 순위%가 낮아짐 (상위권)
    float rankPercent = Lerp(
        boundary.maxExpRankPercent,  // 경험치 많음 -> 순위% 낮음 (상위)
        boundary.minExpRankPercent,  // 경험치 적음 -> 순위% 높음 (하위)
        expRatio
    );
    
    return RankInfo{
        .rank = -1,
        .rankPercent = rankPercent,
        .isTop1000 = false
    };
}

// 선형보간 함수
float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}
```

---

## 선형보간 예시

### 시나리오
- 전체 플레이어 수: 10,000,000명
- 레벨 50 플레이어 수: 100,000명

### 레벨 50 경계 데이터

```cpp
LevelBoundary level50 = {
    .level = 50,
    .minExp = 1000,         // 레벨 50 중 최소 경험치
    .minExpRankPercent = 85.0f,  // 하위 85% (8,500,000등)
    .maxExp = 50000,        // 레벨 50 중 최대 경험치
    .maxExpRankPercent = 15.0f   // 상위 15% (1,500,000등)
};
```

### 플레이어 A (레벨 50, 경험치 25,500)

```cpp
// 1. 경험치 비율 계산
expRatio = (25500 - 1000) / (50000 - 1000) = 24500 / 49000 = 0.5

// 2. 선형보간
rankPercent = Lerp(15.0, 85.0, 0.5) = 15.0 + (85.0 - 15.0) * 0.5 = 50.0%

// 결과: 상위 50% (5,000,000등 근처)
```

### 플레이어 B (레벨 50, 경험치 37,750)

```cpp
// 1. 경험치 비율 계산
expRatio = (37750 - 1000) / (50000 - 1000) = 36750 / 49000 = 0.75

// 2. 선형보간
rankPercent = Lerp(15.0, 85.0, 0.75) = 15.0 + (85.0 - 15.0) * 0.75 = 32.5%

// 결과: 상위 32.5% (3,250,000등 근처)
```

---

## 시각화

### 레벨 50 경험치-순위% 그래프

```
순위%
100% (10,000,000등)
 │
 │                                    ● minExp (1,000)
 │                                   ╱ minExpRankPercent: 85%
85% ─────────────────────────────────●
 │                                 ╱
 │                               ╱
 │                             ╱  선형보간 구간
 │                           ╱
50% ───────────────────────●  Player A (25,500 exp)
 │                       ╱
 │                     ╱
 │                   ╱
 │                 ╱
32.5% ───────────●  Player B (37,750 exp)
 │             ╱
 │           ╱
 │         ╱
15% ──────●────────────────────────────
 │       maxExp (50,000)
 │       maxExpRankPercent: 15%
 │
 0% (1등)
    └──────────────────────────────────-> 경험치
    1,000                        50,000
```

---

## 레벨별 경계 데이터 갱신

### 갱신 주기
- **주기적 갱신**: 10분마다 배치 작업으로 경계 데이터 재계산
- **이벤트 기반 갱신**: 레벨 최고 기록 갱신 시 즉시 업데이트

### 갱신 알고리즘

```cpp
// UpdateLevelBoundaries - 레벨별 경계 데이터 갱신
void UpdateLevelBoundaries()
{
    // 1. 모든 플레이어를 레벨별로 그룹화
    std::unordered_map<int, std::vector<PlayerData>> playersByLevel;
    
    for (const auto& player : mAllPlayers)
    {
        playersByLevel[player.level].push_back(player);
    }
    
    // 2. 각 레벨별로 경계 계산
    for (auto& [level, players] : playersByLevel)
    {
        // 경험치 기준 정렬
        std::sort(players.begin(), players.end(), [](auto& a, auto& b) {
            return a.exp > b.exp;  // 내림차순 (높은 경험치 -> 상위 순위)
        });
        
        // 최소/최대 경험치 플레이어 순위 계산
        int minExpPlayerIndex = players.size() - 1;  // 마지막 (최소 경험치)
        int maxExpPlayerIndex = 0;                   // 첫 번째 (최대 경험치)
        
        // 전체 순위 계산 (간단화: 이전 레벨까지의 플레이어 수 + 현재 인덱스)
        int minExpRank = CalculateGlobalRank(level, minExpPlayerIndex);
        int maxExpRank = CalculateGlobalRank(level, maxExpPlayerIndex);
        
        // 경계 데이터 저장
        mLevelBoundaries[level] = LevelBoundary{
            .level = level,
            .minExp = players[minExpPlayerIndex].exp,
            .minExpRankPercent = (float)minExpRank / mTotalPlayerCount * 100.0f,
            .maxExp = players[maxExpPlayerIndex].exp,
            .maxExpRankPercent = (float)maxExpRank / mTotalPlayerCount * 100.0f
        };
    }
}
```

---

## 상위 100위 목록 제공

### API 설계

```cpp
// GetTop100Players - 상위 100위 목록 조회
std::vector<PlayerRankData> GetTop100Players()
{
    std::vector<PlayerRankData> result;
    
    for (int i = 0; i < 100 && i < mTop1000.size(); ++i)
    {
        result.push_back(mTop1000[i]);
    }
    
    return result;
}
```

### 클라이언트 표시 예시

```
========================================
        [랭킹 TOP 100]
========================================
순위  | 닉네임          | 레벨 | 경험치
----------------------------------------
  1   | DragonSlayer    |  99  | 999,999
  2   | LegendKiller    |  99  | 998,888
  3   | ProGamer2024    |  99  | 997,777
  ...
 50   | CasualPlayer    |  85  | 750,000
  ...
100   | TopHundred      |  82  | 680,000
----------------------------------------

내 순위: 상위 32.5% (약 3,250,000등)
```

---

## 장점

| 장점 | 설명 |
|------|------|
| **메모리 효율** | 레벨별 경계 데이터만 저장하여 효율적 |
| **계산 효율** | 1000위 밖은 O(1) 선형보간만으로 순위% 계산 |
| **사용자 경험** | 상위권은 정확한 순위, 하위권은 "상위 50%" 같은 상대적 정보 제공 |
| **확장성** | 플레이어 수가 증가해도 메모리/계산량 거의 불변 |
| **실시간 업데이트** | 레벨별 경계만 주기적 갱신 (전체 정렬 불필요) |
| **심리적 효과** | 하위권은 "상위 50%"가 "5,000,000등"보다 긍정적 |

---

## 정확도 분석

### 오차 발생 구간
- **상위 1000위**: 오차 0% (정확한 순위)
- **1001~10000위**: 오차 ±5% (레벨 분포가 고르면 정확도 높음)
- **10001위 이상**: 오차 ±10% (하지만 사용자는 정확한 순위보다 %에 관심)

### 오차가 허용되는 이유
1. **사용자 관심**: 하위권 플레이어는 "5,234,567등"보다 "상위 52%"에 관심
2. **게임 디자인**: 보상은 상위 1000위 이내에만 집중
3. **심리적 효과**: "상위 50%"는 동기부여, "5,000,000등"은 좌절감

### 정확도 개선 방법
- 상위 10,000위까지 정확한 순위 저장 (메모리: 120KB)
- 레벨별 샘플링: 각 레벨 중간 지점 샘플 추가 (메모리: +50KB)

---

## 실전 시나리오

### 시나리오 1: 레벨업 직후

```cpp
// 플레이어가 레벨 49 -> 50으로 레벨업
// 경험치: 50,000 -> 0 (초기화)

// Before: 레벨 49, 경험치 50,000 -> 상위 20%
// After:  레벨 50, 경험치 0      -> 상위 85% (레벨 50 중 최하위)
```

**사용자 표시:**
- "레벨업! 순위가 잠시 하락했지만, 곧 회복할 수 있다."

### 시나리오 2: 경험치 대량 획득

```cpp
// 이벤트로 경험치 대량 획득
// 레벨 50, 경험치 1,000 -> 40,000

// Before: 경험치 1,000  -> 상위 85%
// After:  경험치 40,000 -> 상위 28%
```

**사용자 표시:**
- "축하! 순위가 상위 85% -> 상위 28%로 상승!"

### 시나리오 3: 상위 1000위 진입

```cpp
// 플레이어가 1001등 -> 999등 진입

// Before: 선형보간 순위 (상위 0.01%)
// After:  정확한 순위 (999등)
```

**사용자 표시:**
- "축하한다! TOP 1000 진입! (999등)"
- "특별 보상 획득!"

---

## 구현 시 고려사항

### 1. 동시성 제어
- **읽기 전용 작업**: Lock 없이 Read-Only 복사본 사용
- **경계 갱신**: Write Lock + 원자적 교체

```cpp
// Reader-Writer Lock 패턴
std::shared_ptr<RankingSystem> gRankingSystem = nullptr;
Lock gRankingSystemLock;

// 읽기 (Lock-Free)
std::shared_ptr<RankingSystem> snapshot = std::atomic_load(&gRankingSystem);

// 쓰기 (주기적 갱신)
{
    WriteLock lock(gRankingSystemLock);
    auto newSystem = std::make_shared<RankingSystem>(*gRankingSystem);
    newSystem->UpdateLevelBoundaries();
    std::atomic_store(&gRankingSystem, newSystem);
}
```

### 2. 캐싱
- **플레이어 순위 캐싱**: 5분간 캐시 (레벨업/경험치 획득 시 무효화)
- **Top 100 목록 캐싱**: 1분간 캐시

### 3. DB 저장
```sql
-- 레벨별 경계 데이터 테이블
CREATE TABLE RankingLevelBoundary
(
    level INT PRIMARY KEY,
    minExp BIGINT,
    minExpRankPercent FLOAT,
    maxExp BIGINT,
    maxExpRankPercent FLOAT,
    updatedAt DATETIME
);

-- 상위 1000위 스냅샷
CREATE TABLE RankingTop1000
(
    rank INT PRIMARY KEY,
    userId BIGINT,
    level INT,
    exp BIGINT,
    updatedAt DATETIME
);
```

---

## 응용: 멀티 카테고리 랭킹

본 시스템은 다양한 랭킹에 적용 가능한다:

| 랭킹 카테고리 | 기준 | 레벨 대체 차원 |
|--------------|------|---------------|
| **레벨 랭킹** | 레벨 + 경험치 | 레벨 |
| **전투력 랭킹** | 전투력 | 직업 (전사/마법사/궁수) |
| **재화 랭킹** | 골드 보유량 | VIP 등급 |
| **업적 랭킹** | 업적 점수 | 플레이 시간 구간 (초보/중수/고수) |

---

