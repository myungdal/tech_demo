# AI Art Team - UI 파트 인수인계 문서

> **이 문서만 읽으면 UI 아트 작업을 바로 시작할 수 있습니다.**

---

## 1. 프로젝트 개요

| 항목 | 내용 |
|------|------|
| 게임 | Nearest3 - SLG 워게임 / MMO |
| 스타일 | 16-bit 레트로 픽셀 아트 |
| 엔진 | Unreal Engine 5.6 |
| 해상도 | 1920×1080 기준 (UI 스케일 80~120% 지원) |

---

## 2. 폴더 구조

```
ai_art_team/
├── 아트팀UI파트.md          ← 현재 문서 (인수인계용)
├── ui/                       ← UI 아트 작업 폴더
│   ├── README.md            ← UI 작업 상세 가이드
│   ├── hud/                 ← HUD 요소 (체력바, 버프 아이콘 등)
│   ├── minimap/             ← 미니맵 관련
│   ├── cards/               ← 카드 핸드, 카드 프레임
│   ├── buttons/             ← 버튼 (액션, 시스템 메뉴)
│   ├── icons/               ← 아이콘 (시스템, 상태, 재화)
│   ├── popup/               ← 팝업 UI (결과창, 알림)
│   ├── chat/                ← 채팅창 UI
│   └── frames/              ← 공통 프레임, 테두리
├── character/               ← 캐릭터 스프라이트 (별도 파트)
├── bg/                      ← 배경 (별도 파트)
├── effect/                  ← 이펙트 (별도 파트)
└── scripts/                 ← 이미지 처리 스크립트
```

---

## 3. UI 기획 문서 참조

**필수 참조**: `ai_design_team/90_공통/02_UI_및_시스템/02_UI_구성.md`

### 3.1 화면 레이아웃 요약

```
┌─────────────────────────────────────────────────────────┐
│ [미니맵]                                    [시스템 메뉴] │  ← 상단
│                                                          │
│                                                          │
│                    [게임 화면]                            │  ← 중앙 (시야 확보)
│                                                          │
│                                                          │
│ [채팅창]                                    [액션 버튼]   │  ← 하단
│                     [카드 핸드]                          │
└─────────────────────────────────────────────────────────┘
```

### 3.2 주요 UI 요소 목록

| 위치 | 요소 | 크기(기본) | 우선순위 |
|------|------|-----------|:--------:|
| 좌상단 | 미니맵 | 150×150px | 1 |
| 우상단 | 시스템 메뉴 버튼 (6개) | 48×48px 각 | 2 |
| 좌하단 | 채팅창 | 300×150px | 3 |
| 하단 중앙 | 카드 핸드 (4슬롯) | 80×120px 각 | 1 |
| 우하단 | 메인 액션 버튼 | 120×60px | 1 |
| 우하단 | 보조 버튼 (3개) | 48×48px 각 | 2 |
| HUD | 체력바 | 64×8px | 1 |
| HUD | 버프/디버프 아이콘 | 24×24px | 2 |

---

## 4. 스타일 가이드

### 4.1 필수 규칙

| 규칙 | 설명 |
|------|------|
| 픽셀 아트 | 16-bit 레트로 스타일, 하드 엣지 |
| 외곽선 | 뚜렷한 검은색 또는 진한 색상 |
| 채도 | 높은 채도, 명확한 색상 구분 |
| 배경 | PNG 투명 배경 (Alpha) |
| 안티앨리어싱 | 없음 (하드 픽셀만) |

### 4.2 컬러 팔레트

| 용도 | 색상 | Hex |
|------|------|-----|
| 아군 | 파란색 | #4A90D9 |
| 동맹 | 녹색 | #5CB85C |
| 적군 | 빨간색 | #D9534F |
| NPC | 노란색 | #F0AD4E |
| 비활성 | 회색 | #777777 |
| 강조 | 금색 | #FFD700 |
| 경고 | 주황색 | #FF8C00 |

### 4.3 색약 모드 대응

| 모드 | 변경 |
|------|------|
| 적녹색약 | 빨강→주황, 녹색→하늘색 |
| 청황색약 | 파랑→보라, 노랑→분홍 |
| 전색맹 | 형태로 구분 (원/삼각/사각) |

---

## 5. 작업 우선순위

### Phase 1: 핵심 HUD (필수)
1. **카드 핸드 프레임** - 4슬롯, 쿨다운 오버레이
2. **메인 액션 버튼** - 소환/귀환/공성 상태별 4종
3. **체력바** - 아군/적군/동맹/NPC 4색

### Phase 2: 시스템 UI
4. **미니맵 프레임** - 모서리, 줌 버튼
5. **시스템 메뉴 아이콘** - 설정, 우편, 랭킹, 클랜, 인벤, 덱 (6종)
6. **보조 버튼** - 내 유닛, 타겟 고정, 전투 정보 (3종)

### Phase 3: 상태 표시
7. **버프/디버프 아이콘** - 기본 세트 (최소 12종)
8. **알림 배지** - 숫자, 점, 빛나는 효과
9. **IP 게이지 바** - 개입 포인트 표시

### Phase 4: 팝업
10. **결과 화면 프레임** - 귀환 성공 보상
11. **공성 알림 프레임** - 경고 팝업
12. **채팅창 프레임** - 탭, 입력창

