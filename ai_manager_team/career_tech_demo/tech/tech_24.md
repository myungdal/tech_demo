# 24. 자체 개발 패킷을 통한 로그 자동화, 보안 검증 자동화, 재접속 관리

작성자: 안명달 (mooondal@gmail.com)

> **목차로 돌아가기**: [tech.md](tech.md)

---

## 개요

자체 개발한 패킷 시스템을 통해 클라이언트와 서버에 패킷이 투명하게 보이며, 재접속 여부, 보안 위반, 패킷 순서 오류 등을 자동으로 검증하고 로그로 기록하는 통합 시스템이다.

### 핵심: 패킷 투명성 - 클라이언트와 서버에 모든 패킷이 보인다

1. **디버깅 용이**: 클라이언트 로그와 서버 로그를 대조하여 패킷 유실, 순서 오류, 타이밍 문제를 즉시 파악
2. **보안 감사**: 모든 패킷이 기록되어 해킹/어뷰징 시도를 사후 추적 가능
3. **성능 분석**: 패킷 처리 시간, 빈도, 크기 등을 통계로 분석

#### 실제 게임 서버 로그 예시 (Build/log/GAME_SERVER/*.log)

```log
[2026-01-05 15:30:12.345] [RECV] CF_REQ_LOGIN {userId: "player001", token: "abc123..."}
[2026-01-05 15:30:12.346] [SEND] FC_ACK_LOGIN {result: SUCCESS, accountId: 123456}
[2026-01-05 15:30:12.347] [RECV] CF_REQ_ENTER_GAME {serverId: 1, channelIdx: 0}
[2026-01-05 15:30:12.348] [SEND] FG_REQ_USER_ENTER {userId: 123456, worldUserId: 7890}
[2026-01-05 15:30:12.349] [RECV] GF_ACK_USER_ENTER {result: SUCCESS, spawnPos: (100, 200)}
[2026-01-05 15:30:12.350] [SEND] FC_ACK_ENTER_GAME {result: SUCCESS, entityId: 7890}
[2026-01-05 15:30:12.351] [SEND] GC_SPAWN_ENTITY {entityId: 7890, pos: (100, 200), hp: 100}
[2026-01-05 15:30:13.123] [RECV] CG_REQ_MOVE {targetPos: (150, 250)}
[2026-01-05 15:30:13.124] [SEND] GC_MOVE_ENTITY {entityId: 7890, path: [(100,200), (150,250)]}
[2026-01-05 15:30:14.567] [RECV] CG_REQ_ATTACK {targetId: 9999}
[2026-01-05 15:30:14.568] [SEND] GC_ATTACK {attackerId: 7890, targetId: 9999, damage: 50}
[2026-01-05 15:30:14.569] [SEND] GC_HP_CHANGE {entityId: 9999, hp: 50}
```

-> **클라이언트 로그와 서버 로그를 대조하면 어느 시점에 무슨 패킷이 오갔는지 한눈에 파악 가능**

### 핵심 아이디어

**문제:**
- 재접속 시 사용자 상태 검증 복잡
- 패킷 순서 보장 필요
- 서버 간 통신 경로 추적 어려움
- 보안 위반 탐지 지연

**해결:**
```cpp
class PacketHeader
{
    // 서버 라우팅 정보
    std::array<AppId, 3> mAppIds;                   // DB/Front/Game 서버 ID
    std::array<RpcId, 4> mRemoteSocketIds;          // 응답 라우팅 ID
    std::array<RpcId, 2> mRemoteRequestIds;         // RPC 요청 ID
    
    // 순서 보장
    DbShardIdx mDbShardIdx;                         // DB 샤드 인덱스
    DbSeq mDbSeq;                                   // 순차 증가 시퀀스
    
    // 사용자 식별
    AccountId mAccountId;                           // 계정 ID
    ServerId mServerId;                             // 서버 ID
    UserId mUserId;                                 // 유저 ID
    GuildId mGuildId;                               // 길드 ID
    
    // 게임 세션
    GameId mGameId;                                 // 게임 ID
    GameChannelIndex mGameChannelIndex;             // 채널 인덱스
    WorldUserId mWorldUserId;                       // 월드 유저 ID
    
    Result mPacketResult;                           // 처리 결과
};
```

