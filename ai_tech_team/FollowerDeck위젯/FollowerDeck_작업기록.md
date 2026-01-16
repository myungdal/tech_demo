# FollowerDeck 위젯 구현 - 작업 기록

> **작업 기간**: 2026-01-15
> **상태**: 진행 중

---

## 목표

던전 플레이 화면 하단에 소유한 NPC Follower 목록을 표시하는 UI 구현

---

## 작업 진행 내역

### 1. C++ 기반 코드 구현 ✅

| 파일 | 설명 | 상태 |
|------|------|:---:|
| `FollowerSlotData.h` | Follower 슬롯 데이터 구조체 | ✅ |
| `FollowerSlotWidgetBase.h/cpp` | 개별 슬롯 위젯 | ✅ |
| `FollowerDeckWidgetBase.h/cpp` | 덱 컨테이너 위젯 | ✅ |
| `DungeonPlayViewportWidget.h/cpp` | 뷰포트에 덱 추가 | ✅ |

### 2. Python 원격 실행 도구 구축 ✅

| 파일 | 설명 |
|------|------|
| `tools/ue_remote.py` | 언리얼 에디터 Python 원격 실행 |
| `tools/test_widget_inspector.py` | 위젯 구조 검사 |

### 3. WidgetInspectorLibrary (에디터 전용) ✅

| 파일 | 설명 |
|------|------|
| `WidgetInspectorLibrary.h/cpp` | Python에서 WidgetBlueprint 검사용 |

---

## 시행착오 및 해결책

### 1. BindWidget vs 동적 생성

**문제**: 
- C++에서 `UPROPERTY(meta = (BindWidget))`로 선언하면 블루프린트 에디터에서 수동으로 위젯을 추가해야 함
- Python API로는 WidgetBlueprint의 WidgetTree를 수정할 수 없음

**시도한 방법**:
1. ❌ Python으로 WidgetTree 접근 → `widget_tree` 속성이 노출되지 않음
2. ❌ Python으로 위젯 추가 → API 미지원
3. ✅ **C++ NativePreConstruct에서 동적 생성** → 블루프린트 수정 불필요

**해결책**:
```cpp
// BindWidget 제거
UPROPERTY()  // meta = (BindWidget) 제거
UHorizontalBox* mSlotContainer = nullptr;

// NativePreConstruct에서 동적 생성
void UFollowerDeckWidgetBase::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (mSlotContainer) return;  // 이미 생성됨
    
    mSlotContainer = NewObject<UHorizontalBox>(this);
    // ... 위젯 트리 구성
    WidgetTree->RootWidget = mainBorder;
}
```

### 2. UWidgetBlueprint Python 노출 불가

**문제**:
- `UWidgetBlueprint`는 에디터 전용 클래스
- UFUNCTION 파라미터로 직접 노출 불가

**해결책**:
- FString 에셋 경로로 받아서 내부에서 `LoadObject` 사용
- `#if WITH_EDITOR` 가드로 에디터 빌드에서만 작동

```cpp
// ❌ 불가능
UFUNCTION() static UWidget* GetRoot(UWidgetBlueprint* BP);

// ✅ 가능
UFUNCTION() static FString GetHierarchy(const FString& AssetPath);
```

### 3. Live Coding 빌드 차단

**문제**:
- 에디터 실행 중 빌드 시 "Live Coding is active" 에러
- 에디터 종료 후에도 잠금이 풀리지 않음

**원인**:
- 언리얼 프로세스가 완전히 종료되지 않음
- Named Mutex 또는 Live Coding 서버가 남아있음

**해결책**:
```powershell
# PowerShell로 강제 종료
Get-Process UnrealEditor -ErrorAction SilentlyContinue | Stop-Process -Force

# 설정 파일 삭제 (필요시)
rm -rf "client/Client/Saved/Config"
rm -rf "$LOCALAPPDATA/UnrealEngine/5.6/Saved/Config"
```

### 4. mSlotWidgetClass 미설정

**문제**:
- `FollowerDeckWidgetBase`에서 슬롯 생성 시 `mSlotWidgetClass`가 null
- 블루프린트 에디터에서 수동 설정 필요했음

**해결책**:
- C++ 코드에서 자동 로드하도록 변경

```cpp
if (!mSlotWidgetClass)
{
    mSlotWidgetClass = LoadClass<UFollowerSlotWidgetBase>(
        nullptr,
        TEXT("/Game/Bp/FollowerDeck/BpFollowerSlotWidget.BpFollowerSlotWidget_C")
    );
}
```

### 5. 변수명 충돌 (Slot)

**문제**:
```
error C4458: 'Slot' 선언은 클래스 멤버를 숨깁니다.
```
- `UWidget::Slot` 멤버와 로컬 변수 `Slot` 충돌

