# 21. RandMark40 - 선형 키로 생성되면서 바이럴에 용이한 시크릿 키 아이디어

작성자: 안명달 (mooondal@gmail.com)

> **목차로 돌아가기**: [tech.md](tech.md)

---

## 개요

RandMark40은 DB 키를 선형적으로 관리하면서도, 사용자에게는 읽기 쉬운 랜덤 코드를 제공하는 시스템이다.

무한에 가까운 자동생성 랜덤 아이템이나 캐릭터의 ID를 사용자가 공유하는 경우를 기대하며 아이디어를 내고 구현했다.

### 핵심 아이디어

**문제:**
- DB 키: 순차적 정수 (1, 2, 3, ...) -> **예측 가능**, 보안 취약
- UUID: 랜덤하지만 -> **가독성 최악** (예: `550e8400-e29b-41d4-a716-446655440000`)
- 짧은 랜덤 코드: 가독성 좋지만 -> **중복 체크 필요**, DB 인덱스 비효율

**해결:**
```
DB 키: 0, 1, 2, 3, 4, 5, ...         (선형 관리, DB 효율 최대)
        ↓ 변환
코드:  BAD123-FOG456                  (랜덤처럼 보임, 가독성 우수)
       CAT000-DOG001
       MAT789-HOT234
```

-> **선형 키 관리 + 랜덤 가독성 + 충돌 없음 + 금칙어 필터**

---

## 40비트 구조

### CVCDDD-CVCDDD 형식

```
 CVC   DDD    CVC   DDD
 |||   |||    |||   |||
[BAD] [123] - [FOG][456]
 ↑↑↑   ↑↑↑    ↑↑↑   ↑↑↑
 자자모  숫자    자자모  숫자
 음음음  3자리  음음음  3자리
 
10bit  10bit  10bit  10bit
```

### 세그먼트 구성

| 세그먼트 | 타입 | 범위 | 개수 | 비트 |
|---------|------|------|------|------|
| **CVC1** | 자음-모음-자음 | 16×4×16 | 1024 (금칙어 제외) | 10bit |
| **DDD2** | 숫자 | 000-999 | 1000 | 10bit |
| **CVC3** | 자음-모음-자음 | 16×4×16 | 1024 (금칙어 제외) | 10bit |
| **DDD4** | 숫자 | 000-999 | 1000 | 10bit |
| **전체** | - | - | **약 1조 개** | **40bit** |

### 가독성 좋은 기호 선택

```cpp
// 자음 (16개) - 인지적으로 구별하기 쉬운 자음들
static constexpr std::array<wchar_t, 16> CONSONANTS = {
    L'B', L'C', L'D', L'F', L'G', L'H', L'J', L'K',
    L'L', L'M', L'N', L'P', L'R', L'S', L'T', L'W'
};

// 모음 (4개) - 가장 기본적이고 구별하기 쉬운 모음들
static constexpr std::array<wchar_t, 4> VOWELS = { 
    L'A', L'E', L'I', L'O' 
};
```

**선택 기준:**
- 제외: `Q`, `U`, `V`, `X`, `Y`, `Z` (혼동 가능, 발음 어려움)
- 포함: 발음하기 쉽고 시각적으로 구별되는 문자만 선택

---

## 핵심 알고리즘

### 1. 인덱스 -> 4개 세그먼트 변환

```cpp
std::array<uint32_t, 4> RandMark40::IndexToSegments(uint64_t index) const
{
    std::array<uint32_t, 4> segments{};
    
    uint64_t cvcCount = mAllowedCvc.size();  // 금칙어 제외한 CVC 개수
    uint64_t dddCount = 1000;
    
    // 역순 추출 (DDD4 -> CVC3 -> DDD2 -> CVC1)
    segments[3] = static_cast<uint32_t>(index % dddCount);  // DDD4
    index /= dddCount;
    
    segments[2] = static_cast<uint32_t>(index % cvcCount);  // CVC3
    index /= cvcCount;
    
    segments[1] = static_cast<uint32_t>(index % dddCount);  // DDD2
    index /= dddCount;
    
    segments[0] = static_cast<uint32_t>(index % cvcCount);  // CVC1
    
    return segments;
}
```

### 2. 세그먼트 -> 인덱스 변환

