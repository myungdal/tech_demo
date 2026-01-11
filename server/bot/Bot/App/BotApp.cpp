// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;
import ServerEnginePacket;


BotApp::BotApp(AppArg appArg)
	:
	AppBase(AppType::BOT, appArg),
	mBotScenarioManager()
{
	Initialize();
	[[maybe_unused]] const AppConfigData& myAppConfig = GetMyAppConfig();
	// BotApp 역할 준비
	{
		gStaticDataCatalog =
			std::make_shared<StaticDataCatalog>(
				gAppConfigManager->GetStaticDataIndexListForGameServer(),
				gAppConfigManager->GetStaticDataIndexListForClient()
			);

		gStaticDataAccessor = std::make_shared<StaticDataAccessor>();

		gBotAccountManager = std::make_shared<BotAccountManager>();

		gBotMonitor = std::make_shared<BotMonitor>();
	}

	// 봇 시나리오 시작
	{
		mBotScenarioManager = std::make_shared<BotScenarioManager>(myAppConfig, mIocp);
	}
}