**해결책**:
```cpp
// ❌ 충돌
UCanvasPanelSlot* Slot = ...;

// ✅ 해결
UCanvasPanelSlot* FollowerSlot = ...;
```

---

## 위젯 구조 (최종)

```
DungeonPlayViewportWidget (NativeConstruct에서 동적 생성)
└── mFollowerDeck (BpFollowerDeckWidget)
    └── Border (배경, 반투명 검정)
        └── VerticalBox
            ├── mSlotContainer (HorizontalBox)
            │   └── [FollowerSlotWidget x N] (동적 생성)
            │       └── mSlotBorder (Border)
            │           └── VerticalBox
            │               ├── mCharacterIcon (Image, 48x48)
            │               ├── mHpBar (ProgressBar, 48x6)
            │               └── mIndexText (TextBlock)
            └── mCountText (TextBlock) "0 / 10"
```

---

## 빌드 명령어

```bash
# 클라이언트 빌드 (에디터 종료 필수!)
"C:/Program Files/Epic Games/UE_5.6/Engine/Build/BatchFiles/Build.bat" \
  ClientEditor Win64 Development \
  "C:/dev/nearest3/client/Client/Client.uproject" -waitmutex

# 에디터 강제 종료 (빌드 전)
powershell -Command "Get-Process UnrealEditor -ErrorAction SilentlyContinue | Stop-Process -Force"
```

---

## 관련 파일 경로

### C++ 코드
```
client/Client/Source/Client/Private/Widget/FollowerDeck/
├── FollowerSlotData.h
├── FollowerSlotWidgetBase.h
├── FollowerSlotWidgetBase.cpp
├── FollowerDeckWidgetBase.h
└── FollowerDeckWidgetBase.cpp

client/Client/Source/Client/Private/Widget/Viewport/
├── DungeonPlayViewportWidget.h
└── DungeonPlayViewportWidget.cpp

client/Client/Source/Client/Private/Util/WidgetInspector/
├── WidgetInspectorLibrary.h
└── WidgetInspectorLibrary.cpp
```

### 블루프린트
```
client/Client/Content/Bp/FollowerDeck/
├── BpFollowerDeckWidget.uasset
└── BpFollowerSlotWidget.uasset
```

### Python 도구
```
tools/
├── ue_remote.py
├── test_widget_inspector.py
├── create_follower_deck_bp.py
└── setup_follower_widgets.py
```

---

## 남은 작업

- [ ] 에디터에서 FollowerDeck 표시 확인 (로그 추가 완료, 테스트 필요)
- [ ] 게임 플레이 시 Follower 데이터 연동 테스트
- [ ] 슬롯 클릭 시 Follower 선택 기능
- [ ] 캐릭터 아이콘 로드 (CharacterSid → 텍스처)

## 2026-01-15 추가 작업

### 디버그 로그 추가

`DungeonPlayViewportWidget::NativeConstruct()`에 상세 로그 추가:

```cpp
// 블루프린트 로드 결과
UE_LOG(LogTemp, Warning, TEXT("[FollowerDeck] NativeConstruct - FollowerDeckClass: %s"), ...);

// CreateWidget 결과
UE_LOG(LogTemp, Warning, TEXT("[FollowerDeck] CreateWidget result: %s"), ...);

// RootWidget 타입 확인
UE_LOG(LogTemp, Warning, TEXT("[FollowerDeck] RootWidget: %s (Type: %s)"), ...);

// CanvasPanel 추가 결과
UE_LOG(LogTemp, Warning/Error, TEXT("[FollowerDeck] Added to CanvasPanel successfully/Failed!"));
```

### null check 추가

`UpdateFollowerDeck()`에서 `mFollowerDeck` null 체크 추가하여 크래시 방지.

### 테스트 방법

1. 에디터 실행
2. 던전 플레이 진입
3. Output Log에서 `[FollowerDeck]` 검색
4. 로그 확인:
   - `FollowerDeckClass: BpFollowerDeckWidget_C` → 블루프린트 로드 성공
   - `CreateWidget result: Success` → 위젯 생성 성공
   - `RootWidget: XXX (Type: CanvasPanel)` → 루트가 CanvasPanel이어야 추가 가능
   - `Added to CanvasPanel successfully!` → 최종 성공

---

## 교훈

1. **BindWidget은 블루프린트 작업 필요** → 순수 C++ 구현이 필요하면 동적 생성 사용
2. **Python으로 WidgetTree 수정 불가** → C++ NativePreConstruct 활용
3. **Live Coding이 빌드 차단** → 에디터 완전 종료 필수, PowerShell 강제 종료 사용
4. **에디터 전용 클래스 주의** → `#if WITH_EDITOR`, 에셋 경로로 간접 접근