-> **헤더만으로 모든 검증 완료**

---

## PacketHeader 구조

### 1. 서버 라우팅 정보

```cpp
// 패킷이 거쳐온 서버들의 ID
std::array<AppId, 3> mAppIds;
/*
 * [0]: DB_SERVER
 * [1]: FRONT_SERVER
 * [2]: GAME_SERVER
 */

// 응답 라우팅을 위한 소켓 ID
std::array<RpcId, 4> mRemoteSocketIds;
/*
 * [0]: DB_SERVER     - DB 응답 라우팅
 * [1]: FRONT_SERVER  - 클라이언트 소켓 ID
 * [2]: GAME_SERVER   - Game 응답 라우팅
 * [3]: MAIN_SERVER   - Main 응답 라우팅
 */

// RPC 요청 추적 ID
std::array<RpcId, 2> mRemoteRequestIds;
/*
 * [0]: DB_SERVER   - DB RPC 요청 ID
 * [1]: MAIN_SERVER - Main RPC 요청 ID
 */
```

**예시: 클라이언트 -> Front -> Game -> DB 경로**
```
패킷 헤더:
mAppIds[FRONT_SERVER] = 1     // Front서버 ID: 1
mAppIds[GAME_SERVER] = 5      // Game서버 ID: 5
mAppIds[DB_SERVER] = 10       // DB서버 ID: 10

mRemoteSocketIds[FRONT_SERVER] = {idx: 42, token: 1234}  // 클라이언트 소켓 ID
mRemoteSocketIds[GAME_SERVER] = {idx: 15, token: 5678}   // Game 소켓 ID
```

-> **응답 시 역경로로 자동 라우팅**

### 2. 순서 보장

```cpp
DbShardIdx mDbShardIdx;  // DB 샤드 인덱스 (0~N)
DbSeq mDbSeq;            // 순차 증가 시퀀스 (1, 2, 3, ...)
```

**DbSeq 검증 알고리즘:**
```cpp
bool DbUserSocketState::ValidateDbSeq(const PacketHeader& packetHeader) const
{
    // DB 샤드가 같은지 확인
    const bool validateDbShardIdx = (mCurrPacketHeader.GetDbShardIdx() == packetHeader.GetDbShardIdx());
    
    // DbSeq가 일치하는지 확인 (순서 보장)
    const bool validateDbSeq = (mCurrPacketHeader.GetDbSeq() == packetHeader.GetDbSeq());
    
    return (validateDbShardIdx && validateDbSeq);
}

// DbSeq 증가 (패킷 전송 전)
DbSeq DbUserSocketState::AddDbSeq()
{
    DbSeq dbSeq = mCurrPacketHeader.GetDbSeq();
    ++dbSeq;
    if (0 == dbSeq)  // 오버플로우 방지
        ++dbSeq;
    
    mCurrPacketHeader.SetDbSeq(dbSeq);
    return dbSeq;
}
```

**DbSeq 흐름**
```
클라이언트 -> Front -> Game -> DB
         DbSeq: 1
                    DbSeq: 1
                            DbSeq: 1 검증 성공

DB -> Game -> Front -> 클라이언트
  DbSeq: 2 (증가)
        DbSeq: 2
              DbSeq: 2
                    DbSeq: 2
만약 DbSeq: 3인 패킷이 DbSeq: 1 상태에 도착하면?
-> ValidateDbSeq 실패 (패킷 순서 오류)
```

### 3. 사용자 식별 정보

```cpp
AccountId mAccountId;  // 계정 UUID
ServerId mServerId;    // 서버 ID (0~N)
UserId mUserId;        // 유저 UUID
GuildId mGuildId;      // 길드 UUID
```

