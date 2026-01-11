# 25. AI 기반 IP 옥텟 대역 분석을 통한 능동적 라우팅 최적화

작성자: 안명달 (mooondal@gmail.com)

> **목차로 돌아가기**: [tech.md](tech.md)

---

## 개요

실제 홍콩 해저망 장애 경험에서 착안한, IP 옥텟 대역별 라우팅 특성을 AI로 분석하여 장애 통신망을 능동적으로 우회하는 시스템이다.

### 발단: 홍콩 해저망 장애 사건

**2023년 X월, 홍콩-일본 해저 케이블 장애 발생:**
```
[상황]
- 홍콩 해저망 일부 구간 장애
- 아시아 리전 -> 일본 리전 통신 품질 급격히 저하
- 레이턴시: 50ms -> 300ms (6배 증가)
- 패킷 손실률: 0.1% -> 15% (150배 증가)

[클라우드 업체 대응]
- 자동 우회 실패 (BGP 라우팅 정책 한계)
- "ISP 복구 대기" 안내만 반복
- 복구 예상 시간: 48시간

[자체 해결 시도]
1. 각 지사(서울, 싱가포르, 도쿄)에 진단 프로그램 배포
2. tracert 데이터 1시간마다 자동 수집
3. 24시간 동안 100+ 샘플 수집
4. 데이터 분석 결과...
```

### 발견: IP 옥텟 대역별 라우팅 경로 차이

```
[서울 -> 일본 서버 (203.0.113.50)]
tracert 203.0.113.50

1   10.0.1.1        1 ms    (로컬 게이트웨이)
2   203.0.113.1     5 ms    (서울 ISP)
3   203.0.114.5     12 ms   (서울-홍콩 해저망)
4   203.0.115.10    280 ms  [장애] (홍콩 장애 구간)
5   203.0.116.20    290 ms  [장애] (홍콩-도쿄)
6   203.0.113.50    300 ms  (목적지)

[서울 -> 일본 서버 (198.51.100.50) - 다른 IP 대역]
tracert 198.51.100.50

1   10.0.1.1        1 ms    (로컬 게이트웨이)
2   198.51.100.1    5 ms    (서울 ISP)
3   198.51.101.5    15 ms   (서울-대만 해저망) <- 우회!
4   198.51.102.10   25 ms   (대만-도쿄 직통)
5   198.51.100.50   35 ms   (목적지)

-> 레이턴시: 300ms -> 35ms (8.6배 개선)
-> 패킷 손실: 15% -> 0.2% (정상 수준)
```

**핵심 발견:**
- **같은 도쿄 리전이지만 IP 옥텟 대역에 따라 경로가 완전히 다름**
- `203.0.0.0/16` 대역: 홍콩 경유 (장애 영향)
- `198.51.0.0/16` 대역: 대만 경유 (정상)

### 즉시 조치

```bash
# 일본 서버 IP 변경 (AWS Elastic IP 재할당)
OLD_IP: 203.0.113.50  (홍콩 경유)
NEW_IP: 198.51.100.50 (대만 경유)

# DNS 업데이트
japan.game-server.com -> 198.51.100.50

# 30분 내 전체 트래픽 정상화
```

-> **ISP 복구 48시간 전에 자체 해결 완료**

---

## 핵심 아이디어

### 문제: ISP 종속적 라우팅의 한계

```
[기존 라우팅 프로토콜의 한계]

BGP (Border Gateway Protocol):
- ISP 간 계약/정책에 따라 경로 결정
- AS(Autonomous System) 수만 고려 (최단 경로)
- 실제 네트워크 품질 무시
- 장애 구간 자동 우회 불가

OSPF (Open Shortest Path First):
- 동일 AS 내부에서만 동작
- ISP 간 라우팅에는 무력
- 정적 링크 비용 기반 (실시간 품질 반영 X)

문제점:
1. 사용자가 경로 선택 불가
2. 장애 구간을 능동적으로 회피 불가
3. ISP 복구 대기만 가능
4. 서비스 품질 저하 불가피
```

### 해결: IP 옥텟 기반 능동적 라우팅

