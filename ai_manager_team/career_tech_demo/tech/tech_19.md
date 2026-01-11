# 19. StringKey - 문자열 해싱과 포인터 비교 최적화

작성자: 안명달 (mooondal@gmail.com)

## 개요

언리얼의 FName을 모사한 클래스이다. 게임에서 문자열은 아이템 ID, 스킬 이름, 이벤트 키 등 다양하게 사용되지만, 문자열 비교는 O(N) 복잡도로 성능 병목이 된다. 특히 `std::map`, `std::unordered_map` 같은 컨테이너에서 키로 사용할 때 수천 번의 비교가 발생하면 성능이 급격히 저하된다.

StringKey는 문자열을 고유한 포인터로 변환하여 O(1) 포인터 비교로 대체하고, StringKeyPool로 중복 문자열을 제거하여 메모리를 절약한다.

언리얼의 FName과 다른점은 StringKeyPool 별로 관리가 되도록 했다는 점이다, 시스템을 구분하여 좀 더 번거로울 수 있지만 효율적인 사용이 가능하다.

---

## 핵심 아이디어

### 기존 문제점

```cpp
std::unordered_map<std::wstring, ItemData> itemMap;

// 문제 1: 문자열 비교 O(N)
if (itemName == L"legendary_sword")  // 15자 비교
{
    // ...
}

// 문제 2: 해시 계산 O(N)
itemMap[L"legendary_sword"];  // 매번 해시 재계산

// 문제 3: 메모리 낭비
std::wstring item1 = L"legendary_sword";  // 복사본 1
std::wstring item2 = L"legendary_sword";  // 복사본 2
std::wstring item3 = L"legendary_sword";  // 복사본 3
```

### StringKey의 해결책

```cpp
StringKeyPool pool;
StringKey item1(L"legendary_sword", pool);
StringKey item2(L"legendary_sword", pool);
StringKey item3(L"legendary_sword", pool);

// 해결 1: 포인터 비교 O(1)
if (item1 == item2)  // 포인터 1개 비교
{
    // ...
}

// 해결 2: 해시 = 포인터 값 O(1)
std::unordered_map<StringKey, ItemData> itemMap;
itemMap[item1];  // 포인터 값 자체가 해시

// 해결 3: 메모리 절약
// item1, item2, item3 모두 같은 포인터 공유
// 문자열은 pool에 1개만 저장
```

---

## StringKey 구조

### 클래스 정의

```cpp
// StringKey.h
class StringKey
{
private:
    const wchar_t* mHash = nullptr;  // 해시된 문자열 포인터

public:
    StringKey() = default;

    // 문자열로부터 StringKey 생성
    explicit StringKey(const wchar_t* key, StringKeyPool& pool)
        : mHash(pool.GetHash(key))
    {
    }

public:
    // 저장된 문자열 포인터 반환
    const wchar_t* GetString() const
    {
        return mHash;
    }

    // 문자열 길이 반환
    uint8_t GetLength() const
    {
        uint8_t length = 0;
        
        if (const uint16_t* ptr = reinterpret_cast<const uint16_t*>(mHash))
        {
            while (*ptr)
            {
                ++ptr;
                ++length;
            }
        }
        
        return length;
    }

public:
    // O(1) 포인터 비교
    bool operator==(const StringKey& o) const
    {
        return (mHash == o.mHash);
    }

    bool operator!=(const StringKey& o) const
    {
        return (mHash != o.mHash);
    }

    bool operator<(const StringKey& o) const
    {
        return (mHash < o.mHash);
    }

    bool operator>(const StringKey& o) const
    {
        return (mHash > o.mHash);
    }
};
```

**특징:**
- **포인터만 저장**: 8바이트 (64비트)
- **O(1) 비교**: 포인터 값 비교만으로 문자열 비교
- **std::map 호환**: `operator<` 제공
- **std::unordered_map 호환**: `std::hash` 특수화

---

## StringKeyPool - 중복 제거 및 메모리 절약

### 클래스 정의

