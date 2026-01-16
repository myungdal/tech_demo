# 캐릭터 스프라이트 자동 생성 가이드

이 스크립트는 OpenAI DALL-E 3 API를 사용하여 12개 캐릭터의 스프라이트 시트를 자동으로 생성합니다.

---

## 📋 사전 요구사항

### 1. Node.js 설치
- Node.js 18 이상 필요
- [다운로드](https://nodejs.org/)

### 2. OpenAI API 키
- [OpenAI Platform](https://platform.openai.com/api-keys)에서 발급
- 결제 수단 등록 필요

---

## 🚀 실행 방법

### Step 1: API 키 설정

**PowerShell:**
```powershell
$env:OPENAI_API_KEY="sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
```

**CMD:**
```cmd
set OPENAI_API_KEY=sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

### Step 2: 스크립트 실행

```powershell
cd C:\dev\nearest3\ai_art_team\캐릭터
node generate_sprites.js
```

---

## 💰 예상 비용

| 항목 | 비용 |
|:---|:---|
| DALL-E 3 (1024×1024, HD) | $0.12 / 이미지 |
| 12개 캐릭터 | **약 $1.44** |

---

## 📁 출력 결과

스크립트 실행 후 `애니메이션/` 폴더에 PNG 파일이 생성됩니다:

```
ai_art_team/캐릭터/애니메이션/
├── archer.png
├── bat.png
├── bear.png
├── cupid.png
├── dragon.png
├── golem.png
├── hades.png
├── assassin.png
├── phoenix.png
├── spider.png
├── warrior.png
└── zeus.png
```

---

## ⚠️ 주의사항

1. **API 속도 제한**: 분당 5개 요청 제한으로 인해 15초 간격으로 생성
2. **총 소요 시간**: 약 3~4분
3. **후처리 필요**: 마젠타 배경 제거, 프레임 정렬 확인

---

## 🔧 문제 해결

### "OPENAI_API_KEY 환경변수가 설정되지 않았습니다"
→ Step 1의 API 키 설정 확인

### "insufficient_quota"
→ OpenAI 계정에 크레딧 충전 필요

### "rate_limit_exceeded"
→ 15초 대기 시간이 부족할 수 있음. 스크립트에서 대기 시간 늘리기

---

## 📌 다음 단계

생성 완료 후:
1. **배경 제거**: 마젠타(#FF00FF) 배경을 투명으로 변환
2. **프레임 정렬**: 128×128 그리드에 맞게 조정
3. **언리얼 임포트**: Paper2D 스프라이트로 변환
4. **품질 검증**: `애니메이션규격및작업지시서.md`의 체크리스트 확인