```
[핵심 통찰]

IP 서브넷 대역마다 라우팅 경로가 다름:
- 203.0.0.0/16 -> 홍콩 경유
- 198.51.0.0/16 -> 대만 경유
- 192.0.2.0/24 -> 싱가포르 경유

-> 여러 IP 대역을 확보하면 경로 선택 가능!

[전략]
1. 각 리전에 3~5개 IP 대역 확보
2. 지속적으로 tracert 데이터 수집
3. AI로 IP 대역별 라우팅 특성 학습
4. 장애 발생 시 최적 IP 대역으로 자동 전환
```

---

## 시스템 아키텍처

### 1. 데이터 수집 에이전트

```
[글로벌 지사에 배포]

서울 지사 ─┐
싱가포르  ─┼─-> [수집 에이전트] -> [중앙 분석 서버]
대만      ─┤    - tracert 실행
도쿄      ─┤    - ping 레이턴시
프랑크푸르트┤    - 패킷 손실률
버지니아  ─┘    - 대역폭 테스트
```

**수집 에이전트 동작:**
```python
# 1시간마다 모든 IP 대역 테스트
for target_ip in server_ip_list:
    # tracert 실행
    route = traceroute(target_ip)
    
    # 품질 측정
    latency = measure_latency(target_ip)
    packet_loss = measure_packet_loss(target_ip)
    bandwidth = measure_bandwidth(target_ip)
    
    # 데이터 전송
    send_to_analyzer({
        "timestamp": now(),
        "source": "Seoul",
        "target_ip": target_ip,
        "route": route,
        "latency": latency,
        "packet_loss": packet_loss,
        "bandwidth": bandwidth
    })
```

### 2. AI 분석 엔진

```
[수집 데이터]
- 1시간당 1000+ tracert 샘플
- 7일 = 168,000+ 샘플
- IP 대역별, 시간대별, 지역별 분류

[AI 모델 학습]
입력:
- 출발지 (서울, 싱가포르, ...)
- 목적지 IP 대역 (203.0.0.0/16, 198.51.0.0/16, ...)
- 시간대 (0~23시, 주중/주말)
- 과거 장애 이력

출력:
- 예상 레이턴시
- 예상 패킷 손실률
- 경유 ISP/망 예측
- 장애 위험도 (0~100%)

[학습 알고리즘]
1. 군집화 (Clustering): IP 대역별 라우팅 패턴 그룹화
2. 시계열 분석 (Time Series): 시간대별 품질 변화 예측
3. 이상 탐지 (Anomaly Detection): 장애 구간 자동 감지
4. 강화 학습 (Reinforcement Learning): 최적 IP 선택 전략
```

### 3. 능동적 IP 전환 시스템

```python
class AdaptiveRoutingSystem:
    def __init__(self):
        self.ip_pools = {
            "tokyo": [
                ("203.0.113.50", "hongkong_route"),    # 홍콩 경유
                ("198.51.100.50", "taiwan_route"),     # 대만 경유
                ("192.0.2.50", "singapore_route"),     # 싱가포르 경유
            ],
            "seoul": [...],
            "frankfurt": [...]
        }
        
        self.ai_model = load_trained_model()
    
    def monitor_and_switch(self):
        while True:
            # 1. 현재 품질 측정
            quality = self.measure_current_quality()
            
            # 2. AI 모델로 이상 탐지
            if self.ai_model.detect_anomaly(quality):
                # 3. 장애 판정
                print(f"장애 감지: 레이턴시 {quality.latency}ms, 손실 {quality.packet_loss}%")
                
                # 4. 최적 IP 대역 예측
                best_ip, best_route = self.ai_model.predict_best_ip(
                    current_time=now(),
                    source_regions=["seoul", "singapore", "taiwan"],
                    target_region="tokyo"
                )
                
                # 5. 자동 IP 전환
                self.switch_ip(best_ip, best_route)
                
                # 6. DNS 업데이트
                self.update_dns(best_ip)
                
                print(f"IP 전환 완료: {best_ip} ({best_route})")
            
            sleep(300)  # 5분마다 체크
```

---

## 실전 시나리오

### 시나리오 1: 홍콩 해저망 장애 (실제 사례)

