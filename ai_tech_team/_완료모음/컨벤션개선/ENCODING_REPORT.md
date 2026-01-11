# 인코딩 조사 및 복구 보고서

**작성일:** 2026-01-01  
**대상:** 본 프로젝트 로컬 수정 파일

---

## 🔴 현상 (Symptom)

### 1. C++ 소스 파일 UTF-8 BOM 누락 (23개 파일)

Git으로 수정된 파일 중 23개의 C++ 소스 파일이 **UTF-8 without BOM** 상태로 저장되어 있었습니다.

#### 영향받은 파일 분류

**서버 엔진 핵심 헤더 (20개):**
- `server/mainServer/MainServer.ixx`
- `server/serverEngine/ServerEngine/AppBase/AppBase.h` ⚠️ 한글 주석 포함
- `server/serverEngine/ServerEngine/AppConfigManager/DbConfigData.h`
- `server/serverEngine/ServerEngine/Core/AppConstants.h` ⚠️ 한글 주석 포함
- `server/serverEngine/ServerEngine/Core/Macros.h`
- `server/serverEngine/ServerEngine/Core/NetworkTypes.h`
- `server/serverEngine/ServerEngine/Db/DbConnection/DbConnection.h` ⚠️ 한글 주석 포함
- `server/serverEngine/ServerEngine/Db/DbConnection/DbConnectionPool.h`
- `server/serverEngine/ServerEngine/Db/DbSessionBase/DbSession.h`
- `server/serverEngine/ServerEngine/DbSpBase/DbSp.h`
- `server/serverEngine/ServerEngine/Exception/Exception.h` ⚠️ 한글 주석 포함
- `server/serverEngine/ServerEngine/Http/HttpClient.h`
- `server/serverEngine/ServerEngine/Lock/ReadLock.h`
- `server/serverEngine/ServerEngine/Lock/WriteLock.h`
- `server/serverEngine/ServerEngine/Network/Iocp/Iocp.h`
- `server/serverEngine/ServerEngine/Network/NetworkManager.h`
- `server/serverEngine/ServerEngine/Thread/ThreadBase/Thread.h` ⚠️ 한글 주석 포함
- `server/serverEngine/ServerEngine/Util/StringUtil/StringUtil.h`
- `server/serverEngine/ServerEngine/Util/TimeUtil/TimeUtil.h`
- `server/serverEngine/ServerEngine/Worker/WorkerBase/Worker.h`

**새로 생성된 모듈 파일 (3개):**
- `server/mainServer/MainServerPacket.ixx` ⚠️ 한글 주석 포함
- `server/mainServer/MainServerSocket.ixx` ⚠️ 한글 주석 포함
- `server/mainServer/MainServerUtil.ixx`

### 2. 특이사항

- **모든 파일 첫 줄에 `// UTF-8 with BOM` 주석이 있었으나 실제로는 BOM이 없었음**
- 최소 7개 이상의 파일에 한글 주석이 포함되어 있어 잠재적 컴파일 문제 위험
- 클라이언트 파일(14개)은 모두 정상적으로 UTF-8 with BOM 유지

### 3. 규칙 위반

`.cursorrules` 명시 사항:
> 모든 소스 파일은 **UTF-8 with BOM** 인코딩을 사용합니다.

23개 파일이 이 규칙을 위반했습니다.

---

## 🔍 원인 (Root Cause)

### 1. Cursor AI의 파일 저장 방식

**주요 원인:** Cursor AI가 파일을 수정/생성할 때 기본적으로 **UTF-8 without BOM**으로 저장

**증거:**
- `search_replace`, `write` 도구 사용 시 BOM 제거 확인
- 새로 생성된 `.ixx` 파일 3개 모두 BOM 없이 생성됨
- 주석으로 `// UTF-8 with BOM`을 추가했지만 실제 인코딩은 반영되지 않음

### 2. 시스템적 문제

**근본 원인:**
1. **Cursor AI 도구의 기본 동작:** UTF-8 without BOM 사용
2. **검증 부재:** 커밋 전 인코딩 검증 시스템 없음
3. **EditorConfig 부재:** 프로젝트 인코딩 설정 파일 없음
4. **Git Hook 부재:** 자동 검증 메커니즘 없음

### 3. 잠재적 위험

한글 주석이 포함된 파일들이 BOM 없이 저장되면:
- MSVC 컴파일러 경고 발생 가능 (C4819)
- 한글 주석이 깨져서 표시될 수 있음
- 팀원 간 인코딩 불일치로 인한 협업 문제

---

## ⚡ 조치 (Action Taken)

### ✅ 즉시 조치 완료

#### 1. 파일 복구 (23개 파일)

**실행:** `.\bat\fix_encoding_bom.ps1`

**결과:**
```
Fixed: 23 files
Already OK: 0 files
Failed: 0 files
```