**ValidateState 검증:**
```cpp
bool PacketHeader::ValidateState(const PacketHeader& other) const
{
    return
        mAppIds == other.mAppIds &&           // 서버 경로 일치
        
        mDbShardIdx == other.mDbShardIdx &&   // DB 샤드 일치
        
        mAccountId == other.mAccountId &&     // 계정 일치
        mServerId == other.mServerId &&       // 서버 일치
        mUserId == other.mUserId &&           // 유저 일치
        mGuildId == other.mGuildId &&         // 길드 일치
        
        mGameId == other.mGameId &&           // 게임 일치
        mGameChannelIndex == other.mGameChannelIndex;  // 채널 일치
}
```

**검증 실패 시나리오:**
```cpp
// Transactor에서 자동 검증
Result UserAndPacketTransactor::OnPreValidate()
{
    // 1. DbSeq 검증
    if (false == mUserSocketState.ValidateDbSeq(GetCurrPacketHeader()))
        return Result::DB_SEQ_ERROR;
    
    // 2. 상태 검증
    if (false == mUserSocketState.ValidateState(GetCurrPacketHeader()))
    {
        PacketHeader& my = mUserSocketState.GetCurrPacketHeader();
        PacketHeader& other = GetCurrPacketHeader();
        
        // 불일치 항목 로깅
        if (my.mAccountId != other.mAccountId)
            _DEBUG_LOG(RED, L"AccountId 불일치: {} vs {}", my.mAccountId, other.mAccountId);
        
        if (my.mUserId != other.mUserId)
            _DEBUG_LOG(RED, L"UserId 불일치: {} vs {}", my.mUserId, other.mUserId);
        
        // ... 모든 항목 검증
        
        return Result::STATE_VALIDATION_ERROR;
    }
    
    return Result::SUCCEEDED;
}
```

### 4. 게임 세션 정보

```cpp
GameId mGameId;                      // 게임 방 UUID
GameChannelIndex mGameChannelIndex;  // 채널 인덱스 (0~N)
WorldUserId mWorldUserId;            // 월드 내 유저 ID
```

---

## 재접속 검증 시스템

### 1. 재접속 흐름

```
[정상 연결]
클라이언트 -> Front -> 로그인 -> PacketHeader 생성
                                 ↓
                         AccountId: 1234
                         UserId: 5678
                         DbSeq: 1
                         AuthTicket: ABC123

[연결 끊김]
네트워크 장애 발생
↓

[재접속 시도]
클라이언트 -> Front -> 재로그인 (AuthTicket: ABC123)
                       ↓
                 [PacketHeader 검증]
                 - AuthTicket 일치?                 - AccountId 일치?                 - UserId 일치?                 - DbSeq 연속성?                       ↓
                 재접속 성공 -> 저장된 PacketHeader 복원
```

### 2. Bot 재접속 예시

```cpp
class BotSceneReconnect
{
    void CF_REQ_USER_LOGIN()
    {
        // [중요] 저장된 PacketHeader로 재로그인
        BotSocketUtil::SendToFront<CF_REQ_USER_LOGIN::Writer> wp(*this, REQ);
        wp.SetHeader(GetBotContext().CurrPacketHeader());  // 이전 헤더 재사용
        wp.SetValues(
            GetBotContext().AuthTicket()  // 저장된 AuthTicket
        );
    }
    
    void OnHandshakeCompleted(bool reconnected)
    {
        if (reconnected)
        {
            // 재접속 성공 -> 이전 상태로 복원
            _DEBUG_LOG(WHITE, L"재접속 성공: UserId={}", GetBotContext().GetUserId());
        }
    }
};
```

### 3. 서버 측 재접속 검증