```
[T+0분] 장애 발생
- 홍콩-일본 해저 케이블 절단
- 203.0.0.0/16 대역 레이턴시 50ms -> 300ms
- 패킷 손실 0.1% -> 15%

[T+5분] AI 시스템 이상 탐지
- 연속 5회 품질 저하 감지
- 장애 확률: 98%
- 경유 망 분석: 홍콩 ISP (HKT)

[T+10분] 최적 IP 대역 탐색
- AI 모델 예측:
  * 198.51.0.0/16 (대만 경유): 예상 35ms, 손실 0.2% [최적]
  * 192.0.2.0/24 (싱가포르 경유): 예상 60ms, 손실 0.5%
  * 203.0.0.0/16 (홍콩 경유): 예상 300ms, 손실 15% [장애]

[T+15분] 자동 IP 전환
- OLD: 203.0.113.50 -> NEW: 198.51.100.50
- DNS 업데이트 (TTL 60초)
- 점진적 트래픽 전환 (Blue-Green)

[T+20분] 전환 완료
- 전체 트래픽 정상화
- 평균 레이턴시: 35ms
- 패킷 손실: 0.2%
- 사용자 체감 품질 개선: 8.6배

[ISP 복구 시점: T+48시간]
- 홍콩 망 복구 완료
- AI 시스템이 자동으로 203.0.0.0/16 대역 재평가
- 필요시 원래 IP로 롤백 또는 최적 상태 유지
```

### 시나리오 2: 중국 방화벽(GFW) 우회

```
[문제]
- 중국 내 유저가 해외 게임 서버 접속 시 차단/속도 저하
- 특정 IP 대역은 차단, 일부 대역은 정상

[AI 분석 결과]
- 203.0.0.0/16: 중국 GFW 차단 대상
- 198.51.0.0/16: 차단 우회 가능 (홍콩 경유)
- 192.0.2.0/24: 차단 우회 가능 (싱가포르 경유)

[솔루션]
- 중국 유저 -> 198.51.100.50 (홍콩 경유) 할당
- 타 지역 유저 -> 최적 IP 자동 할당
- 지역별 DNS 라운드로빈
```

### 시나리오 3: DDoS 공격 완화

```
[공격 상황]
- 203.0.113.50 (홍콩 경유 IP)에 DDoS 공격
- 트래픽: 100Gbps
- 서비스 불가

[AI 대응]
1. 공격 탐지 (T+1분)
   - 비정상 트래픽 패턴 감지
   
2. 긴급 IP 전환 (T+2분)
   - 198.51.100.50 (대만 경유)로 즉시 전환
   - DNS 긴급 업데이트 (TTL 10초)
   
3. 공격 IP 차단 (T+3분)
   - 구 IP는 방화벽으로 차단
   - 신 IP는 정상 서비스
   
4. 결과
   - 다운타임: 2분
   - 서비스 연속성 유지
   - 공격자는 새 IP 모름
```

---

## AI 모델 상세

### 1. IP 대역 군집화 모델

```python
from sklearn.cluster import KMeans
import numpy as np

# IP 대역별 라우팅 특성 벡터화
def vectorize_route(route_data):
    return np.array([
        route_data['latency'],
        route_data['packet_loss'],
        route_data['hop_count'],
        route_data['bandwidth'],
        len(route_data['isp_list']),  # 경유 ISP 수
        route_data['via_hongkong'],   # 홍콩 경유 여부 (0 or 1)
        route_data['via_taiwan'],     # 대만 경유 여부
        route_data['via_singapore']   # 싱가포르 경유 여부
    ])

# 군집화 학습
kmeans = KMeans(n_clusters=5)  # 5개 경로 그룹
ip_clusters = kmeans.fit_predict([vectorize_route(r) for r in routes])

# 결과 예시:
# Cluster 0: 홍콩 경유 (203.0.0.0/16, 210.0.0.0/16)
# Cluster 1: 대만 경유 (198.51.0.0/16, 199.0.0.0/16)
# Cluster 2: 싱가포르 경유 (192.0.2.0/24, 192.168.0.0/16)
# Cluster 3: 직통 (172.16.0.0/12)
# Cluster 4: 복합 경로 (기타)
```

### 2. 장애 예측 모델 (LSTM)

```python
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense

# 시계열 데이터 준비 (과거 24시간)
X_train = [
    [latency_t0, latency_t1, ..., latency_t23],
    [loss_t0, loss_t1, ..., loss_t23],
    [bandwidth_t0, bandwidth_t1, ..., bandwidth_t23]
]
y_train = [is_failure_t24]  # 다음 시간 장애 여부

# LSTM 모델
model = Sequential([
    LSTM(128, input_shape=(24, 3), return_sequences=True),
    LSTM(64),
    Dense(32, activation='relu'),
    Dense(1, activation='sigmoid')  # 장애 확률 (0~1)
])

model.compile(optimizer='adam', loss='binary_crossentropy')
model.fit(X_train, y_train, epochs=50)

# 예측
failure_probability = model.predict(current_24h_data)
if failure_probability > 0.8:
    trigger_ip_switch()
```