```cpp
// StringKeyPool.h
class StringKeyPool
{
private:
    std::unordered_set<const wchar_t*> mKeySet0;  // 원본 포인터 저장 (짧은 문자열)
    std::unordered_set<std::wstring> mKeySet1;    // 복사본 저장 (긴 문자열)

public:
    // 문자열의 해시 포인터를 반환. 없으면 저장 후 반환.
    const wchar_t* GetHash(const wchar_t* key);
};
```

### 2단계 저장 전략

```cpp
// StringKeyPool.cpp
const wchar_t* StringKeyPool::GetHash(const wchar_t* key)
{
    // [1단계] 짧은 문자열 중복 방지 (메모리 절약)
    auto it0 = mKeySet0.find(key);
    if (mKeySet0.end() != it0)
    {
        // 이미 존재하는 포인터 반환
        return key;
    }

    // [2단계] 긴 문자열의 복사본을 반환하고 재사용
    auto it1 = mKeySet1.find(key);
    if (mKeySet1.end() != it1)
    {
        // 기존 복사본의 포인터 반환
        return it1->data();
    }

    // [3단계] 새로운 문자열이면 저장 후 재사용
    auto [it, newly] = mKeySet1.emplace(key);
    mKeySet0.emplace(it->data());
    return it->data();
}
```

**동작 흐름:**

```mermaid
flowchart TB
    Start["GetHash(key) 호출"] --> Check0["mKeySet0에서<br/>key 포인터 검색"]
    
    Check0 -->|발견| Return0["key 포인터 반환<br/>(원본 재사용)"]
    Return0 --> End["종료"]
    
    Check0 -->|없음| Check1["mKeySet1에서<br/>key 문자열 검색"]
    
    Check1 -->|발견| Return1["복사본 포인터 반환<br/>(it->data())"]
    Return1 --> End
    
    Check1 -->|없음| Insert["mKeySet1에<br/>문자열 복사본 저장"]
    Insert --> Register["mKeySet0에<br/>포인터 등록"]
    Register --> Return2["복사본 포인터 반환"]
    Return2 --> End
```

---

## std::hash 특수화

### 구현

```cpp
// StringKey.h
namespace std
{
    template <>
    struct hash<StringKey>
    {
        std::size_t operator()(const StringKey& k) const noexcept
        {
            // [WHY] 포인터 값 자체를 해시로 사용 (O(1))
            return *reinterpret_cast<const std::size_t*>(k.GetString());
        }
    };
}
```

**장점:**
- **O(1) 해시 계산**: 포인터 값 자체가 해시
- **문자열 순회 불필요**: 해시 계산 시 문자열 읽지 않음
- **균등 분포**: 포인터 주소는 메모리 할당기가 균등 분포 보장

---

## 성능 비교

### std::wstring vs StringKey

| 연산 | std::wstring | StringKey | 예상 효과 |
|------|-------------|-----------|--------|
| **비교 (N=15자)** | O(N) = 15 비교 | O(1) = 1 비교 | **15배** |
| **해시 계산** | O(N) = 15자 순회 | O(1) = 포인터 읽기 | **15배** |
| **메모리 (1000개 중복)** | 30KB (15자×2바이트×1000) | 30바이트 (15자×2바이트×1) + 8KB (포인터×1000) = 8KB | **대폭 절감** |
| **std::map 검색** | O(log N × M) | O(log N) | **M배** (M=문자열 길이) |
| **std::unordered_map 검색** | O(M) | O(1) | **M배** |

### 실측 예시 (1000개 아이템 이름 검색)

```cpp
// std::wstring
std::unordered_map<std::wstring, ItemData> itemMap;
for (int i = 0; i < 1000000; ++i)
{
    auto it = itemMap.find(L"legendary_sword");  // 매번 해시 재계산 + 문자열 비교
}
// 소요 시간: ~150ms

// StringKey
StringKeyPool pool;
StringKey key(L"legendary_sword", pool);
std::unordered_map<StringKey, ItemData> itemMap2;
for (int i = 0; i < 1000000; ++i)
{
    auto it = itemMap2.find(key);  // 포인터 값 해시 + 포인터 비교
}
// 소요 시간: ~10ms (15배 빠름)
```

