# 99. 기술 스택 요약

작성자: 안명달 (mooondal@gmail.com)

## 서버 기술

| 분류 | 기술 | 상세 |
|------|------|------|
| **언어** | C++20 | C++20 Modules, Concepts, Ranges |
| **모듈 시스템** | C++20 Modules | 50개 .ixx 파일, 빌드 28% 단축 |
| **네트워크** | Windows IOCP | 비동기 I/O, Bypass 패킷 패턴 |
| **데이터베이스** | MySQL 8 | ODBC, Master-Slave 복제 |
| **메모리 할당** | tcmalloc | Thread Caching 기반 고속 할당 |
| **메시지 큐** | RabbitMQ | 운영툴 연동, Topic Exchange |
| **로그 수집** | EFK 스택 | FluentBit + FluentD + ElasticSearch + Kibana |
| **빌드** | Visual Studio 2022 | MSBuild, C++20 지원 |
| **버전 관리** | Git | GitHub 연동 |

## 클라이언트 기술

| 분류 | 기술 | 상세 |
|------|------|------|
| **엔진** | Unreal Engine 5.5 | C++ 기반 게임 클라이언트 |
| **언어** | C++20 | 서버와 공유 라이브러리 동기화 |
| **동기화 엔진** | MmoSync | 컴포넌트 기반 + 상태-이벤트 네트워크 동기화 |
| **멀티스레드** | 병렬 시뮬레이션 | UE 렌더링과 게임 로직 분리 실행 |
| **UI 시스템** | UMG + 레드닷 트리 | 계층적 UI 알림 시스템 |

## 공유 라이브러리 (서버-클라 공통)

| 분류 | 기술 | 상세 |
|------|------|------|
| **Packet** | 자동 생성 패킷 | Excel -> TypeScript -> C++ 코드 생성 |
| **Common** | 공통 유틸리티 | Random, Stat, Math, FlowControl |
| **MmoSync** | 동기화 엔진 | 클라-서버 동일 로직 공유, 컴포넌트 기반 |
| **SharedLib** | 공유 라이브러리 | StringKey, BadWordFilter, RandMark 등 |

## 도구 기술

| 분류 | 기술 | 상세 |
|------|------|------|
| **코드 생성** | TypeScript, Node.js | 패킷, ENUM, SP, Config 자동 생성 |
| **스프레드시트** | xlsx 파싱 | Excel -> DB 데이터 + Validation |
| **스크립트** | PowerShell, Batch | 인코딩 검증, 빌드 자동화 |
| **프로토타이핑** | JavaScript, HTML5 Canvas | C++ 구현 전 알고리즘 검증 |
| **AI 연동** | MCP (Model Context Protocol) | 자연어 기반 서버 모니터링 |
| **컨벤션 관리** | .cursorrules | AI 코딩 에이전트용 프로젝트 규칙 |

## 아키텍처 패턴

### 서버 아키텍처

| 패턴 | 적용 |
|------|------|
| **패킷 처리 4단 레이어** | Socket -> Worker -> Handler -> Transactor |
| **DB 트랜잭션 통합** | 패킷 처리와 DB 트랜잭션 원자적 통합 |
| **서버 간 Bypass 패킷** | Front 서버에서 목적지 서버로 중첩 패킷 직접 전달 |
| **Front 서버 분산** | 글로벌 서비스 대비, 리전별 Front + 중앙 인프라 |
| **Room 시스템** | 친구/파티/길드 동기화를 위한 범용 Pub-Sub |
| **분산 UUID** | 서버 가상화, 무중단 통합/이전 (Snowflake 차용) |

### 성능 최적화

| 패턴 | 적용 |
|------|------|
| **C++20 모듈** | include 75% 감소, 빌드 28% 단축 |
| **TlsClock** | Thread Local 시간 캐싱, 시간 역전 대응 |
| **메모리 풀 3종** | DynamicPool, FixedPool, CompactionPool |
| **MwsrQueue** | Lock-free Multi-Writer Single-Reader 큐 |
| **RWLock** | 다중 읽기/단일 쓰기, Read->Write 업그레이드 지원 |
| **StringKey** | 문자열 해싱 + 포인터 비교로 O(1) 조회 |
| **tcmalloc** | Thread별 메모리 캐싱으로 락 경합 최소화 |

### 데이터 관리

| 패턴 | 적용 |
|------|------|
| **스키마 마이그레이션** | DB 컬럼 변경 시 데이터 자동 복제 |
| **하이브리드 랭킹** | 선형보간 기반 메모리 효율 랭킹 시스템 |
| **StatNodeBuilder** | 스탯 의존성 그래프 자동 구축 및 연쇄 갱신 |
| **로그 자동화** | Excel 정의 -> 패킷/SP/핸들러 자동 생성 -> DB 저장 |

### 게임 시스템

| 패턴 | 적용 |
|------|------|
| **MmoSync** | 클라-서버 동기화 로직 공통 사용 |
| **클라 멀티스레드** | 시뮬레이션을 언리얼 엔진과 병렬 처리 |
| **Flow Field 길찾기** | 대규모 유닛 군집 이동 (O(N) 복잡도) |
| **Cellular Automata** | 절차적 던전 생성, ASCII 시각화 |
| **Cron 스케줄러** | 이벤트/일정 관리, 핸들 기반 취소 시스템 |

### 유틸리티

| 패턴 | 적용 |
|------|------|
| **다양한 랜덤 클래스** | Mt19937, Deterministic, RatioTable, Adaptive 등 |
| **RandMark40/60/80** | 바이럴 친화적 시크릿 키 생성 |
| **BadWordFilter** | Trie 기반 다중 패턴 금지어 필터 |
| **UsageMeter** | 타입별 사용량 추적 (런타임 모니터링) |
| **FlowControl 스크립트** | 자체 개발 경량 스크립트 언어 (AI 수정 가능) |

### 테스트/자동화

| 패턴 | 적용 |
|------|------|
| **Test/Bot Framework** | 명령 디스패처 기반 자동화 테스트 |
| **MCP Shell** | 자연어 기반 서버 모니터링/제어 |
| **프로토타이핑** | JavaScript로 알고리즘/게임성 사전 검증 |

---