```cpp
// Front서버: 재접속 요청 처리
HandleResult FrontPacketHandlerLogin::OnPacket_CF_REQ_USER_LOGIN(
    SocketFrontFromClient& clientSocket,
    CF_REQ_USER_LOGIN& rp)
{
    AuthTicket authTicket = rp.Get_authTicket();
    
    // 1. AuthTicket으로 이전 세션 조회
    FrontUser* user = gFrontDataManager->FindUserByAuthTicket(authTicket);
    if (nullptr == user)
        return HandleResult::AUTH_FAILED;
    
    // 2. PacketHeader 검증
    const PacketHeader& storedHeader = user->GetStoredPacketHeader();
    const PacketHeader& requestHeader = rp.GetHeader();
    
    if (false == storedHeader.ValidateState(requestHeader))
    {
        // AccountId, UserId 등 불일치 -> 위조된 재접속 시도
        _DEBUG_LOG(RED, L"재접속 검증 실패: 헤더 불일치");
        return HandleResult::STATE_VALIDATION_ERROR;
    }
    
    // 3. 재접속 성공 -> 소켓 교체
    user->SetSocket(&clientSocket);
    clientSocket.SetUserId(user->GetUserId());
    
    // 4. 응답 (저장된 PacketHeader로)
    SocketUtil::Send<CF_ACK_USER_LOGIN::Writer> wp(clientSocket, ACK);
    wp.SetHeader(storedHeader);  // 이전 헤더 그대로 반환
    wp.SetValues(Result::SUCCEEDED);
    
    return HandleResult::OK;
}
```

---

## 보안 검증

### 1. 암호화 키 교환 (Handshake)

```cpp
// PeerSocket (클라이언트 역할)
Result PeerSocket::OnRecv(uint8_t* buf, size_t len, OUT size_t& recved)
{
    if (0 == mHostEncryptionKey)  // 아직 Handshake 안 됨
    {
        NetworkPacket* ptr = reinterpret_cast<NetworkPacket*>(buf);
        
        // 1. Handshake 패킷만 허용
        if (PacketTypes::HANDSHAKE_TO_PEER != ptr->GetPacketType())
        {
            _DEBUG_BREAK;
            return Result::NEED_HANDSHAKE;  // 보안 위반
        }
        
        HANDSHAKE_TO_PEER* rp = reinterpret_cast<HANDSHAKE_TO_PEER*>(buf);
        
        // 2. 내 암호화 키 생성
        mMyEncryptionKey = (*tRandomEngine32)();
        if (0 == mMyEncryptionKey)
            mMyEncryptionKey = 1;
        
        // 3. 응답 전송 (암호화 없이)
        SocketUtil::Send<HANDSHAKE_TO_HOST::Writer, EncryptionOption::NO_ENCRYPTION> wp(*this, NOTIFY);
        wp.SetValues(
            mMyEncryptionKey,  // 내 키
            mMyAppId,
            mMyAppType
        );
        
        // 4. 상대방 키 저장
        mHostEncryptionKey = rp->Get_hostEncryptionKey();
        
        OnHandshakeCompleted(mIsReconnect);
    }
    else  // Handshake 완료 -> 암호화 통신
    {
        // 모든 패킷 암호화/복호화
        OnEncrypt(buf, sizeof(NetworkPacket), mMyEncryptionKey);
        // ...
    }
}
```

**Handshake 프로토콜**
```
[Client]                          [Server]
   |                                   |
   |  (1) Connect                      |
   |---------------------------------->|
   |                                   |
   |  (2) HANDSHAKE_TO_PEER            |
   |         (serverKey=9876)          |
   |<----------------------------------|
   |                                   |
   |  (3) HANDSHAKE_TO_HOST            |
   |         (clientKey=1234)          |
   |---------------------------------->|
   |                                   |
   |  (4) 이후 모든 패킷 암호화        |
   |   XOR(data, clientKey)            |
   |<=================================>|
   |   XOR(data, serverKey)            |
```

### 2. Handshake 미완료 패킷 차단

```cpp
// HostSocket (서버 역할)
Result HostSocket::OnRecv(uint8_t* buf, size_t len, OUT size_t& recved)
{
    if (0 == mPeerEncryptionKey)  // Handshake 안 됨
    {
        NetworkPacket* ptr = reinterpret_cast<NetworkPacket*>(buf);
        
        if (PacketTypes::HANDSHAKE_TO_HOST != ptr->GetPacketType())
        {
            _DEBUG_RED;
            return Result::NEED_HANDSHAKE;  // 보안 위반
        }
        
        // Handshake 처리...
    }
    else  // Handshake 완료 -> 모든 패킷 허용
    {
        OnEncrypt(buf, sizeof(NetworkPacket), mMyEncryptionKey);
        // 패킷 처리...
    }
}
```

