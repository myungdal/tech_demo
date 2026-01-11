// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;


BotScenarioManager::BotScenarioManager(const AppConfigData& myAppConfig, std::shared_ptr<Iocp> iocp)
	:
	mAppConfigData(myAppConfig),
	mIocp(iocp)
{
	mMaxBotCount = std::min(_IDX(mAppConfigData.mBotCount), LIMIT_BOT_COUNT);
	gBotMonitor->Initialize(mMaxBotCount);
	WorkerManager::RunOnTimer(mAppConfigData.mBotCreationDelay, gTimerWorker->GetTimerWorker(), this, &BotScenarioManager::CreateBot_timer);
}
void BotScenarioManager::CreateBot_timer()
{
	if (mBotPtrList.size() >= mMaxBotCount)
		return;

	BotId botId = mBotPtrList.size();
	BotPtr bot = std::make_shared<Bot>(botId, mIocp, mAppConfigData);

	bot->Start();

	mBotPtrList.emplace_back(bot);

	WorkerManager::RunOnTimer(mAppConfigData.mBotCreationDelay, gTimerWorker->GetTimerWorker(), this, &BotScenarioManager::CreateBot_timer);
}
