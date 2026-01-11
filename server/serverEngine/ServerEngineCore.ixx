// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ServerEngineCore.ixx - Core Module (경량)
// Core, TLS, Clock, Lock, Exception 만 포함

module;

// GMF - Core
#include "ServerEngine/Core/NetworkTypes.h"
#include "ServerEngine/Core/AppConstants.h"

// GMF - TLS
#include "ServerEngine/TLS/TlsGlobals.h"

// GMF - Clock
#include "ServerEngine/Clock/TlsClock.h"

// GMF - Lock
#include "ServerEngine/Lock/Lock.h"
#include "ServerEngine/Lock/ReadLock.h"
#include "ServerEngine/Lock/WriteLock.h"

// GMF - Exception
#include "ServerEngine/Exception/Exception.h"

export module ServerEngineCore;

// ============= Core =============
export using ::IoType;
export using ::Overlapped;
export using ::CompletionHandler;
export using ::ConnectEx;
export using ::DisconnectEx;
export using ::AppInfoPtr;
export using ::AppList;
export using ::gMyAppType;
export using ::gMyAppId;
export using ::gAppName;
export using ::CONSOLE_WIDTH;
export using ::CONSOLE_HEIGHT;

// 소켓 풀 상수
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_BRIDGE_FROM_FRONT;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_DB_FROM_FRONT;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_GAME_FROM_FRONT;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_DB_FROM_GAME;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_LOG_FROM_DB;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_DB_TO_LOG;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_FRONT;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_GAME;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_BRIDGE;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_DB;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_LOG;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_SHELL;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_MAIN;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_GAME_TO_MAIN;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_BRIDGE_TO_MAIN;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_DB_TO_MAIN;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_LOG_TO_MAIN;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_SHELL_TO_MAIN;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_BRIDGE;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_DB;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_GAME;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_GAME_TO_DB;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_FROM_CLIENT;
export using ::MAX_POOL_SIZE_FACTOR_SOCKET_BOT_TO_FRONT;
export using ::BotTestList;
export using ::BotTestTable;

// ============= TLS =============
export using ::ThreadId;
export using ::ThreadCount;
export using ::ThreadWorkerPtr;
export using ::ThreadWorkerPtrList;
export using ::CronHandle;
export using ::tThreadId;
export using ::tWorkingWorker;
export using ::tLockedCount;
export using ::tClock;
export using ::tClockInternalMembers;
export using ::tRandomEngine32;
export using ::tRandomEngine64;
export using ::tDbSpInBuf;
export using ::tDbSpOutBuf;
export using ::gThreadWorkerPtrList;
export using ::gWorkerThreadCount;
export using ::MAX_THREAD_COUNT;
export using ::INVALID_THREAD_ID;
export using ::MAX_DB_SP_IN_BUF_SIZE;
export using ::MAX_DB_SP_OUT_BUF_SIZE;

// ============= Clock =============
export using ::TlsClock;
export using ::TlsClockInternalMembers;

// ============= Lock =============
export using ::Lock;
export using ::ReadLock;
export using ::WriteLock;

// ============= Exception =============
export using ::ExceptionFilter;
export using ::PureCallHandler;
export using ::InvalidParameterHandler;
export using ::InitException;

