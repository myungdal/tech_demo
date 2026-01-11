# 32. AI 규칙 파일(.clauderules / antigravityrules)로 컨벤션 관리하기

작성자: 안명달 (mooondal@gmail.com)

## 1. 개요

프로젝트가 커질수록 "코드/문서의 일관성"은 중요하다.
특히 AI 코딩 에이전트가 참여하면, 규칙이 문서로만 존재하는 순간부터 규칙 위반이 급격히 늘어날 수 있다.
사람이 보는 문서와 AI가 보는 문서를 따로 나누기 보다는 본인이 사용하는 툴에 따라, `.clauderules`, `antigravityrules`, `.cursorrules`를 설정하는 방법을 추천하고 싶다.

토큰 사용량을 줄이려면, 규칙 문구에만 의존하기 보다는 컨벤션 검사나 수정을 프로그램화 하여 git 커밋에 설정하는 방법을 추천한다.

---

## 2. 인코딩/줄바꿈 운영(UTF-8 BOM + CRLF) 예시

가장 안전한 운영은 "규칙 -> 자동 검증 -> 자동 수정" 루프

```powershell
# 검증만
.\power_shell\check_encoding_full.ps1

# 자동 수정
.\power_shell\check_encoding_full.ps1 -FixMode
```

Git hook/CI로 강제(실수 방지)
규칙을 문서로만 두면, 사람은 반드시 실수한다
따라서 pre-commit hook 또는 CI에서 인코딩 검사 스크립트를 실행해
"규칙 위반은 커밋 불가"로 만드는 것이 가장 효과적

---

(추가 작성 예정)