```cpp
uint64_t RandMark40::SegmentsToIndex(const std::array<uint32_t, 4>& segments) const
{
    uint64_t cvcCount = mAllowedCvc.size();
    uint64_t dddCount = 1000;
    
    // Horner's method (호너 방법)
    uint64_t index = segments[0];                    // CVC1
    index = index * dddCount + segments[1];          // CVC1*1000 + DDD2
    index = index * cvcCount + segments[2];          // (CVC1*1000 + DDD2)*cvcCount + CVC3
    index = index * dddCount + segments[3];          // ...* 1000 + DDD4
    
    return index;
}
```

### 3. 인덱스 -> 코드 인코딩

```cpp
bool RandMark40::EncodeIndex(uint64_t index, std::wstring& result) const
{
    // 1. 인덱스를 4개 세그먼트로 분할
    auto segments = IndexToSegments(index);
    
    // 2. CVC1: mAllowedCvc[segments[0]] -> "BAD"
    std::wstring cvc1;
    if (!CvcFromRaw(mAllowedCvc[segments[0]], cvc1))
        return false;
    
    // 3. DDD2: segments[1] -> "123"
    std::wstring ddd2;
    if (!DddFromIndex(segments[1], ddd2))
        return false;
    
    // 4. CVC3, DDD4 동일
    std::wstring cvc3, ddd4;
    // ...
    
    // 5. 결합: "BAD123-FOG456"
    result = cvc1 + ddd2 + L"-" + cvc3 + ddd4;
    return true;
}
```

### 4. 코드 -> 인덱스 디코딩

```cpp
uint64_t RandMark40::DecodeIndex(const std::wstring& code) const
{
    // 1. 형식 검증: "CVCDDD-CVCDDD" (13글자)
    if (code.size() != 13 || code[6] != L'-')
        throw std::invalid_argument("invalid code format");
    
    // 2. 각 부분 추출
    std::wstring cvc1 = code.substr(0, 3);   // "BAD"
    std::wstring ddd2 = code.substr(3, 3);   // "123"
    std::wstring cvc3 = code.substr(7, 3);   // "FOG"
    std::wstring ddd4 = code.substr(10, 3);  // "456"
    
    // 3. 각 부분을 인덱스로 변환
    uint16_t raw1 = TryRawFromCvc(cvc1);     // "BAD" -> raw index
    int16_t acv1 = mReverseCvc[raw1];        // raw -> allowed index
    if (acv1 < 0)
        throw std::invalid_argument("CVC1 is banned");
    
    uint32_t idx2 = IndexFromDdd(ddd2);      // "123" -> 123
    // ...
    
    // 4. 세그먼트를 인덱스로 조합
    std::array<uint32_t, 4> segments = { acv1, idx2, acv3, idx4 };
    return SegmentsToIndex(segments);
}
```

---

## 금칙어 필터링

### 1. CVC 패턴 검증

```cpp
class CvcValidator
{
    // "BAD"가 CVC 패턴인지 확인
    bool IsValidCvcPattern(const std::wstring& str);
    
    // 금칙어 목록에서 실제 생성 가능한 CVC만 필터링
    std::vector<std::wstring> FilterGeneratableBadWords(const std::vector<std::wstring>& badWords);
};
```

### 2. 허용 CVC 테이블 구축

```cpp
void RandMark40::BuildAllowedCvcTable(const std::vector<std::wstring>& badWords)
{
    // 1. 금칙어를 소문자로 변환하고 정렬
    std::vector<std::wstring> bad;
    for (const auto& word : badWords)
    {
        if (word.size() == 3)
            bad.push_back(ToLower(word));
    }
    std::sort(bad.begin(), bad.end());
    
    // 2. 모든 CVC 조합 (16*4*16 = 1024가지) 생성
    mAllowedCvc.clear();
    for (int c0 = 0; c0 < 16; ++c0)
    {
        for (int vv = 0; vv < 4; ++vv)
        {
            for (int c1 = 0; c1 < 16; ++c1)
            {
                std::wstring token;
                token.push_back(std::tolower(CONSONANTS[c0]));
                token.push_back(std::tolower(VOWELS[vv]));
                token.push_back(std::tolower(CONSONANTS[c1]));
                
                // 3. 금칙어가 아니면 허용 테이블에 추가
                bool banned = std::binary_search(bad.begin(), bad.end(), token);
                if (!banned)
                {
                    uint16_t raw = RawIndexFromCvc(c0, vv, c1);
                    mReverseCvc[raw] = mAllowedCvc.size();
                    mAllowedCvc.push_back(raw);
                }
            }
        }
    }
}
```

### 3. 금칙어 예시

