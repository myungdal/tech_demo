// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


struct AppConfigData;
class Iocp;

// 봇 생성 및 시나리오 관리
// - 설정에 따라 봇을 순차적으로 생성
// - 각 봇에게 시나리오 시작 지시
class BotScenarioManager
{
private:
	BotPtrList mBotPtrList;

	const AppConfigData& mAppConfigData;
	std::shared_ptr<Iocp> mIocp = nullptr;
	size_t mMaxBotCount = 0;

public:
	explicit BotScenarioManager(const AppConfigData& myAppConfig, std::shared_ptr<Iocp> iocp);

public:
	void CreateBot_timer();
};
