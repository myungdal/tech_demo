# 20. BadWordFilter - Trie 기반 다중 패턴 금지어 필터

작성자: 안명달 (mooondal@gmail.com)

> **목차로 돌아가기**: [tech.md](tech.md)

---

## 개요

BadWordFilter는 Trie(트라이) 자료구조를 활용한 다중 패턴 매칭 알고리즘으로, 텍스트에서 금지어를 효율적으로 검출한다.
Github에 유사 구현이 많고, 비슷한 방법으로 다시 구현한 클래스이다. 

---

## 핵심 구조

### 1. Trie 노드 (`BadWordNode`)

```cpp
struct BadWordNode
{
	BadWordType mLeafType = BadWordType::NONE;  // 단어 종료 타입
	BadWordNodeMap mChildren;                    // 자식 노드 맵 (문자 -> 다음 노드)
};
```

**트리 구조 예시**:
```
"bad", "bag" 등록 시:

   root
    └─ 'b'
        └─ 'a'
            ├─ 'd' (BLACK)
            └─ 'g' (BLACK)
```

### 2. 금지어 타입 시스템

```cpp
enum class BadWordType : uint8_t
{
	NONE = 0,   // 중간 노드 (단어 종료 아님)
	BLACK,      // 블랙리스트 (검증 실패)
	WHITE       // 화이트리스트 (검증 통과, BLACK보다 우선)
};
```

**우선순위**: `WHITE > BLACK > NONE`

**예시**:
- "ass"를 BLACK으로 등록
- "assassin"을 WHITE로 등록
- -> "assassin" 입력 시 WHITE가 우선하여 통과

---

## 핵심 알고리즘

### 1. 금지어 삽입 (`InsertWord`)

```cpp
void BadWordFilter::InsertWord(const std::wstring& word, BadWordType leafType)
{
	BadWordNode* node = mRoot;

	// 트라이에 문자별로 노드를 생성하며 경로를 만든다
	for (wchar_t ch : word)
	{
		auto it = node->mChildren.find(ch);
		if (it == node->mChildren.end())
		{
			auto emplaceResult = node->mChildren.emplace(ch, _NEW<BadWordNode>());
			it = emplaceResult.first;
		}
		node = it->second;
	}

	// 마지막 노드에 타입을 설정 (금지어의 끝)
	node->mLeafType = leafType;
}
```

**시간 복잡도**: `O(m)` (m = 단어 길이)

### 2. 텍스트 검증 (`ValidateText`)

```cpp
bool BadWordFilter::ValidateText(const std::wstring& text) const
{
	const size_t len = text.length();

	// 모든 시작 위치에서 트라이 탐색
	for (size_t i = 0; i < len; ++i)
	{
		BadWordType lastLeafType = BadWordType::NONE;
		BadWordNode* node = mRoot;

		// i 위치부터 트라이를 따라가며 매칭되는 금지어를 찾는다
		for (size_t j = i; j < len; ++j)
		{
			const wchar_t ch = text[j];
			const auto it = node->mChildren.find(ch);

			if (it == node->mChildren.end())
				break;

			node = it->second;

			// [중요] 최장 매칭 추적
			// 예: "bad"와 "badword" 둘 다 금지어일 때,
			//     더 긴 "badword"도 찾을 수 있도록 계속 탐색
			if (node->mLeafType != BadWordType::NONE)
				lastLeafType = node->mLeafType;
		}

		// BLACK 타입 금지어가 발견되면 검증 실패
		if (lastLeafType == BadWordType::BLACK)
			return false;
	}

	return true;
}
```

**시간 복잡도**: `O(n * m)` (n = 텍스트 길이, m = 가장 긴 금지어 길이)

**개선 가능성**: Aho-Corasick 알고리즘 적용 시 `O(n + z)` (z = 매칭 횟수)

---

## 복잡도 분석

| 항목 | 복잡도 | 설명 |
|------|--------|------|
| **InsertWord** | O(m) | m = 단어 길이 |
| **ValidateText** | O(n * m) | n = 텍스트 길이, m = 최장 금지어 |
| **공간 복잡도** | O(ALPHABET_SIZE * M * K) | M = 평균 단어 길이, K = 단어 수 |
| **Aho-Corasick 개선** | O(n + z) | z = 매칭 횟수 (failure link 추가) |

---