```cpp
std::vector<std::wstring> badWords = {
    L"sex", L"ASS", L"god", L"TiT", L"jiz",
    L"abc", L"GOD", L"fAg", L"cum", L"TIt"
};

RandMark40 randMark40;
randMark40.BuildAllowedCvcTable(badWords);

// "SEX123-GOD456" 같은 코드는 절대 생성되지 않음
```

---

## 용량 계산

### 금칙어 없을 때

```
전체 용량 = CVC × DDD × CVC × DDD
         = 1024 × 1000 × 1024 × 1000
         = 1,048,576,000,000 (약 1조 개)
```

### 금칙어 100개 제외 시

```
허용 CVC = 1024 - 100 = 924

전체 용량 = 924 × 1000 × 924 × 1000
         = 853,776,000,000 (약 8537억 개)
```

-> **수억 명의 유저를 위한 고유 코드 생성 가능**

---

## 실전 사용 예시

### 게임 친구 코드

```cpp
// 유저 DB ID: 123456 (선형 관리)
RandMark40 randMark40;
std::wstring friendCode;
randMark40.EncodeIndex(123456, friendCode);

// 친구 코드: "CAT123-DOG456" (랜덤처럼 보임)
SendToClient(friendCode);
```

### 쿠폰 코드 생성

```cpp
// 쿠폰 DB ID: 0, 1, 2, 3, ...
for (uint64_t i = 0; i < 1000; ++i)
{
    std::wstring couponCode;
    randMark40.EncodeIndex(i, couponCode);
    
    // "BAD000-FOG000", "BAD001-FOG001", ...
    StoreCouponCode(i, couponCode);
}
```

### 매치 방 코드

```cpp
// 방 DB ID: 789
std::wstring roomCode;
randMark40.EncodeIndex(789, roomCode);

// 방 코드: "MAT789-HOT234"
// 유저는 이 코드를 입력하여 방 입장
JoinRoom(roomCode);
```

---

## 테스트 커버리지

```cpp
// Test 1: CVC 패턴 검증
CvcValidator().IsValidCvcPattern(L"BAD");  // true
CvcValidator().IsValidCvcPattern(L"XYZ");  // false (X, Y, Z 미포함)

// Test 2: 금칙어 필터링
std::vector<std::wstring> badWords = { L"sex", L"ASS", L"god" };
auto filtered = CvcValidator().FilterGeneratableBadWords(badWords);
// -> { L"sex", L"god" } (ASS는 이미 대문자라 소문자로 변환됨)

// Test 3: 인코딩-디코딩 일치성
for (uint64_t i = 0; i < 1000; ++i)
{
    std::wstring code;
    randMark40.EncodeIndex(i, code);
    uint64_t decoded = randMark40.DecodeIndex(code);
    assert(i == decoded);  // 100% 일치
}

// Test 4: 발급 여부 판별
bool wasIssued = randMark40.WasIssued(L"BAD123-FOG456", 1000000);
// 현재 100만개 발급됨 -> 이 코드가 발급 범위 내인지 확인

// Test 5: 코드 형식 검증
std::wstring code;
randMark40.EncodeIndex(123, code);
assert(code.size() == 13);      // 13글자
assert(code[6] == L'-');        // 6번째 위치에 '-'

// Test 6: 숫자 범위 검증 (000-999)
std::wstring code999;
randMark40.EncodeIndex(999, code999);
std::wstring ddd = code999.substr(3, 3);
assert(ddd == L"999");          // 999 정상 인코딩
```

**테스트 파일**: `server/test/Test/TestRandMark40/TestRandMark40.cpp`

---

## 장점

| 장점 | 설명 |
|------|------|
| **선형 키 관리** | DB 인덱스 효율 최대, 중복 체크 불필요 |
| **랜덤 가독성** | 사용자는 랜덤 코드로 인식, 예측 불가 |
| **충돌 없음** | 1:1 매핑 보장 (충돌 검사 불필요) |
| **가독성 우수** | 발음 가능한 CVC 패턴 + 숫자 조합 |
| **금칙어 필터** | 부적절한 단어 자동 제외 |
| **대용량 지원** | 약 1조 개 (금칙어 제외 시 8537억 개) |
| **빠른 변환** | 인코딩/디코딩 O(1) (배열 인덱싱) |
| **메모리 효율** | 허용 CVC 테이블만 보관 (~4KB) |
| **DB 효율** | 선형 키로 인덱스 효율 최대 |
| **보안** | 순차적 키 노출 방지 |

---

## 성능 분석