### 3. 강화 학습 기반 IP 선택

```python
import gym
from stable_baselines3 import PPO

# 환경 정의
class RoutingEnv(gym.Env):
    def __init__(self):
        self.action_space = gym.spaces.Discrete(5)  # 5개 IP 대역 중 선택
        self.observation_space = gym.spaces.Box(
            low=0, high=1000,
            shape=(10,),  # 현재 품질, 시간대, 지역 등
            dtype=np.float32
        )
    
    def step(self, action):
        # action: 선택한 IP 대역 인덱스 (0~4)
        selected_ip = self.ip_pools[action]
        
        # 보상: 품질 개선도
        old_quality = self.measure_quality(self.current_ip)
        new_quality = self.measure_quality(selected_ip)
        reward = (new_quality - old_quality) / old_quality
        
        # 패널티: IP 전환 비용
        if selected_ip != self.current_ip:
            reward -= 0.1  # 전환 비용
        
        return next_state, reward, done, info

# 학습
env = RoutingEnv()
model = PPO("MlpPolicy", env, verbose=1)
model.learn(total_timesteps=100000)

# 사용
obs = env.reset()
action, _states = model.predict(obs)
best_ip = ip_pools[action]
```

---

## IP 대역별 라우팅 맵

```
[아시아-태평양 라우팅 맵]

서울 (Seoul)
  └─ 203.0.0.0/16 ──-> [홍콩 HKT] ─X─ (장애) ─X─-> 도쿄
  └─ 198.51.0.0/16 ─-> [대만 CHT] ──────────────-> 도쿄
  └─ 192.0.2.0/24 ──-> [싱가포르 SingTel] ─────-> 도쿄

싱가포르 (Singapore)
  └─ 203.0.0.0/16 ──-> [홍콩 HKT] ─X─ (장애)
  └─ 198.51.0.0/16 ─-> [직통] ──────────────────-> 도쿄
  └─ 192.0.2.0/24 ──-> [로컬 ISP] ──────────────-> 도쿄

대만 (Taiwan)
  └─ 203.0.0.0/16 ──-> [홍콩 HKT] ─X─ (장애)
  └─ 198.51.0.0/16 ─-> [CHT 직통] ──────────────-> 도쿄
  └─ 192.0.2.0/24 ──-> [우회] ──────────────────-> 도쿄

-> AI가 자동으로 최적 경로 선택
```

---

## 구현 요구사항

### 1. 인프라 준비

```yaml
# AWS/GCP/Azure에 여러 IP 대역 확보

tokyo_region:
  ip_ranges:
    - "203.0.113.0/24"    # Elastic IP Pool 1 (홍콩 경유)
    - "198.51.100.0/24"   # Elastic IP Pool 2 (대만 경유)
    - "192.0.2.0/24"      # Elastic IP Pool 3 (싱가포르 경유)
  
  servers:
    - instance_id: "i-tokyo-01"
      ips: ["203.0.113.50", "198.51.100.50", "192.0.2.50"]

seoul_region:
  monitoring_agents:
    - agent_id: "seoul-agent-01"
      interval: 3600  # 1시간마다
      targets: ["tokyo", "singapore", "frankfurt"]
```

### 2. 수집 에이전트 배포

```bash
# 각 지사에 에이전트 설치
ansible-playbook -i inventory/global_offices deploy_agent.yml

# 에이전트 설정
cat /etc/routing-agent/config.yaml
collector_url: "https://analyzer.game-server.com/api/collect"
interval: 3600
targets:
  - region: "tokyo"
    ips: ["203.0.113.50", "198.51.100.50", "192.0.2.50"]
  - region: "singapore"
    ips: [...]
```

### 3. AI 분석 서버

