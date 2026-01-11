# 46. JavaScript lodash vs C++23 ranges - 함수형 데이터 처리 비교

## 개요

JavaScript의 **lodash**는 Node.js로 SLG 워게임 서버를 제작할 때 가장 활용도가 높았던 라이브러리 중 하나이다. C++23의 **std::ranges**와 비교해본 후 DB서버에서 활용을 검토해보고자 한다.

---

## 철학 비교

| 항목 | JavaScript lodash | C++23 ranges |
|------|-------------------|--------------|
| **타입** | 런타임 동적 타입 | 컴파일타임 정적 타입 |
| **평가** | 즉시 평가 (일부 lazy) | 지연 평가 (views) |
| **성능** | 런타임 오버헤드 | 제로코스트 추상화 |
| **체이닝** | `_.chain()` 또는 `_.flow()` | `|` 파이프 연산자 |
| **에러** | 런타임 에러 | 컴파일타임 에러 |

---

## 핵심 연산자 대응표

| lodash | C++23 ranges | 설명 |
|--------|--------------|------|
| `_.map(fn)` | `views::transform(fn)` | 각 요소를 변환 |
| `_.filter(pred)` | `views::filter(pred)` | 조건에 맞는 요소 선택 |
| `_.reduce(fn, init)` | `ranges::fold_left(init, fn)` | 누적 연산 |
| `_.take(n)` | `views::take(n)` | 처음 n개 선택 |
| `_.drop(n)` | `views::drop(n)` | 처음 n개 건너뛰기 |
| `_.flatten()` | `views::join` | 중첩 컨테이너 평탄화 |
| `_.reverse()` | `views::reverse` | 역순 |
| `_.find(pred)` | `ranges::find_if(pred)` | 조건에 맞는 첫 요소 |
| `_.every(pred)` | `ranges::all_of(pred)` | 모든 요소가 조건 만족 |
| `_.some(pred)` | `ranges::any_of(pred)` | 하나라도 조건 만족 |
| `_.sortBy(key)` | `ranges::sort` + 비교자 | 정렬 |
| `_.groupBy(key)` | `views::chunk_by(pred)` | 그룹화 |
| `_.zip(arr)` | `views::zip(range)` | 여러 배열 결합 |
| `_.uniq()` | `ranges::unique` | 중복 제거 |

---

## 지연 평가 (Lazy Evaluation) 비교

### JavaScript lodash - 즉시 평가

```javascript
const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

// 즉시 평가: 중간 배열이 매번 생성됨
const result = _.chain(numbers)
    .filter(n => n % 2 === 0)  // [2, 4, 6, 8, 10] 생성
    .map(n => n * 2)           // [4, 8, 12, 16, 20] 생성
    .take(3)                   // [4, 8, 12] 생성
    .value();

console.log(result);  // [4, 8, 12]
```

### C++23 ranges - 지연 평가

```cpp
#include <ranges>
#include <vector>
#include <iostream>

std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 지연 평가: 중간 컨테이너 생성 없음
auto result = numbers
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * 2; })
    | std::views::take(3);

// 실제 연산은 여기서 발생 (순회 시)
for (int n : result)
{
    std::cout << n << " ";  // 4 8 12
}
```

**핵심 차이**: C++ views는 파이프라인을 구성할 때 연산하지 않고, 순회할 때만 필요한 요소를 계산한다.

---

## 체이닝 패턴 비교

### JavaScript lodash

```javascript
// 방법 1: _.chain() 사용
const result1 = _.chain(users)
    .filter(u => u.active)
    .map(u => u.name)
    .sortBy()
    .value();

// 방법 2: _.flow() 사용 (함수 합성)
const processUsers = _.flow([
    arr => _.filter(arr, u => u.active),
    arr => _.map(arr, u => u.name),
    arr => _.sortBy(arr)
]);
const result2 = processUsers(users);

// 방법 3: 개별 호출
const result3 = _.sortBy(
    _.map(
        _.filter(users, u => u.active),
        u => u.name
    )
);
```

### C++23 ranges

