// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"


class BotScenarioManager;

// 봇 애플리케이션 메인 클래스
class BotApp final : public AppBase
{
private:
	std::shared_ptr<BotScenarioManager> mBotScenarioManager = nullptr;

public:
	explicit BotApp(AppArg appArg);
};

inline std::shared_ptr<BotApp> gBotApp = nullptr;
