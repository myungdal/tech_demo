# 38. IFF (Identification Friend or Foe) 처리 고도화 방안 (작성중)

작성자: 안명달 (mooondal@gmail.com)

## 개요

게임 서버에서 진영/적대 관계 판별(IFF: Identification Friend or Foe)과 그룹 동적 병합(Union Find)과 bitset으로의 표현과 캐싱 구현 아이디어를 작성중.

---

## IFF (Identification Friend or Foe)

### 용도
- 유닛 간 적/아군 판별
- 타겟팅 시스템
- 스킬/공격 대상 필터링
- PvP/PvE 전환

### IffRelationType (현재 구현)

```cpp
enum class IffRelationType : uint8_t
{
    NONE = 0,         // 관계 없음
    SELF = 1,         // 동일 객체
    USER_VS_NPC = 2,  // 유저 vs NPC인 경우
    SAME_SPECIES = 3, // 동일 종족
    CHAOS = 4,        // 혼란 상태 (무법자 등)
    HOSTILE = 5,      // 적대적 관계
    PARTY_MEMBER = 6, // 파티 멤버
    GUILD_MEMBER = 7, // 길드 멤버
    ALLIED = 8,       // 동맹
    ENEMY = 9,        // 적대 관계
    MAX = 10
};
```

### WorldIffChecker (현재 구현)

`WorldIffChecker::IsFriend()` 함수로 두 엔티티 간 아군 여부를 판별한다:

```cpp
// 아군 판별 조건 (순서대로)
1. 동일 객체 -> true
2. 같은 리더를 따르는 팔로워들 -> true
3. 리더-팔로워 관계 -> true
4. PC 간: 같은 worldUserId -> true
5. NPC 간: 같은 spawner -> true
6. 그 외 -> false
```

### bitset 기반 IFF 필터링 (확장 예정)

빠른 관계 필터링을 위해 `std::bitset` 기반 마스크 연산 도입 예정:

```cpp
// 예정된 구현 방향
using IffMask = std::bitset<_IDX(IffRelationType::MAX)>;

// 스킬별 타겟 가능 관계 정의
constexpr IffMask SKILL_TARGET_ENEMY = 
    (1 << _IDX(IffRelationType::HOSTILE)) | 
    (1 << _IDX(IffRelationType::ENEMY));

constexpr IffMask SKILL_TARGET_ALLY = 
    (1 << _IDX(IffRelationType::PARTY_MEMBER)) | 
    (1 << _IDX(IffRelationType::GUILD_MEMBER)) | 
    (1 << _IDX(IffRelationType::ALLIED));

// O(1) 관계 체크
bool CanTarget(IffRelationType relation, IffMask allowedMask)
{
    return allowedMask.test(_IDX(relation));
}
```

**bitset 장점:**
- O(1) 복잡도로 다중 관계 동시 체크
- 스킬/버프별 타겟 조건을 마스크로 정의
- 컴파일 타임 상수로 최적화 가능

---

## Union Find (Disjoint Set)

### 용도
- 그룹 동적 병합/분리
- 연결된 컴포넌트 탐지
- 실시간 클랜/길드 통합
- 맵 영역 연결성 판별

### 구현 예정
- Path Compression 최적화
- Union by Rank/Size
- 실시간 그룹 상태 동기화

---

## 캐싱 및 브로드캐스팅 전략

### IFF 캐싱

매 프레임 IFF 계산을 줄이기 위한 캐싱 전략:

```cpp
// 엔티티별 IFF 캐시
struct IffCache
{
    IffMask friendMask;      // 아군으로 판정된 엔티티 마스크
    IffMask enemyMask;       // 적으로 판정된 엔티티 마스크
    uint32_t frameUpdated;   // 마지막 갱신 프레임
};

// 캐시 무효화 조건
- 파티/길드 변경 시
- 진영 변경 시 (배신, 동맹 등)
- 혼란 상태 진입/해제 시
```

### 브로드캐스팅 전략

IFF 관계 변경 시 효율적인 전파:

| 이벤트 | 브로드캐스트 범위 | 방식 |
|--------|------------------|------|
| 파티 가입/탈퇴 | 파티 멤버 전원 | Multicast |
| 길드 가입/탈퇴 | 같은 채널 길드원 | Multicast |
| 동맹 체결/파기 | 양측 진영 전원 | Topic 기반 Pub/Sub |
| PvP 모드 전환 | 주변 유저 | 시야 범위 Broadcast |

### Union Find 기반 그룹 동기화

```cpp
// 그룹 변경 시 브로드캐스트 최적화
1. Union 연산 -> 병합된 그룹 대표 노드 결정
2. 대표 노드가 그룹 내 IFF 변경 사항 집계
3. 단일 브로드캐스트로 그룹 전원 통보
```

---

## 예상 활용처

| 시스템 | IFF | Union Find | 캐싱 |
|--------|-----|------------|------|
| 전투 시스템 | 적/아군 판별 | - | 프레임 캐시 |
| 파티/길드 | 멤버 IFF 동기화 | 그룹 병합 | 이벤트 기반 무효화 |
| 공성전 | 진영별 IFF | 동맹 그룹 관리 | Topic Pub/Sub |
| 맵 생성 | - | 영역 연결성 검증 | - |

---