---

## 6. 파일 명명 규칙

```
{카테고리}_{요소명}_{상태}_{크기}.png

예시:
btn_action_summon_normal_120x60.png
btn_action_summon_pressed_120x60.png
btn_action_return_normal_120x60.png
icon_system_settings_48x48.png
icon_buff_attack_up_24x24.png
frame_card_normal_80x120.png
frame_card_cooldown_80x120.png
hud_healthbar_ally_64x8.png
```

---

## 7. 작업 흐름

```
1. 기획 문서 확인
   └─ ai_design_team/90_공통/02_UI_및_시스템/02_UI_구성.md

2. 해당 폴더에 작업
   └─ ai_art_team/ui/{카테고리}/

3. AI 이미지 생성 (필요시)
   ├─ 프롬프트에 스타일 가이드 포함
   └─ 픽셀 아트, 투명 배경, 하드 엣지 명시

4. 품질 확인
   ├─ 크기 정확한가?
   ├─ 배경 투명한가?
   ├─ 스타일 일관적인가?
   └─ 파일명 규칙 준수?

5. 저장 및 기록
   └─ 해당 폴더의 README.md에 작업 현황 업데이트
```

---

## 8. AI 프롬프트 템플릿

### 8.1 버튼 생성

```
Create a pixel art game UI button.
- Size: {WIDTH}x{HEIGHT} pixels
- Style: 16-bit retro pixel art
- Hard pixel edges, NO anti-aliasing
- Transparent PNG background
- Clear dark outline
- High saturation colors

Button type: {ACTION/SYSTEM/TOGGLE}
State: {NORMAL/HOVER/PRESSED/DISABLED}
Icon: {DESCRIPTION}
Color scheme: {PRIMARY COLOR}
```

### 8.2 아이콘 생성

```
Create a pixel art game icon.
- Size: {SIZE}x{SIZE} pixels (square)
- Style: 16-bit retro pixel art
- Transparent PNG background
- Clear silhouette, readable at small size
- NO text, symbol only

Icon represents: {DESCRIPTION}
Color: {PRIMARY COLOR}
```

### 8.3 프레임/패널 생성

```
Create a pixel art UI frame/panel.
- Size: {WIDTH}x{HEIGHT} pixels
- Style: 16-bit retro pixel art
- Transparent center (content area)
- Decorative border only
- Dark outline, beveled edges optional

Purpose: {CARD SLOT/POPUP/MINIMAP BORDER}
Theme: {FANTASY/MEDIEVAL/NEUTRAL}
```

---

## 9. 작업 현황

### ui/ 폴더 생성 상태

| 폴더 | 상태 | 파일 수 |
|------|:----:|:-------:|
| hud/ | 미생성 | 0 |
| minimap/ | 미생성 | 0 |
| cards/ | 미생성 | 0 |
| buttons/ | 미생성 | 0 |
| icons/ | 미생성 | 0 |
| popup/ | 미생성 | 0 |
| chat/ | 미생성 | 0 |
| frames/ | 미생성 | 0 |

### 개별 에셋 완료율

| Phase | 항목 | 상태 |
|:-----:|------|:----:|
| 1 | 카드 핸드 프레임 | ⬜ |
| 1 | 메인 액션 버튼 (4종) | ⬜ |
| 1 | 체력바 (4색) | ⬜ |
| 2 | 미니맵 프레임 | ⬜ |
| 2 | 시스템 메뉴 아이콘 (6종) | ⬜ |
| 2 | 보조 버튼 (3종) | ⬜ |
| 3 | 버프/디버프 아이콘 | ⬜ |
| 3 | 알림 배지 | ⬜ |
| 3 | IP 게이지 바 | ⬜ |
| 4 | 결과 화면 프레임 | ⬜ |
| 4 | 공성 알림 프레임 | ⬜ |
| 4 | 채팅창 프레임 | ⬜ |

---

## 10. 연관 문서

| 문서 | 경로 | 용도 |
|------|------|------|
| UI 기획 상세 | `ai_design_team/90_공통/02_UI_및_시스템/02_UI_구성.md` | 레이아웃, 기능 정의 |
| 카드 시스템 | `ai_design_team/90_공통/05_카드_시스템/05_카드_시스템.md` | 카드 UI 연관 |
| 캐릭터 애니메이션 규격 | `ai_design_team/90_공통/04_리소스_규격/04_애니메이션_규격.md` | 스타일 참조 |
| 이펙트 규격 | `ai_design_team/90_공통/04_리소스_규격/05_이펙트_규격.md` | UI 이펙트 참조 |
| 아트팀 전체 README | `ai_art_team/README.md` | 팀 구조 |

---

## 11. 주의사항

### 절대 금지
- 안티앨리어싱 적용
- 불투명 배경
- 텍스트 포함 (아이콘에 글자 넣지 않음)
- 비일관적인 스타일 (캐릭터 파트와 맞추기)

### 필수 확인
- 모든 PNG는 투명 배경
- 크기는 2의 배수 권장 (24, 32, 48, 64, 80, 120, 150...)
- 파일명 규칙 준수
- 작업 후 현황 업데이트

---

**문서 버전**: 1.0
**최초 작성**: 2025-01-14
**마지막 수정**: 2025-01-14