```cpp
// 파이프 연산자로 체이닝 (왼쪽에서 오른쪽으로 읽기 쉬움)
auto result = users
    | std::views::filter([](const User& u) { return u.active; })
    | std::views::transform([](const User& u) { return u.name; });

// 정렬은 action이므로 별도 처리
std::vector<std::string> sorted(result.begin(), result.end());
std::ranges::sort(sorted);
```

---

## 실전 예시 비교

### 예시 1: 짝수 필터링 후 제곱

**JavaScript:**
```javascript
const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

const result = _.chain(numbers)
    .filter(n => n % 2 === 0)
    .map(n => n * n)
    .value();
// [4, 16, 36, 64, 100]
```

**C++23:**
```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

auto result = numbers
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; });
// 4, 16, 36, 64, 100
```

### 예시 2: 객체 배열에서 특정 필드 추출

**JavaScript:**
```javascript
const users = [
    { id: 1, name: 'Alice', score: 85 },
    { id: 2, name: 'Bob', score: 92 },
    { id: 3, name: 'Charlie', score: 78 }
];

const names = _.map(users, 'name');
// ['Alice', 'Bob', 'Charlie']

const highScorers = _.chain(users)
    .filter(u => u.score >= 80)
    .map('name')
    .value();
// ['Alice', 'Bob']
```

**C++23:**
```cpp
struct User
{
    int id;
    std::string name;
    int score;
};

std::vector<User> users = {
    {1, "Alice", 85},
    {2, "Bob", 92},
    {3, "Charlie", 78}
};

auto names = users
    | std::views::transform(&User::name);
// "Alice", "Bob", "Charlie"

auto highScorers = users
    | std::views::filter([](const User& u) { return u.score >= 80; })
    | std::views::transform(&User::name);
// "Alice", "Bob"
```

### 예시 3: 합계 계산 (Reduce/Fold)

**JavaScript:**
```javascript
const numbers = [1, 2, 3, 4, 5];

const sum = _.reduce(numbers, (acc, n) => acc + n, 0);
// 15

const product = _.reduce(numbers, (acc, n) => acc * n, 1);
// 120
```

**C++23:**
```cpp
#include <numeric>  // fold_left는 <algorithm>에 있음

std::vector<int> numbers = {1, 2, 3, 4, 5};

int sum = std::ranges::fold_left(numbers, 0, std::plus<>{});
// 15

int product = std::ranges::fold_left(numbers, 1, std::multiplies<>{});
// 120
```

### 예시 4: 그룹화

**JavaScript:**
```javascript
const items = [
    { type: 'fruit', name: 'apple' },
    { type: 'fruit', name: 'banana' },
    { type: 'vegetable', name: 'carrot' }
];

const grouped = _.groupBy(items, 'type');
// {
//   fruit: [{type: 'fruit', name: 'apple'}, {type: 'fruit', name: 'banana'}],
//   vegetable: [{type: 'vegetable', name: 'carrot'}]
// }
```

**C++23:**
```cpp
// C++23 chunk_by는 연속된 동일 키 그룹화 (정렬 필요)
auto sorted = items;
std::ranges::sort(sorted, {}, &Item::type);

auto grouped = sorted | std::views::chunk_by(
    [](const Item& a, const Item& b) { return a.type == b.type; }
);

// 또는 수동 그룹화
std::map<std::string, std::vector<Item>> groupedMap;
for (const auto& item : items)
{
    groupedMap[item.type].push_back(item);
}
```

---

## 성능 비교

| 항목 | JavaScript lodash | C++23 ranges |
|------|-------------------|--------------|
| **중간 컨테이너** | 매 단계마다 생성 | 없음 (지연 평가) |
| **함수 호출** | 런타임 디스패치 | 인라인화 가능 |
| **메모리 할당** | GC 의존 | RAII / 스택 할당 |
| **분기 예측** | 어려움 | 컴파일러 최적화 |

### 벤치마크 예시 (100만 요소)

```
작업: filter(even) -> map(square) -> take(1000)

lodash (Node.js):     ~45ms
C++23 ranges:         ~0.3ms

속도 차이: ~150배
```

