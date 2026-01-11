# 10. 군집 시뮬레이션 게임성 프로토타이핑 (Javascript)

작성자: 안명달 (mooondal@gmail.com)

## 개요

C++ 로직을 구현하기 전에 알고리즘과 게임성을 빠르게 검증하기 위한 JavaScript 기반 프로토타입 모음이다. 브라우저에서 즉시 실행 가능하여 아이디어를 빠르게 테스트하고 시각화할 수 있다.

## 라이브 데모

> **[프로토타입 목록 페이지 바로가기](https://mooondal.github.io/nearest3/docs/index.html)**

## 프로토타입 목록

### 1. Flock - 군집 행동 시뮬레이션

2000개 객체의 군집 행동(Boid) 알고리즘 검증. 분리(Separation), 정렬(Alignment), 응집(Cohesion) 규칙 기반 자연스러운 무리 이동.

- **[라이브 데모](https://mooondal.github.io/nearest3/docs/1_flock/index.html)**
- 소스: `docs/1_flock/`

---

### 2. Hull - 충돌 및 그리드 시스템

그리드 기반 공간 분할, 스킬/이펙트 시스템, 다중 팀 전투 시뮬레이션. 2000개 유닛 동시 처리 테스트.

- **[라이브 데모](https://mooondal.github.io/nearest3/docs/2_hull/index.html)**
- 소스: `docs/2_hull/`

---

### 3. Predators - LockStep 동기화

LockStep 동기화 알고리즘 검증. 클라이언트-서버 동기화 로직의 핵심 프로토타이핑.

- **[라이브 데모](https://mooondal.github.io/nearest3/docs/3_predators/index.html)**
- 소스: `docs/3_predators/`

---

### 4. RPG - 스킬 및 속성 시스템

RPG 스킬 시스템과 캐릭터 속성(Attribute) 설계 검증. 스킬 쿨다운, 데미지 계산 로직 테스트.

- **[라이브 데모](https://mooondal.github.io/nearest3/docs/4_rpg/index.html)**
- 소스: `docs/4_rpg/`

---

### 5. Skill - 스킬 이펙트 시스템

다양한 스킬 이펙트(범위, 투사체, 지속 효과)와 LockStep 결합 테스트. 시각적 피드백 검증.

- **[라이브 데모](https://mooondal.github.io/nearest3/docs/5_skill/index.html)**
- 소스: `docs/5_skill/`

---

### 6. Combination - 조합 시스템

유닛 조합 및 진화 시스템 게임성 검증. 플레이어 상호작용과 전략적 요소 테스트.

- **[라이브 데모](https://mooondal.github.io/nearest3/docs/6_combination/index.html)**
- 소스: `docs/6_combination/`

---

### 7. Craft - 컴포넌트 기반 전투 시뮬레이션 (최종 프로토타입)

**MmoSync의 전신**이 되는 완성도 높은 컴포넌트 기반 전투 시뮬레이션. `docs/7_craft/app/systems/`에 **54개 시스템 파일**이 존재하며, 다양한 유닛 타입/스킬 조합/그리드 기반 물리 등 핵심 게임플레이 검증을 포함한다.

- 단축키 기반 유닛 생성/제어
- 다중 팀 전투
- 컴포넌트-시스템 아키텍처

- **[라이브 데모](https://mooondal.github.io/nearest3/docs/7_craft/app/index.html)**
- 소스: `docs/7_craft/`

---

### 8. JitterTest - 네트워크 지터 테스트

TimeStep 기반 시뮬레이션에서 네트워크 지터(Jitter) 보상 알고리즘 검증. 패킷 지연/손실 상황 시뮬레이션.

- **[라이브 데모](https://mooondal.github.io/nearest3/docs/8_jitterTest/index.html)**
- 소스: `docs/8_jitterTest/`

---

## 프로토타이핑 장점

| 장점 | 설명 |
|------|------|
| **빠른 이터레이션** | 컴파일 없이 브라우저에서 즉시 실행 |
| **시각적 검증** | Canvas 기반 실시간 렌더링으로 알고리즘 동작 확인 |
| **게임성 테스트** | 밸런스, 재미 요소를 C++ 구현 전에 검증 |
| **설계 문서화** | 프로토타입 코드가 설계 명세 역할 |

---

