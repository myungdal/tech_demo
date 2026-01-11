# ServerEngine 파티션별 PCH 의존성 분석

## 현재 상태

모든 ServerEngine 헤더 파일은 **pch_common.h**를 활용합니다:

- 표준 라이브러리 헤더 (`<atomic>`, `<memory>`, `<vector>` 등) - pch에서 제공
- Windows 헤더 (`Windows.h`, `winhttp.h` 등) - pch에서 제공
- NOMINMAX, WIN32_LEAN_AND_MEAN 매크로 - pch에서 정의

개별 헤더에서는 중복 include를 제거하고 pch를 활용합니다.

---

## 파티션별 상태

### GMF 최소화 완료

| 파티션 | GMF 헤더 | 상태 |
|--------|----------|------|
| **:Core** | `CoreTypes.h` | ? 완료 |
| **:Lock** | `Lock.h`, `ReadLock.h`, `WriteLock.h` | ? 완료 |
| **:Exception** | `Exception.h` | ? 완료 |
| **:Http** | `HttpClient.h` | ? 완료 |
| **:Thread** | `Thread.h`, `*Thread.h`, `WorkerThreadManager.h` | ? 완료 |
| **:Worker** | `pch_packet.h` + Worker 관련 헤더 | ? 완료 |

### pch_packet.h 필요 (완전 분리 어려움)

| 파티션 | 의존성 | 사유 |
|--------|--------|------|
| **:Clock** | ClockPoint, TIMESTAMP_STRUCT | pch_common.h 커스텀 타입 의존 |
| **:Log** | LogColor, 포맷 매크로 | 매크로/타입 의존 |
| **:Socket** | OVERLAPPED, WSABUF, 네트워크 API | Windows + 커스텀 타입 |
| **:Network** | SendBuffer, IOCP | 복잡한 의존성 |
| **:DB** | SQLLEN, ODBC API | SQL API 의존 |
| **:Util** | 다양한 유틸리티 | 광범위한 의존성 |
| **:AppBase** | AppType, AppId | pch_packet.h 정의 |
| **:PacketTransactor** | 패킷 처리 | Packet 의존 |

---

## PCH 체인

```
pch_common.h
    ├── 표준 라이브러리 (<atomic>, <memory>, <vector>, ...)
    ├── Windows.h (NOMINMAX, WIN32_LEAN_AND_MEAN)
    ├── winhttp.h, winsock2.h, sql.h, ...
    └── Common/*.h

pch_packet.h
    └── #include "pch_common.h"
    └── Packet/*.h

pch_serverEngine.h
    └── #include "pch_packet.h"
    └── ServerEngine/*.h (일부)
    └── CoreTypes.h
    └── thread_local 변수들 (tThreadId, tLockedCount)
    └── 매크로 (_YIELD_SPIN, _ASSERT_CRASH)
```

---

## 정리된 헤더 목록

다음 헤더에서 중복 include 제거 완료:

- `Exception.h` - NOMINMAX, WIN32_LEAN_AND_MEAN, Windows.h 제거
- `HttpClient.h` - 위와 동일 + winhttp.h 제거
- `Thread.h` - Windows.h 제거
- `ConsoleThread.h` - Windows.h, functional, future 제거
- `WorkerThread.h`, `IocpThread.h` - memory 제거
- `WorkerThreadManager.h` - memory, vector 제거
- `Lock.h` - atomic, cstdint 제거
- `CoWorker.h`, `Worker.h`, `CronWorker.h`, `ThreadWorker.h` - 표준 헤더 제거
- `SafeMap.h`, `WorkerTask.h` - 표준 헤더 제거
- `AppListManager.h`, `AppConfigManager.h` - 표준 헤더 제거
- `RequestSocket.h`, `BypassSocket.h` - 표준 헤더 제거
- `CoreTypes.h` - cstdint, cstddef 제거

---

## 원칙

1. **pch_common.h가 제공하는 것은 개별 헤더에서 include하지 않음**
2. **ServerEngine 자체 헤더 간 의존성은 명시적 include 유지**
3. **Lock 계열은 필요한 곳에서 직접 include**
4. **.cpp 파일은 기존대로 PCH 자동 적용**
