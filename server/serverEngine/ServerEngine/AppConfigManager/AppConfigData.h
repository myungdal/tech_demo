// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// AppConfigData.h - 앱 설정 데이터 구조체
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 앱 설정 데이터
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 앱 설정 데이터 (config.json에서 로드)
struct AppConfigData
{
	bool mEnable = false;                           // 앱 활성화 여부
	AppId mAppId = INVALID_APP_ID;                  // 앱 ID
	AppType mAppType = AppType::NONE;               // 앱 타입
	ThreadCount mWorkerThreadCount = 0;             // Worker 스레드 개수
	int32_t mMainDbConnPoolCount = 0;               // Main DB 커넥션 풀 크기
	int32_t mUserDbConnPoolCount = 0;               // User DB 커넥션 풀 크기
	GameChannelCount mGameChannelCount = 0;         // 게임 채널 개수
	int32_t mBacklog = 0;                           // Listen 백로그 크기
	int32_t mMaxConnectionCount = 0;                // 최대 연결 개수
	std::wstring mBotDestIp = L"";                  // 봇 목적지 IP
	BotScenarioId mBotScenarioId = 0;               // 봇 시나리오 ID
	int32_t mBotCount = 0;                          // 봇 개수
	ClockMs mBotCreationDelay = 1000ms;             // 봇 생성 지연 시간
	ClockMs mBotTaskDelay = 1000ms;                 // 봇 작업 지연 시간
	int32_t mBotRepeatCount = -1;                   // 봇 반복 횟수 (-1: 무한)
};

// 유효하지 않은 앱 설정 데이터
constexpr AppConfigData INVALID_APP_CONFIG_DATA{};

// 앱 설정 데이터 리스트
using AppConfigDataList = std::vector<AppConfigData>;

// 앱 타입별 설정 배열
using AppConfigArray = std::array<AppConfigDataList, _IDX(AppType::MAX)>;
