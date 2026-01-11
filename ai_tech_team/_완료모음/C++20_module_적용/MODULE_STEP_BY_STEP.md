# C++20 Module 전환 - 단계별 작업 가이드

## 개요

각 단계는 **15분 이내**에 완료 가능하며, 단계별로 **커밋**하여 문제 발생 시 롤백할 수 있습니다.

**총 11단계, 예상 시간: 2시간 45분**

---

## Step 1: 사전 준비 (15분)

### 목표
- 현재 상태 백업
- 정상 빌드 확인

### 작업
```
[ ] 1. Git 현재 상태 커밋
    git add -A
    git commit -m "Before C++20 Module conversion"

[ ] 2. 솔루션 전체 빌드 (Release 또는 Debug)
    - Visual Studio에서 F7
    - 빌드 성공 확인

[ ] 3. 서버 실행 테스트 (선택)
    - MainServer 실행
    - 정상 작동 확인
```

### 완료 조건
- [ ] 빌드 성공
- [ ] Git 커밋 완료

### 롤백 방법
```
# 주의: 미커밋 변경사항이 있으면 먼저 stash!
git stash                    # 미커밋 변경 임시 저장
git reset --hard HEAD        # 마지막 커밋으로 복원
# git stash pop              # 필요시 저장된 변경 복원
```

---

## Step 2: ServerEngine 프로젝트 설정 (15분)

### 목표
- C++20 표준 활성화
- 모듈 지원 활성화

### 작업
```
[ ] 1. Visual Studio에서 serverEngine 프로젝트 우클릭 → 속성

[ ] 2. 구성: "모든 구성", 플랫폼: "x64" 선택

[ ] 3. C/C++ → 일반
    - C++ 언어 표준: "ISO C++20 표준(/std:c++20)"

[ ] 4. C/C++ → 고급
    - "스캔 소스에서 모듈 종속성 검색": 예
    
    주의: 여기서 /interface 옵션을 프로젝트 전체에 적용하지 마세요!
    /interface는 Step 3-4에서 .ixx 파일에만 개별 적용합니다.

[ ] 5. 적용 → 확인

[ ] 6. 빌드 테스트 (F7)
    - 기존 코드가 여전히 빌드되는지 확인
```

### 완료 조건
- [ ] 프로젝트 설정 변경 완료
- [ ] 기존 코드 빌드 성공

### 롤백 방법
- 프로젝트 속성에서 C++ 언어 표준을 원래대로 변경

### 커밋
```
git add -A
git commit -m "Step 2: Enable C++20 for ServerEngine"
```

---

## Step 3: ServerEngine .ixx 파일 추가 (1/2) (15분)

### 목표
- 6개 .ixx 파일을 프로젝트에 추가

### 작업
```
[ ] 1. Visual Studio 솔루션 탐색기에서 serverEngine 프로젝트 우클릭
    → 추가 → 기존 항목

[ ] 2. 다음 파일들 선택하여 추가:
    - ServerEngine.ixx
    - ServerEngine-Core.ixx
    - ServerEngine-Worker.ixx
    - ServerEngine-Lock.ixx
    - ServerEngine-Network.ixx
    - ServerEngine-Socket.ixx

[ ] 3. 각 .ixx 파일 우클릭 → 속성:

    [일반]
    - "항목 형식": C/C++ 컴파일러
    
    [C/C++ → 일반]
    - "미리 컴파일된 헤더 사용": 사용 안 함 (/Yu 해제)
    
    [C/C++ → 고급]
    - "모듈 인터페이스로 컴파일": 예 (/interface)
    - "모듈 출력 파일명": $(IntDir)%(Filename).ifc
    
    중요: 
    - 항목 형식이 "C/C++ 컴파일러"인지 반드시 확인!
    - /interface는 .ixx 파일에만 개별 적용!
    - PCH는 .ixx에서 반드시 비활성화! (GMF로 대체됨)

[ ] 4. 프로젝트 저장 (Ctrl+S)
```

### 완료 조건
- [ ] 6개 파일이 프로젝트에 표시됨
- [ ] 모든 .ixx에 /interface 설정됨
- [ ] 모든 .ixx에 PCH 비활성화됨
- [ ] 모듈 출력 경로 설정됨

### 롤백 방법
- 솔루션 탐색기에서 추가한 파일들 우클릭 → 프로젝트에서 제외

### 커밋
```
git add -A
git commit -m "Step 3: Add ServerEngine .ixx files (1/2)"
```

---

## Step 4: ServerEngine .ixx 파일 추가 (2/2) (15분)

### 목표
- 나머지 9개 .ixx 파일을 프로젝트에 추가