---

## 실전 시나리오

### 시나리오 1: 정상 재접속

```
1. 유저 플레이 중:
   PacketHeader:
   - AccountId: 1234
   - UserId: 5678
   - DbSeq: 100
   - AuthTicket: ABC123

2. 네트워크 끊김:
   -> Front서버: 소켓 연결 해제
   -> DB에 PacketHeader 저장 (AccountId: 1234)

3. 재접속:
   클라이언트 -> CF_REQ_USER_LOGIN (AuthTicket: ABC123)
   
4. 서버 검증:
   - AuthTicket으로 이전 세션 조회   - AccountId 일치 (1234 == 1234)   - UserId 일치 (5678 == 5678)   - DbSeq 연속성 (100 -> 101)   
5. 재접속 성공:
   -> 저장된 PacketHeader 복원
   -> DbSeq: 101부터 재개
   -> 게임 플레이 계속
```

### 시나리오 2: 위조된 재접속 시도

```
1. 해커가 AuthTicket 탈취:
   AuthTicket: ABC123 (다른 유저)

2. 재접속 시도:
   해커 클라이언트 -> CF_REQ_USER_LOGIN (AuthTicket: ABC123)
   PacketHeader:
   - AccountId: 9999  (해커 계정)
   - UserId: 8888     (해커 유저)

3. 서버 검증:
   - AuthTicket으로 이전 세션 조회   - 저장된 PacketHeader:
     * AccountId: 1234  (원래 유저)
     * UserId: 5678     (원래 유저)
   
4. ValidateState 실패:
   - AccountId 불일치 (1234 != 9999)   - UserId 불일치 (5678 != 8888)   
5. 재접속 거부:
   -> Result::STATE_VALIDATION_ERROR
   -> 로그 기록: "헤더 위조 시도 감지"
   -> 해커 IP 차단
```

### 시나리오 3: 패킷 순서 오류 탐지

```
1. 정상 패킷 순서:
   DbSeq: 1 -> 2 -> 3 -> 4 -> 5

2. 네트워크 지연으로 순서 뒤바뀜:
   DbSeq: 1 -> 2 -> 4 -> 3 -> 5
                    ↑ 순서 오류!

3. DB서버 검증:
   mCurrDbSeq: 2
   수신 패킷 DbSeq: 4
   -> ValidateDbSeq 실패   
4. 처리:
   - Result::DB_SEQ_ERROR 반환
   - 패킷 버퍼링 또는 재전송 요청
   - 로그 기록: "패킷 순서 오류 (예상: 3, 실제: 4)"
```

### 시나리오 4: 서버 간 경로 검증

```
1. 정상 경로:
   Client -> Front(1) -> Game(5) -> DB(10)
   
   PacketHeader:
   mAppIds[FRONT_SERVER] = 1
   mAppIds[GAME_SERVER] = 5
   mAppIds[DB_SERVER] = 10

2. 비정상 경로 (Front 우회):
   Client -> Game(5) -> DB(10)
   
   PacketHeader:
   mAppIds[FRONT_SERVER] = INVALID_APP_ID    mAppIds[GAME_SERVER] = 5
   mAppIds[DB_SERVER] = 10

3. DB서버 검증:
   if (packetHeader.GetAppId(AppType::FRONT_SERVER) == INVALID_APP_ID)
   {
       _DEBUG_LOG(RED, L"Front서버를 거치지 않은 패킷 감지");
       return Result::INVALID_ROUTE;
   }

4. 처리:
   - 패킷 거부
   - 로그 기록: "비정상 경로 패킷"
   - 해당 Game서버 조사
```

---

## 성능 최적화

### 1. 헤더 크기 최적화

