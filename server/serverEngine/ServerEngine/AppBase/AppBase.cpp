// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "AppBase.h"

import ServerEngineApp;
import ServerEngineLog;
import ServerEngineWorker;
import ServerEngineNetwork;
import ServerEngineCore;


AppBase::AppBase(AppType appType, AppArg appArg)
	: 
	mAppType(appType),
	mAppArg(appArg)
{
	wcscpy_s(gAppName, GetAppTypeString(mAppType));
	gNetworkManager = std::make_shared<NetworkManager>();
	tClock.Update();
	gCoWorker = std::make_shared<CoWorker>();
	gCronWorker = std::make_shared<CronWorker>();
	gTimerWorker = std::make_shared<TimerWorker>();
}
AppBase::~AppBase()
{
}
void AppBase::Initialize()
{
	gAppConfigManager = std::make_shared<AppConfigManager>();
	const AppConfigData& appConfigData = gAppConfigManager->GetMyAppConfig(mAppType, mAppArg);

	gMyAppType= GetMyAppType();
	gMyAppId = GetMyAppId();

	InitException();

	auto [logPath, _] = StringUtil::FormatStr(L"{}/", GetAppTypeString(gMyAppType));

	mLogWriterConsole = std::make_shared<LogWriterConsole>(mAppType, mAppArg, appConfigData.mAppId);
	mLogWriterFile = std::make_shared<LogWriterFile>(*logPath, gAppName);

	mIocp = std::make_shared<Iocp>();

	mWorkerThreadManager = std::make_shared<WorkerThreadManager>(appConfigData.mWorkerThreadCount, mIocp);
}

void AppBase::CreateIocpThread()
{
	mWorkerThreadManager->CreateIocpThread();
}

void AppBase::ResumeWorker() const
{
	gTimerWorker->Resume();
	mWorkerThreadManager->Start();
}

void AppBase::SuspendWorker() const
{
	if (mWorkerThreadManager)
		mWorkerThreadManager->Stop();
}

void AppBase::Run()
{
	while (true)
	{
		TlsCommonBuffer::ResetTlsCommonBufferIdx();

		tClock.Update();

		UpdateLog();

		_YIELD_APP;
	}

	TlsCommonBuffer::ReleaseTlsCommonBuffer();
}

void AppBase::UpdateLog()
{
	if (nullptr == gLogQueue)
		return;

	size_t logCount = 0;
	Log* log = gLogQueue->Pop(logCount);

	mLogWriterConsole->WriteToConsole(log);
	
	mLogWriterFile->WriteToFile(log);

	while (nullptr != log)
	{
		Log* temp = log->GetNext();
		log->Dispose();
		log = temp;
	}
}

const AppConfigData& AppBase::GetMyAppConfig() const
{
	return gAppConfigManager->GetMyAppConfig(mAppType, mAppArg);
}

AppId AppBase::GetMyAppId() const
{
	const AppConfigData& myAppConfig = gAppConfigManager->GetMyAppConfig(mAppType, mAppArg);
	return myAppConfig.mAppId;
}
