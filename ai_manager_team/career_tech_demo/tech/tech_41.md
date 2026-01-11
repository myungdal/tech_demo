# 41. DEBUG_VAR_DEF - X-Macro 기반 디버그 변수 시스템

작성자: 안명달 (mooondal@gmail.com)

## 개요

매크로를 많이 사용한 프로젝트에서 고생을 너무 많이 하여 매크로를 최대한 배제하려고 했지만, 이 부분은 **편의가 중요**하기 때문에 매크로를 사용할 수밖에 없었다.

게임 개발 시 **수백 개의 튜닝 파라미터**를 관리해야 한다. 물리 상수, 렌더링 옵션, AI 파라미터 등 빈번하게 조정되는 값들의 효율적 관리가 필요하다.

일반적인 접근법:
- **하드코딩**: 수정할 때마다 재컴파일 필요
- **외부 파일(JSON/XML)**: 파싱 코드 필요, 타입 안전성 없음
- **에디터 통합**: 별도 UI 개발 필요

이 프로젝트에서는 **X-Macro 패턴**을 활용하여 **코드 = 데이터**인 시스템을 구현했다.

## 핵심 아이디어

> **초기화 코드와 동적 파일 로딩을 동일한 형식으로 맞춘다**

```cpp
// 이 한 줄이 선언, 정의, 런타임 등록을 모두 수행
DEBUG_VAR_DEF(PHYSICS_GROUND_VELOCITY, int64_t, 15)
```

- 컴파일 시: 변수 선언 + 초기값 정의
- 런타임: 이름-값 맵에 자동 등록
- 에디터: 맵을 순회하며 UI 자동 생성

## 왜 매크로를 사용했는가

> **매크로 사용을 지양하는 것이 일반적인 C++ 모범 사례이지만,  
> 이 경우는 매크로를 사용할 수밖에 없는 대표적인 예외 케이스이다.**

### 매크로가 필수인 이유

