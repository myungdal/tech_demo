// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import BridgeServer;


BridgeServerApp::BridgeServerApp(AppArg appArg)
	:
	AppBase(AppType::BRIDGE_SERVER, appArg)
{
	Initialize();
	const AppConfigData& myAppConfig = GetMyAppConfig();

	// 어플리케이션 관리자 생성
	gAppListManager = std::make_shared<AppListManager>();

	// Bridge서버 역할 준비
	{
		gBridgePacketHandlerAuth = std::make_shared<BridgePacketHandlerAuth>();
	}

	// 서버간 연결
	{
		// [WHY] MainServer와 연결하여 앱 정보 동기화 및 패킷 교환
		gSocketBridgeToMain = std::make_shared<SocketBridgeToMain>();
		gSocketBridgeToMain->SetIocp(mIocp);
		// [WHY] Connect 실패 시 PeerSocket의 재접속 로직이 자동으로 동작함
		gSocketBridgeToMain->Connect(
			gAppConfigManager->GetVariableStr(L"MAIN_SERVER_IP"),
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_BM"),
			GetMyAppId(),
			GetMyAppType(),
			gAppConfigManager->GetFirstAppId(AppType::MAIN_SERVER),
			AppType::MAIN_SERVER
		);

		// [WHY] FrontServer로부터의 요청을 수신하기 위한 리스너 소켓
		mSocketBridgeFromFrontListener = std::make_shared<SocketBridgeFromFront>();
		mSocketBridgeFromFrontListener->SetIocp(mIocp);
		if (!mSocketBridgeFromFrontListener->Listen(
			L"0.0.0.0",
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_FB"),
			myAppConfig.mBacklog,
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[BridgeServer] SocketBridgeFromFront Listen failed (PORT_FB)");
			_ASSERT_CRASH(false);
			return;
		}
	}
}
