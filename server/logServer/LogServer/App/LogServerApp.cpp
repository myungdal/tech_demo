// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import LogServer;


LogServerApp::LogServerApp(AppArg appArg)
	: 
	AppBase(AppType::LOG_SERVER, appArg)
{
	Initialize();
	const AppConfigData& myAppConfig = GetMyAppConfig();

	// 어플리케이션 관리자 생성
	gAppListManager = std::make_shared<AppListManager>();

	// Log서버 역할 준비.
	{
		gDbEnv = std::make_shared<DbEnv>();

		gLogDbConnPool = std::make_shared<LogDbConnPool>(gDbEnv->GetHandle(), gWorkerThreadCount);

		gLogPacketHandler = std::make_shared<LogPacketHandler>();
	}

	// 서버간 연결.
	{
		gSocketLogToMain = std::make_shared<SocketLogToMain>();
		gSocketLogToMain->SetIocp(mIocp);
		// Connect 실패 시 PeerSocket의 재접속 로직이 동작함
		gSocketLogToMain->Connect(
			gAppConfigManager->GetVariableStr(L"MAIN_SERVER_IP"),
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_LM"),
			GetMyAppId(),
			GetMyAppType(),
			gAppConfigManager->GetFirstAppId(AppType::MAIN_SERVER),
			AppType::MAIN_SERVER
		);

		mSocketLogFromDbListener = std::make_shared<SocketLogFromDb>();
		mSocketLogFromDbListener->SetIocp(mIocp);
		if (!mSocketLogFromDbListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_DL"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[LogServer] SocketLogFromDb Listen failed (PORT_DL)");
			_ASSERT_CRASH(false);
			return;
		}
	}
}

