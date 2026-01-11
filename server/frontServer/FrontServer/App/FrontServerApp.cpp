// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;


FrontServerApp::FrontServerApp(AppArg appArg)
	: 
	AppBase(AppType::FRONT_SERVER, appArg)
{	
	Initialize();
	const AppConfigData& myAppConfig = GetMyAppConfig();

	// 어플리케이션 관리자 생성
	gAppListManager = std::make_shared<AppListManager>();

	// Front 서버 역할 준비.
	{
		gFrontDataManager = std::make_shared<FrontDataManager>();
		gChattingDelayCalculator = std::make_shared<ChattingDelayCalculator>();

		gFrontPacketHandlerChat = std::make_shared<FrontPacketHandlerChat>();
		gFrontPacketHandlerGame = std::make_shared<FrontPacketHandlerGame>();
		gFrontPacketHandlerLogin = std::make_shared<FrontPacketHandlerLogin>();
		gFrontPacketHandlerSystem = std::make_shared<FrontPacketHandlerSystem>();
	}

	// 서버간 연결.
	{
		gSocketFrontToMain = std::make_shared<SocketFrontToMain>();
		gSocketFrontToMain->SetIocp(mIocp);
		// Connect 실패 시 PeerSocket의 재접속 로직이 동작함
		gSocketFrontToMain->Connect(
			gAppConfigManager->GetVariableStr(L"MAIN_SERVER_IP"), 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_FM"),
			GetMyAppId(),
			GetMyAppType(),
			gAppConfigManager->GetFirstAppId(AppType::MAIN_SERVER),
			AppType::MAIN_SERVER
		);

		mSocketFrontFromClientListener = std::make_shared<SocketFrontFromClient>();
		mSocketFrontFromClientListener->SetIocp(mIocp);
		if (!mSocketFrontFromClientListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_CF"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[FrontServer] SocketFrontFromClient Listen failed (PORT_CF)");
			_ASSERT_CRASH(false);
			return;
		}
	}
}