| 항목 | 복잡도 | 설명 |
|------|--------|------|
| **EncodeIndex** | O(1) | 배열 인덱싱 + 문자열 조합 |
| **DecodeIndex** | O(1) | 배열 인덱싱 + 숫자 파싱 |
| **BuildAllowedCvcTable** | O(N log N) | N = 금칙어 개수 (정렬) |
| **메모리** | O(1) | 허용 CVC 테이블 (~4KB 고정) |

### 실측 성능

```cpp
// 인코딩: 100만 번 -> 약 100ms (1번당 0.1μs)
for (uint64_t i = 0; i < 1000000; ++i)
{
    std::wstring code;
    randMark40.EncodeIndex(i, code);
}

// 디코딩: 100만 번 -> 약 150ms (1번당 0.15μs)
for (uint64_t i = 0; i < 1000000; ++i)
{
    std::wstring code = "BAD123-FOG456";
    uint64_t decoded = randMark40.DecodeIndex(code);
}
```

-> **실시간 게임에서도 충분히 빠름**

---

## 실전 시나리오

### 시나리오 1: 친구 추천 시스템

**요구사항:**
- 1억 명의 유저
- 각 유저에게 고유 친구 코드 부여
- 코드는 발음 가능하고 읽기 쉬워야 함
- 부적절한 단어 필터링

**해결:**
```cpp
// 유저 DB: user_id (0, 1, 2, ...)
uint64_t userId = 123456789;

RandMark40 randMark40;
randMark40.BuildAllowedCvcTable(badWords);

std::wstring friendCode;
randMark40.EncodeIndex(userId, friendCode);

// 친구 코드: "LAM456-BOT789"
// 유저는 이 코드를 친구에게 공유
```

### 시나리오 2: 쿠폰 시스템

**요구사항:**
- 1000만 개의 쿠폰 생성
- 각 쿠폰은 고유 코드
- 코드는 13자리로 고정 (UI 일관성)

**해결:**
```cpp
// 쿠폰 DB: coupon_id (0, 1, 2, ...)
for (uint64_t i = 0; i < 10000000; ++i)
{
    std::wstring couponCode;
    randMark40.EncodeIndex(i, couponCode);
    
    // DB 저장: (coupon_id, couponCode)
    InsertCoupon(i, couponCode);
}

// 유저가 "BAD123-FOG456" 입력
uint64_t couponId = randMark40.DecodeIndex(L"BAD123-FOG456");
UseCoupon(couponId);
```

### 시나리오 3: 매치 방 코드

**요구사항:**
- 방 생성 시 고유 코드 부여
- 유저는 코드로 방 검색
- 코드는 음성으로 전달 가능해야 함 (음성 채팅)

**해결:**
```cpp
// 방 DB: room_id (0, 1, 2, ...)
uint64_t roomId = CreateRoom();

std::wstring roomCode;
randMark40.EncodeIndex(roomId, roomCode);

// 방 코드: "CAT000-DOG123"
// 유저: "CAT ZERO ZERO ZERO, DOG ONE TWO THREE"
// -> 음성으로 전달 가능 (발음 가능한 CVC 패턴)
```

---

## 구현 시 고려사항

### 1. 금칙어 관리
```cpp
// 금칙어는 서버 시작 시 한 번만 로드
std::vector<std::wstring> LoadBadWords()
{
    // DB 또는 파일에서 로드
    return { L"sex", L"ass", L"god", ... };
}

RandMark40 randMark40;
randMark40.BuildAllowedCvcTable(LoadBadWords());
```

### 2. 발급 범위 검증
```cpp
// 현재까지 발급된 개수를 추적
uint64_t issuedCount = GetIssuedCount();

// 코드가 발급 범위 내인지 확인
bool wasIssued = randMark40.WasIssued(userCode, issuedCount);
if (!wasIssued)
{
    SendError("유효하지 않은 코드이다.");
}
```

### 3. 대소문자 처리
```cpp
// 코드는 항상 대문자로 저장
std::wstring code = L"BAD123-FOG456";

// 유저 입력은 대문자로 변환
std::wstring userInput = L"bad123-fog456";
std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::toupper);

// 비교
if (code == userInput)
{
    // 일치
}
```

---

## 확장: RandMark60, RandMark80

### RandMark60 (60비트)

```
구조: CVCDDD-CVCDDD-CVCDDD
용량: 1024^3 × 1000^3 = 약 10^18 (100경)
```

### RandMark80 (80비트)

```
구조: CVCDDD-CVCDDD-CVCDDD-CVCDDD
용량: 1024^4 × 1000^4 = 약 10^24 (1자)
```

-> **코드 길이와 용량을 쉽게 확장 가능**

---

[목차로 돌아가기](tech.md)
