// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// AppConstants.h - App 공통 상수/전역/타입
// (모듈에서는 ServerEngineApp/Project에서 export됨)

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// App 타입
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// AppInfo forward declaration
class AppInfo;

// AppInfo 스마트 포인터
using AppInfoPtr = std::shared_ptr<AppInfo>;

// App 리스트 (고정 크기 배열)
using AppList = std::array<AppInfoPtr, MAX_APP_COUNT>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// App 전역
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 현재 앱 타입
inline AppType gMyAppType = AppType::NONE;

// 현재 앱 ID
inline AppId gMyAppId = INVALID_APP_ID;

// 앱 이름
inline wchar_t gAppName[32] = { 0, };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 콘솔 설정
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 콘솔 창 너비(픽셀)
inline constexpr int CONSOLE_WIDTH = 500;

// 콘솔 창 높이(픽셀)
inline constexpr int CONSOLE_HEIGHT = 300;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 소켓 풀 크기 상수
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 풀 크기 팩터 (풀 크기 = 2^factor)
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_BRIDGE_FROM_FRONT = 5;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_DB_FROM_FRONT = 5;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_GAME_FROM_FRONT = 5;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_DB_FROM_GAME = 5;

inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_LOG_FROM_DB = 5;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_DB_TO_LOG = 2;

inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_FRONT = 5;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_GAME = 5;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_BRIDGE = 5;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_DB = 5;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_LOG = 2;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_SHELL = 2;

inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_MAIN = 2;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_GAME_TO_MAIN = 2;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_BRIDGE_TO_MAIN = 2;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_DB_TO_MAIN = 2;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_LOG_TO_MAIN = 2;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_SHELL_TO_MAIN = 2;

inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_BRIDGE = 2;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_DB = 2;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_GAME = 2;

inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_GAME_TO_DB = 2;

inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_FROM_CLIENT = 10;
inline constexpr size_t MAX_POOL_SIZE_FACTOR_SOCKET_BOT_TO_FRONT = 10;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bot 타입
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 봇 테스트 목록(테스트 이름 리스트)
using BotTestList = std::vector<std::wstring>;

// 봇 테스트 테이블(시나리오 ID -> 테스트 목록)
using BotTestTable = std::map<BotScenarioId, BotTestList>;