1. **Stringification (#)**: 변수명을 문자열로 변환
   ```cpp
   #define DEBUG_INFO_WSTR(x) L#x  // PHYSICS_GROUND_VELOCITY -> L"PHYSICS_GROUND_VELOCITY"
   ```
   템플릿이나 constexpr로는 불가능

2. **Token Pasting (##)**: 식별자 조합
   ```cpp
   #define DEBUG_VAR_DEF(name, type, value) static type name;
   ```

3. **X-Macro 패턴**: 하나의 정의로 여러 컨텍스트에서 다르게 확장
   ```cpp
   // 선언용 (헤더)
   #define DEBUG_VAR_DEF(name, type, value) static type name;
   #include "DebugInfoDef.inl"
   #undef DEBUG_VAR_DEF
   
   // 정의용 (cpp)
   #define DEBUG_VAR_DEF(name, type, value) type DebugInfo::name = value;
   #include "DebugInfoDef.inl"
   #undef DEBUG_VAR_DEF
   
   // 등록용 (런타임)
   #define DEBUG_VAR_DEF(name, type, value) \
       sVarMap[L#name] = &name; \
       sTypeMap[L#name] = L#type;
   #include "DebugInfoDef.inl"
   #undef DEBUG_VAR_DEF
   ```

### 대안과 한계

| 대안 | 한계 |
|------|------|
| 템플릿 | Stringification 불가, 변수명->문자열 변환 불가 |
| constexpr | 런타임 등록 로직 생성 불가 |
| 코드 생성기 | 빌드 복잡도 증가, 추가 도구 필요 |
| 리플렉션 | C++ 표준에 없음 (C++26 예정) |

## 구현 상세

### 파일 구조

```
MmoSync/
├── Public/
│   └── DebugControlTool/
│       ├── DebugInfo.h         # 클래스 선언 + 변수 선언
│       └── DebugInfoDef.inl    # 변수 정의 (유일한 수정 지점)
└── Private/
    └── DebugControlTool/
        └── DebugInfo.cpp       # 변수 정의 + 런타임 등록
```

### DebugInfoDef.inl - 단일 진실의 원천

```cpp
// 디버그 설정
DEBUG_VAR_DEF(DEBUG_ENTITY_ID, EntityId, -1)
DEBUG_VAR_DEF(DEBUG_DAMAGE, StatValue, -1)

// 렌더링 옵션
DEBUG_VAR_DEF(RENDER_TILE, int, 1)
DEBUG_VAR_DEF(RENDER_CHARACTER_MODE, int, 1)

// 물리 시스템
DEBUG_VAR_DEF(PHYSICS_GROUND_VELOCITY, int64_t, 15)
DEBUG_VAR_DEF(PHYSICS_GROUND_FRICTION, int64_t, 5)

// 상수 표현식도 사용 가능
DEBUG_VAR_DEF(MOVE_TO_DISTANCE, int64_t, (1000 << CELL_SIZE_DESIGN_FACTOR))
```

### DebugInfo.h - 변수 선언 확장

```cpp
struct MMOSYNC_API DebugInfo
{
    // 타입별 Setter
    using SetterFunc = std::function<void(void*, int64_t)>;
    
    // 런타임 맵
    static std::unordered_map<std::wstring, void*> sVarMap;      // 이름 -> 주소
    static std::unordered_map<std::wstring, std::wstring> sTypeMap;  // 이름 -> 타입명
    static std::unordered_map<std::wstring, SetterFunc> sSetterMap;  // 타입명 -> Setter
    
    // 변수 선언 (X-Macro 확장)
    #define DEBUG_VAR_DEF(name, type, value) static type name;
    #include "MmoSync/DebugControlTool/DebugInfoDef.inl"
    #undef DEBUG_VAR_DEF
    
    static void InitVarMap();
    void LoadDebugInfoVar(const wchar_t* key, int64_t value);
};
```

### DebugInfo.cpp - 정의 및 등록

```cpp
// Wide string 매크로 헬퍼
#define DEBUG_INFO_WIDEN2(x) L ## x
#define DEBUG_INFO_WIDEN(x) DEBUG_INFO_WIDEN2(x)
#define DEBUG_INFO_WSTR(x) DEBUG_INFO_WIDEN(#x)

// 변수 정의 (X-Macro 확장)
#define DEBUG_VAR_DEF(name, type, value) type DebugInfo::name = value;
#include "MmoSync/DebugControlTool/DebugInfoDef.inl"
#undef DEBUG_VAR_DEF

void DebugInfo::InitVarMap()
{
    if (sInitialized) return;
    sInitialized = true;
    
    // 타입별 Setter 등록
    sSetterMap[L"int"] = [](void* ptr, int64_t value) { 
        *static_cast<int*>(ptr) = static_cast<int>(value); 
    };
    sSetterMap[L"int64_t"] = [](void* ptr, int64_t value) { 
        *static_cast<int64_t*>(ptr) = value; 
    };
    sSetterMap[L"Scalar"] = [](void* ptr, int64_t value) { 
        *static_cast<Scalar*>(ptr) = value; 
    };
    // ... 기타 타입
    
    // 변수 등록 (X-Macro 확장)
    #define DEBUG_VAR_DEF(name, type, value) \
        sVarMap[DEBUG_INFO_WSTR(name)] = &name; \
        sTypeMap[DEBUG_INFO_WSTR(name)] = DEBUG_INFO_WSTR(type);
    #include "MmoSync/DebugControlTool/DebugInfoDef.inl"
    #undef DEBUG_VAR_DEF
}
```

## 사용 패턴

### 1. 직접 접근 (컴파일 타임)

```cpp
// 일반 static 변수처럼 사용
if (DebugInfo::RENDER_TILE)
    RenderTiles();

int64_t velocity = DebugInfo::PHYSICS_GROUND_VELOCITY;
```

### 2. 런타임 수정 (에디터/콘솔)

```cpp
// 이름으로 값 변경
gDebugInfo.LoadDebugInfoVar(L"PHYSICS_GROUND_VELOCITY", 30);
gDebugInfo.LoadDebugInfoVar(L"RENDER_TILE", 0);
```

### 3. 에디터 UI 자동 생성

```cpp
// 모든 변수 순회
for (const auto& [name, ptr] : DebugInfo::sVarMap)
{
    const auto& typeName = DebugInfo::sTypeMap[name];
    CreateSliderUI(name, typeName, ptr);
}
```

## 장점

### 1. DRY 원칙 준수

변수 추가 시 `.inl` 파일에 **한 줄만** 추가:

```cpp
DEBUG_VAR_DEF(NEW_VARIABLE, int, 42)
```

자동으로:
- 헤더에 선언 추가
- cpp에 정의 추가
- 런타임 맵에 등록
- 에디터에서 접근 가능

### 2. 빠른 재컴파일

값 변경 시 `.inl`만 수정하면 `.cpp`만 재컴파일된다.  
헤더 의존성이 없어 증분 빌드가 빠릅니다.

### 3. 타입 안전성

```cpp
// 컴파일 타임에 타입 체크
int vel = DebugInfo::PHYSICS_GROUND_VELOCITY;  // int64_t -> int 경고
```

### 4. IDE 지원

일반 static 변수이므로:
- 자동완성 작동
- Find References 작동
- 리팩토링 도구 작동

## 실제 변수 카테고리

현재 **80개 이상**의 튜닝 변수가 등록되어 있다:

| 카테고리 | 예시 |
|----------|------|
| 디버그 설정 | DEBUG_ENTITY_ID, DEBUG_DAMAGE |
| 렌더링 옵션 | RENDER_TILE, RENDER_CHARACTER_MODE |
| 물리 시스템 | PHYSICS_GROUND_VELOCITY, PHYSICS_AIR_FRICTION |
| 이동 명령 | MOVE_TO_ARRIVAL_RANGE, MOVE_TO_ASTAR_STEP |
| 진형 시스템 | CROWD_FORMATION_GAP, FLOCK_ORBIT_RADIUS |
| 전술 AI | TACTICS_COMBAT_RANGE, DETECTION_DISTANCE |
| 스포너 | SPAWNER_MAX_COUNT, SPAWNER_COST_TIME |
| 맵 생성 | DEFAULT_MAP_CASTLE_CNT, DEFAULT_MAP_NEST_CNT |

## 상수 표현식 지원

값에 상수 표현식을 사용할 수 있다:

```cpp
// 셀 크기 팩터 적용
DEBUG_VAR_DEF(MOVE_DISTANCE, int64_t, (1000 << CELL_SIZE_DESIGN_FACTOR))

// 시간 팩터 적용
DEBUG_VAR_DEF(MOTION_DURATION, Span, (1600 >> TIME_STEP_INTERVAL_FACTOR))
```

런타임 파싱을 위해 알려진 상수 맵도 제공:

```cpp
static std::unordered_map<std::wstring, int64_t> sKnownConstants = {
    { L"CELL_SIZE_DESIGN_FACTOR", static_cast<int64_t>(CELL_SIZE_DESIGN_FACTOR) },
    { L"TIME_STEP_INTERVAL_FACTOR", static_cast<int64_t>(TIME_STEP_INTERVAL_FACTOR) }
};
```

## 모듈 경계 처리 (DLL)

언리얼 엔진 환경에서는 모듈(DLL) 간 static 변수 공유 이슈가 있다:

```cpp
// 헤더
#if defined(__UNREAL__)
MMOSYNC_API extern DebugInfo gDebugInfo;  // DLL export
#else
inline DebugInfo gDebugInfo;               // 인라인 (서버용)
#endif

// cpp
#if defined(__UNREAL__)
DebugInfo gDebugInfo;  // DLL에서 정의
#endif
```

## 정리

| 특징 | 설명 |
|------|------|
| 패턴 | X-Macro (Token Pasting + Stringification) |
| 단일 진실 | `.inl` 파일 한 곳에서 관리 |
| 타입 안전 | 일반 static 변수로 컴파일 타임 체크 |
| 런타임 접근 | 이름->주소 맵으로 동적 수정 |
| 빌드 최적화 | 값 변경 시 cpp만 재컴파일 |
| 매크로 사용 이유 | Stringification은 템플릿으로 대체 불가 |

---