### 작업
```
[ ] 1. Visual Studio 솔루션 탐색기에서 serverEngine 프로젝트 우클릭
    → 추가 → 기존 항목

[ ] 2. 다음 파일들 선택하여 추가:
    - ServerEngine-Log.ixx
    - ServerEngine-Thread.ixx
    - ServerEngine-DB.ixx
    - ServerEngine-Clock.ixx
    - ServerEngine-Util.ixx
    - ServerEngine-Exception.ixx
    - ServerEngine-Http.ixx
    - ServerEngine-AppBase.ixx
    - ServerEngine-PacketTransactor.ixx

[ ] 3. 각 .ixx 파일 우클릭 → 속성:

    [일반]
    - "항목 형식": C/C++ 컴파일러
    
    [C/C++ → 일반]
    - "미리 컴파일된 헤더 사용": 사용 안 함
    
    [C/C++ → 고급]
    - "모듈 인터페이스로 컴파일": 예 (/interface)
    - "모듈 출력 파일명": $(IntDir)%(Filename).ifc

[ ] 4. 프로젝트 저장 (Ctrl+S)
```

### 완료 조건
- [ ] 총 15개 .ixx 파일이 프로젝트에 표시됨
- [ ] 모든 .ixx에 항목 형식 "C/C++ 컴파일러" 설정됨
- [ ] 모든 .ixx에 /interface 설정됨
- [ ] 모든 .ixx에 PCH 비활성화됨

### 커밋
```
git add -A
git commit -m "Step 4: Add ServerEngine .ixx files (2/2)"
```

---

## Step 5: ServerEngine 빌드 테스트 (15분)

### 목표
- ServerEngine 모듈 빌드 성공
- .ifc 파일 생성 확인

### 작업
```
[ ] 1. 솔루션 탐색기에서 serverEngine 프로젝트만 빌드
    - 우클릭 → 빌드

[ ] 2. 빌드 결과 확인
    - 성공: Step 6으로 진행
    - 실패: 오류 메시지 확인 및 수정

[ ] 3. .ifc 파일 생성 확인
    - 경로: $(IntDir) 폴더 (예: x64\Debug\)
    - 파티션 모듈이므로 여러 .ifc 파일 생성됨:
      * ServerEngine.ifc (메인)
      * ServerEngine-Worker.ifc 등 (파티션)
    - 주요 .ifc 파일 존재 여부 확인 (증분 빌드 시 일부만 갱신될 수 있음)

[ ] 4. 오류 발생 시 일반적인 해결책:
    - "모듈을 찾을 수 없음" → 빌드 순서 확인
    - "구문 오류" → .ixx 파일 내용 확인
    - "헤더 없음" → include 경로 확인
```

### 완료 조건
- [ ] ServerEngine 빌드 성공
- [ ] .ifc 파일 생성됨

### 롤백 방법
- .ixx 파일들을 프로젝트에서 제외
- Step 2에서 변경한 설정 되돌리기

### 커밋
```
git add -A
git commit -m "Step 5: ServerEngine module build success"
```

---

## Step 6: MainServer 모듈화 (15분)

### 목표
- MainServer 프로젝트 C++20 모듈 설정
- MainServer.ixx 추가 및 빌드

### 작업
```
[ ] 1. mainServer 프로젝트 우클릭 → 속성
    - C/C++ → 일반 → C++ 언어 표준: /std:c++20
    - C/C++ → 고급 → 스캔 소스에서 모듈 종속성 검색: 예

[ ] 2. MainServer.ixx 파일 프로젝트에 추가
    - 추가 → 기존 항목 → MainServer.ixx

[ ] 3. MainServer.ixx 속성 설정:
    [일반]
    - 항목 형식: C/C++ 컴파일러
    [C/C++ → 일반]
    - 미리 컴파일된 헤더: 사용 안 함
    [C/C++ → 고급]
    - 모듈 인터페이스로 컴파일: 예
    - 모듈 출력 파일명: $(IntDir)%(Filename).ifc

[ ] 4. main.cpp 수정 (모듈 import 방식으로):
    
    // Before (기존 방식)
    #include "pch.h"
    #include "MainServer/App/MainServerApp.h"
    
    // After (모듈 방식)
    #include "pch.h"           // PCH는 그대로 유지 (맨 앞)
    import ServerEngine;       // import는 PCH 다음
    import MainServer;
    // 매크로 필요한 헤더만 추가 include
    
    주의: PCH가 있으면 PCH가 맨 앞, 그 다음 import!

[ ] 5. 프로젝트 빌드 (우클릭 → 빌드)

[ ] 6. 오류 수정 (필요시)
```

### 완료 조건
- [ ] MainServer 빌드 성공
- [ ] main.cpp가 import 방식으로 수정됨