> **주의**: 실제 성능은 데이터 크기, 연산 복잡도, 하드웨어에 따라 달라집니다.

---

## C++23 ranges 고급 기능

### views::zip - 여러 컨테이너 결합

```cpp
std::vector<int> ids = {1, 2, 3};
std::vector<std::string> names = {"Alice", "Bob", "Charlie"};

for (auto [id, name] : std::views::zip(ids, names))
{
    std::cout << id << ": " << name << "\n";
}
// 1: Alice
// 2: Bob
// 3: Charlie
```

**JavaScript 대응:**
```javascript
_.zip([1, 2, 3], ['Alice', 'Bob', 'Charlie']);
// [[1, 'Alice'], [2, 'Bob'], [3, 'Charlie']]
```

### views::enumerate - 인덱스 포함

```cpp
std::vector<std::string> fruits = {"apple", "banana", "cherry"};

for (auto [idx, fruit] : std::views::enumerate(fruits))
{
    std::cout << idx << ": " << fruit << "\n";
}
// 0: apple
// 1: banana
// 2: cherry
```

**JavaScript 대응:**
```javascript
fruits.forEach((fruit, idx) => console.log(`${idx}: ${fruit}`));
// 또는
_.map(fruits, (fruit, idx) => [idx, fruit]);
```

### views::chunk - 청크 분할

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

for (auto chunk : numbers | std::views::chunk(3))
{
    // chunk: [1,2,3], [4,5,6], [7,8,9], [10]
}
```

**JavaScript 대응:**
```javascript
_.chunk([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 3);
// [[1, 2, 3], [4, 5, 6], [7, 8, 9], [10]]
```

### views::slide - 슬라이딩 윈도우

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5};

for (auto window : numbers | std::views::slide(3))
{
    // window: [1,2,3], [2,3,4], [3,4,5]
}
```

**JavaScript 대응 (직접 구현):**
```javascript
function* sliding(arr, size) {
    for (let i = 0; i <= arr.length - size; i++) {
        yield arr.slice(i, i + size);
    }
}
```

---

## 선택 기준

### lodash가 적합한 경우
- **빠른 프로토타이핑**: 타입 정의 없이 바로 사용
- **웹 프론트엔드**: 브라우저 환경
- **Node.js 서버**: I/O 바운드 작업 위주
- **동적 데이터 구조**: 스키마가 유동적인 경우

### C++23 ranges가 적합한 경우
- **성능 크리티컬**: 게임 서버, 시뮬레이션
- **대용량 데이터**: 수백만 요소 처리
- **컴파일타임 안전성**: 타입 에러 조기 발견
- **메모리 효율**: 지연 평가로 중간 할당 제거

---

## 마이그레이션 팁

### lodash -> C++ ranges

1. **`_.chain()`** -> 파이프 `|` 연산자로 변환
2. **동적 속성 접근** `_.map(arr, 'field')` -> 멤버 포인터 `&Type::field`
3. **`_.groupBy()`** -> 정렬 후 `chunk_by` 또는 수동 맵 구성
4. **`_.reduce()`** -> `ranges::fold_left`
5. **`undefined`/`null` 처리** -> `std::optional` 사용

### 주의사항

```cpp
// [BAD] 잘못된 사용: dangling reference
auto getFiltered()
{
    std::vector<int> temp = {1, 2, 3, 4, 5};
    return temp | std::views::filter([](int n) { return n % 2 == 0; });
    // temp가 소멸되어 dangling!
}

// [GOOD] 올바른 사용: 컨테이너 수명 보장
std::vector<int> numbers = {1, 2, 3, 4, 5};
auto filtered = numbers | std::views::filter([](int n) { return n % 2 == 0; });
// numbers가 살아있는 동안 filtered 사용 가능
```

---

## 참고 자료

- [C++ Reference - Ranges](https://en.cppreference.com/w/cpp/ranges)
- [Lodash Documentation](https://lodash.com/docs)
- [C++20/23 Ranges - A Gentle Introduction](https://www.cppstories.com/2022/ranges-cpp23/)