```cpp
#pragma pack(push, 1)  // 1바이트 정렬로 메모리 절약
class PacketHeader
{
    // 배열 대신 고정 크기 사용
    std::array<AppId, 3> mAppIds;              // 12 bytes (4*3)
    std::array<RpcId, 4> mRemoteSocketIds;     // 32 bytes (8*4)
    std::array<RpcId, 2> mRemoteRequestIds;    // 16 bytes (8*2)
    
    DbShardIdx mDbShardIdx;                    // 2 bytes
    DbSeq mDbSeq;                              // 8 bytes
    
    AccountId mAccountId;                      // 8 bytes (UUID)
    ServerId mServerId;                        // 2 bytes
    UserId mUserId;                            // 8 bytes (UUID)
    GuildId mGuildId;                          // 8 bytes (UUID)
    
    GameId mGameId;                            // 8 bytes (UUID)
    GameChannelIndex mGameChannelIndex;        // 2 bytes
    WorldUserId mWorldUserId;                  // 4 bytes
    
    Result mPacketResult;                      // 2 bytes
    
    // 총 크기: 약 110 bytes
};
#pragma pack(pop)
```

### 2. 검증 성능

| 검증 항목 | 복잡도 | 설명 |
|----------|--------|------|
| **ValidateState** | O(1) | 단순 정수/UUID 비교 (8개 필드) |
| **ValidateDbSeq** | O(1) | DbShardIdx + DbSeq 비교 (2개 필드) |
| **전체 검증** | **O(1)** | 모든 검증 합쳐도 O(1) |

**실측 성능:**
```cpp
// 100만 번 검증 벤치마크
for (int i = 0; i < 1000000; ++i)
{
    bool valid = header1.ValidateState(header2);
}

// 결과: 약 5ms (1번당 5ns)
// -> 패킷 처리에 거의 영향 없음
```

---

## 장점

| 장점 | 설명 |
|------|------|
| **즉시 검증** | 헤더만 보고 재접속/보안 위반 즉시 판단 |
| **패킷 순서 보장** | DbSeq로 순서 오류 감지 |
| **경로 추적** | mAppIds로 서버 경로 완벽 추적 |
| **위조 방지** | ValidateState로 헤더 위조 차단 |
| **재접속 간편** | 저장된 헤더로 상태 즉시 복원 |
| **O(1) 검증** | 모든 검증이 상수 시간 |
| **디버깅 용이** | 헤더 로그만으로 문제 파악 |
| **서버 간 통신** | RemoteSocketIds로 자동 응답 라우팅 |

---

## 구현 시 고려사항

### 1. DbSeq 오버플로우

```cpp
DbSeq DbUserSocketState::AddDbSeq()
{
    DbSeq dbSeq = mCurrPacketHeader.GetDbSeq();
    ++dbSeq;
    
    // [중요] 오버플로우 시 0 건너뛰기
    if (0 == dbSeq)
        ++dbSeq;
    
    mCurrPacketHeader.SetDbSeq(dbSeq);
    return dbSeq;
}
```

### 2. 재접속 타임아웃

```cpp
class FrontUser
{
    std::chrono::steady_clock::time_point mLastAccessTime;
    
    bool IsReconnectTimeout() const
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - mLastAccessTime);
        
        // 10분 이상 재접속 없으면 타임아웃
        return (elapsed.count() > 600);
    }
};
```

### 3. 헤더 로깅

```cpp
template<>
struct std::formatter<PacketHeader>
{
    auto format(const PacketHeader& h, auto& ctx) const
    {
        std::wstring result;
        
        if (h.mAppIds[0] != INVALID_APP_ID)
            result += std::format(L"\n  AppIds[DB]: {}", h.mAppIds[0]);
        
        if (h.GetDbSeq() != INVALID_DB_SEQ)
            result += std::format(L"\n  DbSeq: {}", h.GetDbSeq());
        
        if (h.GetUserId() != INVALID_UUID)
            result += std::format(L"\n  UserId: {}", h.GetUserId());
        
        // ... 모든 필드 출력
        
        return std::ranges::copy(result, ctx.out()).out;
    }
};

// 사용
_DEBUG_LOG(WHITE, L"PacketHeader: {}", packetHeader);
```

---

[목차로 돌아가기](tech.md)
