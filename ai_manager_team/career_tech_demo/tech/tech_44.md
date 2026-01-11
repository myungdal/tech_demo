# 44. LockGuard 소유권 전달 구조를 통한 편리한 코딩

작성자: 안명달 (mooondal@gmail.com)

## 1. 개요

어떤 데이터에 관한 Lock을 걸면서 여러 함수를 오가는 구현을 하다 보면 위험할 경우가 많아서 주석을 주렁주렁 달곤 했다. 락 가드에 적절한 이름을 기입하여 객체로 넘겨주면 편리하지 않을까 하는 아이디어에서 시작했다.

일반적인 `std::lock_guard` 사용법을 넘어서, **Lock을 일급 객체(first-class object)로 취급**하여 함수에서 반환하거나 인수로 전달하는 패턴이다.

이 패턴을 사용하면 **함수 시그니처만 보고도 락 요구 사항을 알 수 있고**, 락 없이 호출하면 **컴파일 에러**가 발생하여 런타임 버그를 방지한다.

---

## 2. 기존 방식의 문제점

### 2.1 일반적인 Lock 사용

```cpp
class UserManager
{
private:
    std::mutex mMutex;
    std::unordered_map<UserId, UserData> mUserMap;
    
public:
    void UpdateUser(UserId userId, const UserData& data)
    {
        std::lock_guard<std::mutex> lock(mMutex);  // 락 획득
        mUserMap[userId] = data;
    }
    
    UserData GetUser(UserId userId)
    {
        std::lock_guard<std::mutex> lock(mMutex);  // 락 획득
        return mUserMap[userId];
    }
    
    // [BAD] 문제: 이 함수는 락이 필요한가? 시그니처만 봐서는 알 수 없음
    void ProcessUser(UserId userId);
};
```

### 2.2 문제점

| 문제 | 설명 |
|------|------|
| **락 요구 사항 불명확** | 함수 시그니처만 보고는 락이 필요한지 알 수 없음 |
| **중복 락 위험** | 이미 락을 건 상태에서 내부 함수 호출 시 데드락 |
| **락 누락 위험** | 락 없이 호출해도 컴파일됨 -> 런타임 버그 |

---

## 3. 핵심 아이디어: Lock 객체화

Lock을 값 타입 객체로 만들어 **소유권을 명시적으로 전달**한다.

### 3.1 LockGuard 클래스

```cpp
template<typename MutexType>
class LockGuard
{
private:
    std::unique_lock<MutexType> mLock;
    
public:
    // 명시적 생성 (락 획득)
    explicit LockGuard(MutexType& mutex)
        : mLock(mutex)
    {}
    
    // 이동 가능 (소유권 전달)
    LockGuard(LockGuard&& other) noexcept = default;
    LockGuard& operator=(LockGuard&& other) noexcept = default;
    
    // 복사 금지
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
    
    // 락 상태 확인
    bool OwnsLock() const { return mLock.owns_lock(); }
    
    // 소멸 시 자동 해제 (RAII)
    ~LockGuard() = default;
};

using UserLockGuard = LockGuard<std::mutex>;
```

---

## 4. Lock 반환 패턴

함수가 Lock을 획득하고 **호출자에게 소유권을 이전**한다.

### 4.1 구현

```cpp
class UserManager
{
private:
    std::mutex mMutex;
    std::unordered_map<UserId, UserData> mUserMap;
    
public:
    // 락을 획득하고 반환 (소유권 이전)
    [[nodiscard]] UserLockGuard AcquireLock()
    {
        return UserLockGuard(mMutex);
    }
    
    // 락이 있는 상태에서만 호출 가능한 내부 함수
    UserData& GetUserUnsafe(UserId userId)
    {
        return mUserMap[userId];
    }
};
```

### 4.2 사용 예시

```cpp
void ProcessUserData(UserManager& manager, UserId userId)
{
    // 락 획득 - [[nodiscard]]로 무시하면 경고
    auto lock = manager.AcquireLock();
    
    // 락이 있는 상태에서 안전하게 접근
    auto& user = manager.GetUserUnsafe(userId);
    user.level += 1;
    user.exp = 0;
    
    // 스코프 끝에서 자동 해제
}
```

---

## 5. Lock 인수 패턴

함수가 **Lock을 인수로 요구**하여, 호출자가 락을 보유하고 있음을 강제한다.

### 5.1 구현