### 커밋
```
git add -A
git commit -m "Step 6: MainServer module setup complete"
```

---

## Step 7: FrontServer, GameServer 모듈화 (15분)

### 목표
- 2개 서버 프로젝트 모듈화

### 작업 (각 프로젝트마다 동일하게)
```
[ ] 1. FrontServer:
    a) 프로젝트 속성 → C++20 활성화
    b) FrontServer.ixx 추가 + 속성 설정:
       - 항목 형식: C/C++ 컴파일러
       - PCH 사용 안 함
       - /interface 활성화
       - 모듈 출력: $(IntDir)%(Filename).ifc
    c) main.cpp 수정:
       #include "pch.h"         // PCH 유지
       import ServerEngine;
       import FrontServer;
    d) 빌드 테스트

[ ] 2. GameServer:
    a) 프로젝트 속성 → C++20 활성화
    b) GameServer.ixx 추가 + 속성 설정 (위와 동일)
    c) main.cpp 수정:
       #include "pch.h"
       import ServerEngine;
       import GameServer;
    d) 빌드 테스트
```

### 완료 조건
- [ ] FrontServer 빌드 성공
- [ ] GameServer 빌드 성공
- [ ] 각 main.cpp가 PCH + import 방식으로 수정됨

### 커밋
```
git add -A
git commit -m "Step 7: FrontServer, GameServer module setup"
```

---

## Step 8: DbServer, LogServer, BridgeServer 모듈화 (15분)

### 목표
- 3개 서버 프로젝트 모듈화

### 작업 (각 프로젝트마다)
```
[ ] 1. DbServer:
    a) C++20 활성화
    b) DbServer.ixx 추가 + 항목형식/PCH해제/interface 설정
    c) main.cpp: #include "pch.h" → import ServerEngine; import DbServer;
    d) 빌드 테스트

[ ] 2. LogServer:
    a) C++20 활성화
    b) LogServer.ixx 추가 + 항목형식/PCH해제/interface 설정
    c) main.cpp: #include "pch.h" → import ServerEngine; import LogServer;
    d) 빌드 테스트

[ ] 3. BridgeServer:
    a) C++20 활성화
    b) BridgeServer.ixx 추가 + 항목형식/PCH해제/interface 설정
    c) main.cpp: #include "pch.h" → import ServerEngine; import BridgeServer;
    d) 빌드 테스트
```

### 완료 조건
- [ ] DbServer 빌드 성공
- [ ] LogServer 빌드 성공
- [ ] BridgeServer 빌드 성공

### 커밋
```
git add -A
git commit -m "Step 8: DbServer, LogServer, BridgeServer module setup"
```

---

## Step 9: Shell, Bot 모듈화 (15분)

### 목표
- 2개 프로젝트 모듈화

### 작업
```
[ ] 1. Shell:
    a) C++20 활성화
    b) Shell.ixx 추가 + 항목형식/PCH해제/interface 설정
    c) main.cpp: #include "pch.h" → import ServerEngine; import Shell;
    d) 빌드 테스트

[ ] 2. Bot:
    a) C++20 활성화
    b) Bot.ixx 추가 + 항목형식/PCH해제/interface 설정
    c) main.cpp: #include "pch.h" → import ServerEngine; import Bot;
    d) 빌드 테스트
```

### 완료 조건
- [ ] Shell 빌드 성공
- [ ] Bot 빌드 성공

### 커밋
```
git add -A
git commit -m "Step 9: Shell, Bot module setup"
```

---

## Step 10: Test 프로젝트 모듈화 (15분)

### 목표
- Test 프로젝트가 ServerEngine 모듈을 참조하도록 설정

### 작업
```
[ ] 1. test 프로젝트 우클릭 → 속성
    - C/C++ → 일반 → C++ 언어 표준: /std:c++20
    - C/C++ → 고급 → 스캔 소스에서 모듈 종속성 검색: 예

[ ] 2. test 프로젝트 종속성 확인
    - 프로젝트 우클릭 → 빌드 종속성 → 프로젝트 종속성
    - serverEngine이 체크되어 있는지 확인

[ ] 3. Test 코드에서 ServerEngine 모듈 import:
    
    // Before
    #include "pch.h"
    #include "ServerEngine/Worker/WorkerImpl/CoWorker.h"
    
    // After
    #include "pch.h"           // PCH는 반드시 맨 앞!
    import ServerEngine;       // import는 PCH 다음
    #include "Debug.h"         // 매크로 필요한 헤더는 마지막
    
    순서: PCH → import → 매크로 헤더

[ ] 4. 빌드 테스트

[ ] 5. 테스트 실행 확인
```

### 완료 조건
- [ ] Test 프로젝트 빌드 성공
- [ ] 테스트 실행 정상

