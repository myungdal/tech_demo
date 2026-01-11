// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import GameServer;
import ServerEnginePacket;


GameServerApp::GameServerApp(AppArg appArg)
	: 
	AppBase(AppType::GAME_SERVER, appArg)
{
	Initialize();
	const AppConfigData& myAppConfig = GetMyAppConfig();

	// 어플리케이션 관리자 생성
	gAppListManager = std::make_shared<AppListManager>();

	// Game서버 역할 준비.
	{
		gStaticDataCatalog = 
			std::make_shared<StaticDataCatalog>(
				gAppConfigManager->GetStaticDataIndexListForGameServer(), 
				gAppConfigManager->GetStaticDataIndexListForClient()
			);

		gStaticDataAccessor = std::make_shared<StaticDataAccessor>();

		gGamePacketHandlerStaticData = std::make_shared<GamePacketHandlerStaticData>();
		gGamePacketHandlerGame = std::make_shared<GamePacketHandlerGame>();
		gGamePacketHandlerSystem = std::make_shared<GamePacketHandlerSystem>();

		// 게임 채널 매니저 생성
		gGameChannelManager = std::make_shared<GameChannelManager>(myAppConfig.mGameChannelCount);
	}

	// 서버간 연결.
	{
		gSocketGameToMain = std::make_shared<SocketGameToMain>();
		gSocketGameToMain->SetIocp(mIocp);
		// Connect 실패 시 PeerSocket의 재접속 로직이 동작함
		gSocketGameToMain->Connect(
			gAppConfigManager->GetVariableStr(L"MAIN_SERVER_IP"), 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_GM"),
			GetMyAppId(),
			GetMyAppType(),
			gAppConfigManager->GetFirstAppId(AppType::MAIN_SERVER),
			AppType::MAIN_SERVER
		);

		mSocketGameFromFrontListener = std::make_shared<SocketGameFromFront>();
		mSocketGameFromFrontListener->SetIocp(mIocp);
		if (!mSocketGameFromFrontListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_FG"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[GameServer] SocketGameFromFront Listen failed (PORT_FG)");
			_ASSERT_CRASH(false);
			return;
		}
	}
}

