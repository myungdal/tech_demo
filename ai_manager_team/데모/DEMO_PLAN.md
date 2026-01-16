# myungdal tech-demo project - AI 협업 게임 서버 데모

> **20년 경력 서버 프로그래머의 AI 협업 파이프라인 시연**

---

## 🎯 프로젝트 개요

본 프로젝트는 **Unreal Engine 5.6 클라이언트 + C++20 서버**로 구성된 MMORPG 프로젝트입니다.

이 데모는 AI 도구(Cursor, Claude, Copilot 등)를 활용하여 게임 서버 개발 생산성을 **3배 이상 향상**시키는 방법을 보여줍니다.

---

## 🏗️ 아키텍처

```
┌─────────────────────────────────────────────────────────┐
│                    Architecture Overview                 │
├─────────────────────────────────────────────────────────┤
│                                                          │
│  ┌──────────────┐         ┌──────────────┐              │
│  │  UE5 Client  │◄───────►│  C++ Server  │              │
│  │  (Blueprint  │  Packet │  (C++20      │              │
│  │   + C++)     │         │   Modules)   │              │
│  └──────────────┘         └──────────────┘              │
│         │                        │                       │
│         └────────┬───────────────┘                       │
│                  │                                       │
│         ┌────────▼────────┐                              │
│         │  Code Generator │                              │
│         │  (Excel → C++)  │                              │
│         └─────────────────┘                              │
│                  │                                       │
│         ┌────────▼────────┐                              │
│         │    Resource     │                              │
│         │  (xlsx, sql)    │                              │
│         └─────────────────┘                              │
│                                                          │
└─────────────────────────────────────────────────────────┘
```

---

## 📁 폴더 구조

```
nearest3/
├── client/              # UE5 클라이언트
│   └── Client/Source/   # 클라이언트 소스 코드
├── server/              # C++ 서버
│   ├── ServerEngine/    # 서버 엔진 (공용 모듈)
│   ├── MainServer/      # 메인 서버
│   ├── FrontServer/     # 프론트 서버
│   └── DbServer/        # DB 서버
├── resource/            # 리소스 정의
│   ├── packet.xlsx      # 패킷 정의
│   ├── enum.xlsx        # Enum 정의
│   ├── config.xlsx      # 설정 정의
│   └── db/              # DB 스키마
├── bat/                 # 자동화 스크립트
│   ├── generate_all.bat # 전체 코드 생성
│   └── dbAutomation/    # DB 자동화
└── demo/                # 데모 관련 문서
    └── docs/            # 상세 문서
```

---

## 🚀 핵심 기능

### 1. 코드 자동 생성

| 입력 | 출력 |
|------|------|
| `resource/packet.xlsx` | 서버/클라이언트 패킷 클래스 |
| `resource/enum.xlsx` | Enum 정의 (양쪽 동기화) |
| `resource/db/*.sql` | DB 접근 코드 (CRUD) |

### 2. AI 페어 프로그래밍

- Cursor + Claude/Copilot 활용
- `.cursorrules`로 프로젝트 컨벤션 자동 적용
- 실시간 코드 리뷰 및 수정

### 3. 클라이언트-서버 동기화

- 패킷 정의 한 번으로 양쪽 코드 자동 생성
- 타입 안전성 보장
- 버전 불일치 방지

---

## 🎬 데모 시나리오

| 시나리오 | 소요시간 | 난이도 | 설명 |
|----------|----------|--------|------|
| [패킷 추가](docs/DEMO_SCENARIOS.md#시나리오-1-새-패킷-추가) | 5분 | ⭐ | 새 패킷 정의 → 자동 생성 |
| [DB 테이블 추가](docs/DEMO_SCENARIOS.md#시나리오-2-db-테이블-추가) | 10분 | ⭐⭐ | SQL → CRUD 코드 생성 |
| [신규 시스템 구현](docs/DEMO_SCENARIOS.md#시나리오-3-신규-시스템-구현) | 20분 | ⭐⭐⭐ | AI 페어 프로그래밍 |

---

## 📹 데모 영상

> (영상 링크 추가 예정)

---

## 🛠️ 기술 스택

| 분야 | 기술 |
|------|------|
| **클라이언트** | Unreal Engine 5.6, C++, Blueprint |
| **서버** | C++20 Modules, IOCP |
| **DB** | MySQL, PostgreSQL |
| **메시징** | RabbitMQ, Redis |
| **AI 도구** | Cursor, Claude, Copilot, Gemini |
| **자동화** | PowerShell, Python, bat 스크립트 |

---

## 👤 개발자

**안명달 (Ahn Myungdal)**

- 20년+ 게임 개발 경력
- MMORPG 서버 전문
- 이메일: mooondal@gmail.com
- 전화: 010-2331-4249

---

## 📄 관련 문서

- [아키텍처 상세](docs/ARCHITECTURE.md)
- [AI 협업 워크플로우](docs/AI_WORKFLOW.md)
- [데모 시나리오](docs/DEMO_SCENARIOS.md)
- [준비 체크리스트](docs/CHECKLIST.md)