## 테스트 커버리지

```cpp
// Test 1: 기본 금지어 검출
filter.InsertWord(L"badword", BadWordType::BLACK);
!filter.ValidateText(L"this is a badword here");  // 차단

// Test 4: 접두사 금지어 (짧은 단어가 긴 단어의 접두사)
filter.InsertWord(L"ass", BadWordType::BLACK);
filter.InsertWord(L"assassin", BadWordType::BLACK);
!filter.ValidateText(L"class");  // "ass"가 "class" 내부에서 검출

// Test 5: 화이트리스트 우선순위
filter.InsertWord(L"ban", BadWordType::BLACK);
filter.InsertWord(L"banana", BadWordType::WHITE);
filter.ValidateText(L"I like banana");  // WHITE 우선 통과

// Test 6: 유니코드 한글 처리
filter.InsertWord(L"욕설", BadWordType::BLACK);
!filter.ValidateText(L"욕설하지마");  // 한글 정상 검출

// Test 8: 단일 문자 금지어
filter.InsertWord(L"X", BadWordType::BLACK);
!filter.ValidateText(L"aXb");  // 단일 문자도 검출
```

**테스트 파일**: `server/test/Test/TestBadWordFilter/TestBadWordFilter.cpp`

---

## 주요 특징

### 1. UTF-16 완벽 지원
- `std::wstring` 사용 -> **한글, 이모지 등 유니코드 완벽 처리**
- `wchar_t` 단위 트라이 탐색 -> **언어 독립적**

### 2. 최장 매칭 (Longest Match)
```cpp
// "bad"와 "badword" 둘 다 금지어일 때
if (node->mLeafType != BadWordType::NONE)
    lastLeafType = node->mLeafType;  // 계속 탐색하여 더 긴 매칭 추적
```

### 3. 블랙리스트 + 화이트리스트
- **BLACK**: 금지어 차단
- **WHITE**: 예외 허용 (BLACK보다 우선)
- **예시**: "ass" BLACK, "assassin" WHITE -> "assassin"은 통과

### 4. 부분 매칭
- **단어 경계 검사 없음** -> "class"에서 "ass" 검출
- **엄격한 필터링** 정책 적용

---

## 실전 사용 예시

```cpp
BadWordFilter filter;

// 금지어 등록
filter.InsertWord(L"금지어1", BadWordType::BLACK);
filter.InsertWord(L"금지어2", BadWordType::BLACK);
filter.InsertWord(L"예외단어", BadWordType::WHITE);  // 화이트리스트

// 텍스트 검증
bool isValid = filter.ValidateText(L"사용자 입력 텍스트");

if (!isValid)
{
    // 금지어 발견 -> 에러 처리
    SendError("부적절한 표현이 포함되어 있다.");
}
else
{
    // 정상 처리
    ProcessUserInput();
}
```

---

## 장점

| 장점 | 설명 |
|------|------|
| **단순하고 직관적** | 트리 구조로 이해하기 쉬움 |
| **다중 패턴 지원** | 수천 개의 금지어를 한 번에 검색 |
| **UTF-16 완벽 지원** | 한글, 이모지 등 모든 유니코드 처리 |
| **화이트리스트** | 예외 단어 우선 처리 (유연한 필터링) |
| **최장 매칭** | 짧은 금지어가 긴 단어의 접두사일 때 올바르게 처리 |
| **메모리 효율** | 공통 접두사 공유 (O(ALPHABET_SIZE * M * K)) |
| **확장 가능** | Aho-Corasick으로 O(n) 개선 가능 |

---

## 개선 여지

### Aho-Corasick 알고리즘 적용 시
- **Failure Link** 추가 -> 백트래킹 제거
- **시간 복잡도**: O(n * m) -> **O(n + z)** (z = 매칭 횟수)
- **효과**: 텍스트 길이 n에만 비례 (금지어 길이 m 무관)

**예시**:
```cpp
// 현재: O(n * m) - 매 위치마다 트라이 탐색
for (size_t i = 0; i < len; ++i)         // n번
    for (size_t j = i; j < len; ++j)     // 최대 m번

// Aho-Corasick: O(n) - failure link로 백트래킹 제거
for (size_t i = 0; i < len; ++i)         // n번만
    node = node->failure_link;            // O(1) 점프
```

---

[목차로 돌아가기](tech.md)