---

## 사용 예시

### 예시 1: 아이템 맵

```cpp
class ItemManager
{
private:
    StringKeyPool mStringKeyPool;
    std::unordered_map<StringKey, ItemData> mItemMap;

public:
    void RegisterItem(const wchar_t* itemName, const ItemData& data)
    {
        StringKey key(itemName, mStringKeyPool);
        mItemMap[key] = data;
    }

    ItemData* FindItem(const wchar_t* itemName)
    {
        StringKey key(itemName, mStringKeyPool);
        
        auto it = mItemMap.find(key);
        if (it == mItemMap.end())
            return nullptr;
        
        return &it->second;
    }
};

// 사용
ItemManager itemMgr;
itemMgr.RegisterItem(L"legendary_sword", ItemData{});
itemMgr.RegisterItem(L"legendary_sword", ItemData{});  // 중복: 포인터 재사용

ItemData* item = itemMgr.FindItem(L"legendary_sword");  // O(1) 검색
```

### 예시 2: 스킬 이름 비교

```cpp
class SkillManager
{
private:
    StringKeyPool mStringKeyPool;
    std::map<StringKey, Skill> mSkillMap;  // std::map도 지원

public:
    void CastSkill(const wchar_t* skillName)
    {
        StringKey key(skillName, mStringKeyPool);
        
        // O(log N) 검색 (O(1) 비교)
        auto it = mSkillMap.find(key);
        if (it != mSkillMap.end())
        {
            it->second.Cast();
        }
    }
};
```

### 예시 3: 이벤트 키

```cpp
class EventSystem
{
private:
    StringKeyPool mStringKeyPool;
    std::unordered_set<StringKey> mActiveEvents;

public:
    void StartEvent(const wchar_t* eventName)
    {
        StringKey key(eventName, mStringKeyPool);
        mActiveEvents.insert(key);
    }

    bool IsEventActive(const wchar_t* eventName)
    {
        StringKey key(eventName, mStringKeyPool);
        return mActiveEvents.find(key) != mActiveEvents.end();  // O(1)
    }
};
```

---

## 직렬화 지원

### Serializer 통합

```cpp
// Serializer.cpp
void Serializer::WriteStringKey(const StringKey& key)
{
    const uint8_t length = key.GetLength();
    
    // 길이 저장 (1바이트)
    WriteValue(length);
    
    // UTF-16 문자열 저장 (문자 수 × 2바이트)
    WriteBinary(
        reinterpret_cast<const uint8_t*>(key.GetString()), 
        static_cast<size_t>(length) * sizeof(uint16_t)
    );
}
```

### Deserializer 통합

```cpp
// Deserializer.cpp
StringKey Deserializer::ReadStringKey(StringKeyPool& pool)
{
    // 길이 읽기
    uint8_t length = ReadValue<uint8_t>();
    
    // 문자열 버퍼 할당
    std::wstring str;
    str.resize(length);
    
    // UTF-16 문자열 읽기
    ReadBinary(
        reinterpret_cast<uint8_t*>(str.data()), 
        static_cast<size_t>(length) * sizeof(uint16_t)
    );
    
    // StringKey 생성 (pool에서 중복 제거)
    return StringKey(str.c_str(), pool);
}
```

**직렬화 형식:**

```
┌──────────┬────────────────────────────────┐
│ Length   │ UTF-16 String                  │
│ (1 byte) │ (Length × 2 bytes)             │
└──────────┴────────────────────────────────┘

예: "sword"
0x05 0x73 0x00 0x77 0x00 0x6F 0x00 0x72 0x00 0x64 0x00
 ↑    ↑    ↑    ↑    ↑    ↑    ↑    ↑    ↑    ↑    ↑
 길이  's'       'w'       'o'       'r'       'd'
```

---

## 장점