```cpp
class UserManager
{
public:
    // 락을 인수로 받음 - 호출자가 락을 보유해야만 호출 가능
    void UpdateUserWithLock(UserLockGuard&& lock, UserId userId, const UserData& data)
    {
        // lock이 있으므로 안전하게 접근
        mUserMap[userId] = data;
        
        // lock은 이 함수 스코프 끝에서 해제
    }
    
    // const 참조로 받으면 락 유지 (호출자가 계속 사용)
    UserData GetUserWithLock(const UserLockGuard& lock, UserId userId) const
    {
        return mUserMap.at(userId);
    }
};
```

### 5.2 사용 예시

```cpp
void Example(UserManager& manager, UserId userId)
{
    auto lock = manager.AcquireLock();
    
    // 락을 전달하면서 호출
    auto user = manager.GetUserWithLock(lock, userId);  // lock 유지
    
    user.gold += 1000;
    
    // 락 소유권 이전 (이후 lock 사용 불가)
    manager.UpdateUserWithLock(std::move(lock), userId, user);
    
    // [BAD] 컴파일 에러 또는 런타임 체크 실패
    // manager.GetUserWithLock(lock, userId);  // lock은 이미 이동됨
}
```

---

## 6. 컴파일 타임 안전성

### 6.1 락 없이 호출 시 컴파일 에러

```cpp
void UpdateUserWithLock(UserLockGuard&& lock, UserId userId, const UserData& data);

// 사용
manager.UpdateUserWithLock(/* 락 없이 호출 */, userId, data);
// [BAD] 컴파일 에러: 인수 누락

manager.UpdateUserWithLock(manager.AcquireLock(), userId, data);
// [GOOD] OK: 락 획득 후 전달
```

### 6.2 [[nodiscard]]로 락 무시 방지

```cpp
[[nodiscard]] UserLockGuard AcquireLock();

// 사용
manager.AcquireLock();  // [WARN] 경고: 반환값 무시됨 (즉시 해제)

auto lock = manager.AcquireLock();  // [GOOD] OK
```

---

## 7. 실전 사용 예시

### 7.1 복합 트랜잭션

```cpp
void TransferGold(UserManager& manager, UserId fromId, UserId toId, int64_t amount)
{
    // 단일 락으로 두 유저 동시 접근
    auto lock = manager.AcquireLock();
    
    auto& fromUser = manager.GetUserUnsafe(fromId);
    auto& toUser = manager.GetUserUnsafe(toId);
    
    if (fromUser.gold < amount)
    {
        throw InsufficientGoldException();
    }
    
    fromUser.gold -= amount;
    toUser.gold += amount;
    
    // 락은 스코프 끝에서 자동 해제
}
```

### 7.2 조건부 락 해제

```cpp
void ProcessWithEarlyUnlock(UserManager& manager, UserId userId)
{
    auto lock = manager.AcquireLock();
    
    auto& user = manager.GetUserUnsafe(userId);
    auto snapshot = user;  // 복사본 저장
    
    // 락 조기 해제 (더 이상 공유 자원 접근 안 함)
    lock = UserLockGuard();  // 빈 락으로 교체 -> 기존 락 해제
    
    // 락 없이 무거운 작업 수행
    DoHeavyProcessing(snapshot);
}
```

### 7.3 계층적 락 (Lock Hierarchy)

```cpp
// 타입으로 락 계층 구분
using UserLock = LockGuard<UserMutex>;
using GuildLock = LockGuard<GuildMutex>;

// 길드 락 -> 유저 락 순서 강제
void UpdateGuildMember(GuildLock&& guildLock, UserLock&& userLock, ...)
{
    // 함수 시그니처로 락 순서가 명시됨
}
```

---

## 8. 장점

| 장점 | 설명 |
|------|------|
| **API 명확화** | 함수 시그니처만 보고 락 요구 사항 파악 가능 |
| **컴파일 타임 안전성** | 락 없이 호출 시 컴파일 에러 |
| **실수 방지** | 락 획득 누락, 중복 락 등 런타임 버그 방지 |
| **RAII 유지** | 스코프 끝에서 자동 해제 |
| **소유권 명시** | 이동(move) vs 참조(ref)로 락 수명 의도 표현 |
| **문서화 효과** | 코드 자체가 락 계약을 문서화 |

---

## 9. 주의사항

### 9.1 이동 후 사용 방지

```cpp
auto lock = manager.AcquireLock();
manager.UpdateUserWithLock(std::move(lock), userId, data);

// [BAD] 위험: lock은 이미 이동됨
auto& user = manager.GetUserUnsafe(userId);  // 락 없이 접근!
```

**해결**: `OwnsLock()` 체크 또는 이동 후 접근 불가하도록 설계

### 9.2 성능 오버헤드

- `std::unique_lock`은 `std::lock_guard`보다 약간 무거움 (이동 지원 때문)
- 대부분의 경우 무시할 수 있는 수준 (나노초 단위)