### 커밋
```
git add -A
git commit -m "Step 10: Test project module setup"
```

---

## Step 11: 전체 통합 테스트 (15분)

### 목표
- 전체 솔루션 리빌드
- 서버 실행 테스트

### 작업
```
[ ] 1. 솔루션 전체 리빌드
    - 빌드 → 솔루션 다시 빌드

[ ] 2. 빌드 성공 확인

[ ] 3. MainServer 실행 테스트
    - Debug 또는 Release로 실행
    - 정상 시작 확인

[ ] 4. Test 프로젝트 실행
    - 테스트 정상 통과 확인

[ ] 5. (선택) 다른 서버들도 실행 테스트

[ ] 6. (선택) 클라이언트 연결 테스트
```

### 완료 조건
- [ ] 전체 솔루션 빌드 성공
- [ ] MainServer 실행 성공
- [ ] Test 프로젝트 테스트 통과

### 최종 커밋
```
git add -A
git commit -m "Complete: C++20 Module conversion finished"
```

---

## 요약 체크리스트

| Step | 내용 | 예상 시간 | 완료 |
|------|------|-----------|------|
| 1 | 사전 준비 (백업, 빌드 확인) | 15분 | [ ] |
| 2 | ServerEngine C++20 설정 | 15분 | [ ] |
| 3 | ServerEngine .ixx 추가 (1/2) + PCH/interface 설정 | 15분 | [ ] |
| 4 | ServerEngine .ixx 추가 (2/2) + PCH/interface 설정 | 15분 | [ ] |
| 5 | ServerEngine 빌드 테스트 | 15분 | [ ] |
| 6 | MainServer 모듈화 + main.cpp import 수정 | 15분 | [ ] |
| 7 | FrontServer, GameServer + main.cpp import 수정 | 15분 | [ ] |
| 8 | DbServer, LogServer, BridgeServer | 15분 | [ ] |
| 9 | Shell, Bot | 15분 | [ ] |
| 10 | Test 프로젝트 모듈화 | 15분 | [ ] |
| 11 | 전체 통합 테스트 | 15분 | [ ] |
| **총계** | | **2시간 45분** | |

---

## 문제 발생 시 롤백 가이드

### ?? 롤백 전 주의사항

`git reset --hard`는 **미커밋 변경사항을 영구 삭제**합니다!

```
# 롤백 전 반드시 확인:
git status                   # 미커밋 변경 확인
git stash                    # 미커밋 변경 임시 저장 (필요시)
```

### 특정 Step에서 문제 발생 시

1. **미커밋 변경 저장 (선택)**
```
git stash                    # 현재 변경사항 임시 저장
```

2. **해당 Step 커밋으로 되돌리기**
```
git log --oneline            # 커밋 목록 확인
git reset --hard <이전_커밋_해시>
```

3. **문제 분석 후 재시도**

### 전체 롤백 (처음 상태로)
```
git stash                    # 미커밋 변경 임시 저장
git reset --hard <Step1_이전_커밋>
```

### .ixx 파일만 제외하기 (소프트 롤백)

Git을 사용하지 않고 수동으로 되돌리기:
- 솔루션 탐색기에서 .ixx 파일들 선택
- 우클릭 → 프로젝트에서 제외
- 프로젝트 속성에서 C++20 설정 되돌리기

---

## 자주 발생하는 오류 및 해결

### 1. "모듈 'ServerEngine'을(를) 찾을 수 없습니다"
- **원인**: ServerEngine이 먼저 빌드되지 않음
- **해결**: 프로젝트 종속성 설정 또는 ServerEngine 먼저 빌드

### 2. "헤더 파일을 열 수 없습니다"
- **원인**: include 경로 문제
- **해결**: .ixx 파일의 #include 경로 확인

### 3. "export 선언이 잘못되었습니다"
- **원인**: C++20 표준이 활성화되지 않음
- **해결**: 프로젝트 속성에서 /std:c++20 확인

### 4. "using 선언이 잘못되었습니다"
- **원인**: 해당 클래스가 글로벌 네임스페이스에 없음
- **해결**: .ixx 파일의 using 선언 확인

### 5. "모든 .cpp 파일이 인터페이스로 처리됨"
- **원인**: 프로젝트 전체에 /interface가 적용됨
- **해결**: .ixx 파일에만 개별적으로 /interface 적용

### 6. "매크로가 import 후 사용 불가"
- **원인**: C++20 모듈에서 매크로는 import로 전파 안 됨
- **해결**: constexpr/inline 함수로 변환, 또는 별도 헤더 include

---

**팁**: 각 Step 완료 후 반드시 커밋하세요! 문제 발생 시 쉽게 롤백할 수 있습니다.