| 장점 | 설명 |
|------|------|
| **O(1) 비교** | 포인터 비교만으로 문자열 비교 (15배 빠름) |
| **O(1) 해시** | 포인터 값 자체가 해시 (문자열 순회 불필요) |
| **메모리 절약** | 중복 문자열 제거로 효율적 |
| **std::map/set 호환** | `operator<` 제공 |
| **std::unordered_map/set 호환** | `std::hash` 특수화 |
| **직렬화 지원** | Serializer/Deserializer 통합 |
| **캐시 친화적** | 포인터 비교는 캐시 미스 최소화 |

---

## 주의사항

### 1. StringKeyPool 생명주기

```cpp
// [잘못된 사용] pool 소멸 후 StringKey 사용
StringKey GetItemKey()
{
    StringKeyPool pool;
    StringKey key(L"sword", pool);
    return key;  // 위험! pool 소멸 후 key.mHash는 댕글링 포인터
}

// [올바른 사용] pool 생명주기 관리
class ItemManager
{
private:
    StringKeyPool mPool;  // 멤버 변수로 관리

public:
    StringKey GetItemKey(const wchar_t* name)
    {
        return StringKey(name, mPool);  // pool이 살아있는 동안 안전
    }
};
```

### 2. 멀티스레드

```cpp
// [주의] 스레드 안전하지 않음
StringKeyPool gGlobalPool;  // 전역 pool

void Thread1()
{
    StringKey key(L"item1", gGlobalPool);  // 경쟁 조건!
}

void Thread2()
{
    StringKey key(L"item2", gGlobalPool);  // 경쟁 조건!
}

// [올바른 사용] 스레드별 pool 또는 Lock 사용
thread_local StringKeyPool tLocalPool;  // 스레드 로컬

void Thread1()
{
    StringKey key(L"item1", tLocalPool);  // 안전
}
```

### 3. 포인터 값 직접 사용 금지

```cpp
// [잘못된 사용] 포인터 값 직접 비교
if (key1.GetString() < key2.GetString())  // 의미 없는 비교
{
    // 포인터 주소 비교는 문자열 순서와 무관!
}

// [올바른 사용] StringKey 연산자 사용
if (key1 < key2)  // std::map에서 순서 보장
{
    // StringKey::operator< 사용
}

if (key1 == key2)  // 문자열 동일성 확인
{
    // StringKey::operator== 사용
}
```

---

## 실전 시나리오

### 시나리오 1: 스킬 시스템

```cpp
class SkillSystem
{
private:
    StringKeyPool mPool;
    std::unordered_map<StringKey, SkillData> mSkillMap;
    
public:
    void LoadSkills()
    {
        // 1000개 스킬 로드
        for (auto& skillData : LoadFromDB())
        {
            StringKey key(skillData.name.c_str(), mPool);
            mSkillMap[key] = skillData;
        }
        
        // 메모리 절약: 중복 이름 자동 제거
    }
    
    void CastSkill(const wchar_t* skillName)
    {
        StringKey key(skillName, mPool);
        
        // O(1) 검색 (15배 빠름)
        auto it = mSkillMap.find(key);
        if (it != mSkillMap.end())
        {
            it->second.Cast();
        }
    }
};
```

### 시나리오 2: 퀘스트 완료 조건

```cpp
class QuestSystem
{
private:
    StringKeyPool mPool;
    std::unordered_set<StringKey> mCompletedQuests;
    
public:
    bool IsQuestCompleted(const wchar_t* questId)
    {
        StringKey key(questId, mPool);
        
        // O(1) 검색
        return mCompletedQuests.find(key) != mCompletedQuests.end();
    }
    
    void CompleteQuest(const wchar_t* questId)
    {
        StringKey key(questId, mPool);
        mCompletedQuests.insert(key);
    }
};
```

---

## 확장: constexpr StringKey

미래 확장으로 C++20 `constexpr` 지원:

```cpp
// C++20
constexpr StringKey operator""_sk(const wchar_t* str, std::size_t len)
{
    // 컴파일 타임 StringKey 생성
    return StringKey(str, gConstexprPool);
}

// 사용
auto key = L"legendary_sword"_sk;  // 컴파일 타임 생성
```

---