**검증 완료:**
- 모든 파일이 UTF-8 with BOM으로 변환됨
- 한글 주석 정상 유지 확인
- 파일 내용 변경 없음 (인코딩만 변경)

#### 2. 자동화 도구 생성

**생성된 스크립트:**

| 파일 | 용도 |
|------|------|
| `bat/fix_encoding_bom.ps1` | 수정된 파일 중 BOM 없는 파일 자동 복구 |
| `bat/check_encoding_bom.ps1` | 프로젝트 전체 인코딩 검증 (자동 수정 옵션 포함) |
| `bat/install_git_hook.ps1` | Git pre-commit hook 설치 |
| `bat/README_ENCODING.md` | 인코딩 관리 가이드 문서 |

#### 3. 프로젝트 설정 파일

**생성된 설정:**

| 파일 | 내용 |
|------|------|
| `.editorconfig` | C++ 파일 UTF-8 with BOM 강제 설정 |
| `.git/hooks/pre-commit` | 커밋 전 자동 BOM 검증 |

#### 4. Git Hook 설치

**실행:** `.\bat\install_git_hook.ps1`

**효과:**
- C++ 파일 커밋 시 자동으로 BOM 검증
- BOM 없는 파일 발견 시 커밋 차단
- 개발자에게 수정 방법 안내

---

## 🚫 재발 방지 (Prevention)

### 1. 자동 검증 시스템

#### Git Pre-commit Hook (설치 완료)
```bash
# 커밋 전 자동 실행
# BOM 없는 C++ 파일 발견 시 커밋 차단
```

#### 정기 검증 스크립트
```powershell
# 주기적으로 실행 권장
.\bat\check_encoding_bom.ps1
```

### 2. 프로젝트 설정

#### .editorconfig (생성 완료)
```ini
[*.{cpp,h,ixx}]
charset = utf-8-bom
```

#### Cursor/VSCode 권장 설정
```json
{
  "files.encoding": "utf8bom",
  "files.autoGuessEncoding": false
}
```

### 3. 개발 프로세스

#### 파일 수정 후 체크리스트
1. ✅ 코드 변경 완료
2. ✅ 인코딩 검증: `.\bat\check_encoding_bom.ps1`
3. ✅ Git 커밋 (자동 검증됨)

#### 새 파일 생성 시
1. ✅ 파일 생성 후 즉시 BOM 확인
2. ✅ 필요 시 `.\bat\fix_encoding_bom.ps1` 실행

### 4. CI/CD 통합 (권장)

```yaml
# GitHub Actions 예시
- name: Check UTF-8 BOM
  run: .\bat\check_encoding_bom.ps1
```

---

## 📊 영향 분석

### 긍정적 영향
- ✅ MSVC 컴파일 경고 방지
- ✅ 한글 주석 정상 표시 보장
- ✅ 팀원 간 인코딩 일관성 확보
- ✅ 자동 검증으로 실수 방지

### 변경 사항
- 📝 23개 파일의 인코딩 변경 (내용은 동일)
- 📝 Git에서 변경으로 표시될 수 있음
- 📝 다음 커밋에 포함 필요

---

## 📋 후속 조치 권장

### 즉시 (완료)
- [x] 23개 파일 BOM 복구
- [x] Git Hook 설치
- [x] .editorconfig 생성
- [x] 자동화 스크립트 작성

### 단기 (권장)
- [ ] 팀원들에게 인코딩 규칙 공유
- [ ] `bat/README_ENCODING.md` 문서 검토
- [ ] Cursor 설정 업데이트 안내

### 중기 (선택)
- [ ] CI/CD에 인코딩 검증 추가
- [ ] 정기 검증 스케줄 설정
- [ ] 자동 생성 스크립트 인코딩 점검

---

## 🔗 참고 자료

### 생성된 문서
- `bat/README_ENCODING.md` - 인코딩 관리 상세 가이드
- `ENCODING_REPORT.md` - 이 보고서

### 관련 규칙
- `.cursorrules` - 프로젝트 전체 규칙
- `server/.cursorrules` - 서버 폴더 규칙
- `client/Client/Source/.cursorrules` - 클라이언트 폴더 규칙

### 도구 사용법
```powershell
# 검증
.\bat\check_encoding_bom.ps1

# 자동 수정
.\bat\check_encoding_bom.ps1 -FixMode

# 수정된 파일만 복구
.\bat\fix_encoding_bom.ps1

# Git Hook 설치
.\bat\install_git_hook.ps1
```

---

## ✅ 결론

**문제:** 23개 C++ 파일이 UTF-8 without BOM으로 저장되어 규칙 위반  
**원인:** Cursor AI의 기본 저장 방식 + 검증 시스템 부재  
**조치:** 전체 파일 복구 + 자동 검증 시스템 구축 완료  
**예방:** Git Hook + EditorConfig + 자동화 스크립트로 재발 방지

**현재 상태:** ✅ 모든 파일 정상화 완료, 재발 방지 시스템 구축 완료