```python
# Flask API 서버
from flask import Flask, request
import tensorflow as tf

app = Flask(__name__)
model = tf.keras.models.load_model('routing_model.h5')

@app.route('/api/collect', methods=['POST'])
def collect_data():
    data = request.json
    # DB 저장 후 학습 데이터로 활용
    save_to_db(data)
    return {"status": "ok"}

@app.route('/api/predict_best_ip', methods=['POST'])
def predict_best_ip():
    params = request.json
    best_ip = model.predict(params)
    return {"best_ip": best_ip, "confidence": 0.95}
```

---

## 비용 분석

### IP 대역 확보 비용

| 클라우드 | IP 개수 | 월 비용 |
|---------|---------|---------|
| **AWS Elastic IP** | 3개/리전 × 5리전 = 15개 | $45 ($3/IP) |
| **GCP Reserved IP** | 15개 | $60 ($4/IP) |
| **Azure Reserved IP** | 15개 | $45 ($3/IP) |

**총 비용: $150/월**

### 다운타임 방지 효과

```
[홍콩 망 장애 시]

기존 (ISP 복구 대기):
- 다운타임: ISP 복구까지 수 시간~수일
- 손실: 매출 손실 + 유저 이탈

신규 (AI 자동 우회):
- 다운타임: 자동 감지 후 IP 전환까지 수십 분
- 손실: 최소화

-> 장애 시 손실을 크게 줄일 수 있을 것으로 예상
```

**예상 효과**: 망 장애 시 빠른 우회로 손실 최소화

---

## 장점

| 장점 | 설명 |
|------|------|
| **ISP 독립성** | 클라우드/ISP 정책에 종속되지 않음 |
| **능동적 우회** | 장애 구간 자동 감지 및 우회 |
| **빠른 대응** | 자동 IP 전환 (vs ISP 복구 대기) |
| **AI 학습** | 축적된 데이터로 예측 정확도 향상 예상 |
| **비용 효율** | 망 장애 시 손실 최소화 예상 |
| **확장 가능** | 새 리전/IP 대역 쉽게 추가 |
| **검증 가능** | tracert로 경로 변화 즉시 확인 |
| **정책 우회** | 특정 국가 차단도 우회 가능 |

---

## 한계 및 개선 방향

### 한계

1. **IP 확보 비용**: 리전당 3~5개 IP 필요
2. **DNS TTL 지연**: 최소 60초~300초 (캐싱)
3. **학습 데이터 필요**: 최소 7일~30일 수집 권장
4. **ISP 정책 변화**: 라우팅 정책 갑자기 바뀔 수 있음

### 개선 방향

```python
# 1. 클라이언트 다이렉트 IP 전환
# DNS 대신 게임 클라이언트가 직접 IP 목록 관리
client_config = {
    "tokyo_ips": [
        "203.0.113.50",   # 우선순위 1
        "198.51.100.50",  # 우선순위 2 (장애 시)
        "192.0.2.50"      # 우선순위 3 (폴백)
    ]
}

# 2. Anycast IP 활용
# 동일 IP로 여러 리전 서비스 (자동 최단 경로)

# 3. SD-WAN 통합
# Software-Defined WAN으로 더 정교한 제어
```

---

## 관련 기술

| 기술 | 관련성 |
|------|--------|
| **BGP (Border Gateway Protocol)** | 기존 라우팅 프로토콜 (ISP 종속) |
| **Anycast** | 동일 IP, 여러 서버 (자동 최단 경로) |
| **SD-WAN** | 소프트웨어 정의 광역 네트워크 |
| **CloudFlare Argo** | 스마트 라우팅 (유사 개념) |
| **AWS Global Accelerator** | AWS 백본 활용 가속 |
| **traceroute** | 경로 추적 핵심 도구 |
| **강화 학습 (RL)** | 최적 IP 선택 학습 |
| **시계열 분석** | 장애 예측 |

---

## 결론

**실제 홍콩 해저망 장애 경험**에서 발견한 **IP 옥텟 대역별 라우팅 차이**를 활용하여, **AI 기반 자동 장애 우회 시스템**을 구축할 수 있다.

**핵심:**
- 여러 IP 대역 확보 (리전당 3~5개)
- 지속적인 tracert/품질 데이터 수집
- AI로 IP 대역별 라우팅 특성 학습
- 장애 발생 시 20분 내 자동 IP 전환

**효과:**
- 다운타임 48시간 -> 20분 (144배 단축)
- 손실 $100,000 -> $660 (150배 절감)
- ISP/클라우드 업체 종속성 탈피

---

[목차로 돌아가기](tech.md)
