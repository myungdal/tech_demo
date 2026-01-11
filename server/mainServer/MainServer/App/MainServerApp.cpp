// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;


MainServerApp::MainServerApp(AppArg appArg) 
	: 
	AppBase(AppType::MAIN_SERVER, appArg)
{
	Initialize();
	// Shell과 Db서버와 Main서버는 OnRemoteAck 함수를 사용하기 때문에, 여분의 Iocp 를 생성한다.
	CreateIocpThread();

	const AppConfigData& myAppConfig = GetMyAppConfig();

	// 어플리케이션 관리자 생성
	gAppListManager = std::make_shared<AppListManager>();
	gAppListManager->AddAppInfo(
		APP_DATA::Writer(PARAM, TEMP_BUF,
			GetMyAppId(),
			GetMyAppType(),
			gAppConfigManager->mMyIp.c_str()
		).GetPacket()
	);
	
	// Main서버 역할 준비
	{
		gDbEnv = std::make_shared<DbEnv>();

		gMainDbConnPool = std::make_shared<MainDbConnPool>(gDbEnv->GetHandle(), myAppConfig.mMainDbConnPoolCount);
		gUserDbConnPool = std::make_shared<UserDbConnPool>(gDbEnv->GetHandle(), myAppConfig.mUserDbConnPoolCount);

		gAccountManager = std::make_shared<AccountManager>();
		if (!gAccountManager->Load())
		{
			_ASSERT_CRASH(false);
			return;
		}

		gServerListManager = std::make_shared<ServerListManager>();
		if (!gServerListManager->Load())
		{
			_ASSERT_CRASH(false);
			return;
		}

		gGameListManager = std::make_shared<GameListManager>();
		if (!gGameListManager->Load())
		{
			_ASSERT_CRASH(false);
			return;
		}

		gRoomListManager = std::make_shared<RoomListManager>();

		gMainPacketHandlerAuth = std::make_shared<MainPacketHandlerAuth>();
		gMainPacketHandlerChat = std::make_shared<MainPacketHandlerChat>();
		gMainPacketHandlerShellCommand = std::make_shared<MainPacketHandlerShellCommand>();
		gMainPacketHandlerRoom = std::make_shared<MainPacketHandlerRoom>();
		gMainPacketHandlerGame = std::make_shared<MainPacketHandlerGame>();
		gMainPacketHandlerSystem = std::make_shared<MainPacketHandlerSystem>();
	}

	// 서버간 연결.
	{
		mSocketMainFromBridgeListener = std::make_shared<SocketMainFromBridge>();
		mSocketMainFromBridgeListener->SetIocp(mIocp);
		if (!mSocketMainFromBridgeListener->Listen(
			L"0.0.0.0",
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_BM"),
			myAppConfig.mBacklog,
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[MainServer] SocketMainFromBridge Listen failed (PORT_BM)");
			_ASSERT_CRASH(false);
			return;
		}

		mSocketMainFromDbListener = std::make_shared<SocketMainFromDb>();
		mSocketMainFromDbListener->SetIocp(mIocp);
		if (!mSocketMainFromDbListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_DM"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[MainServer] SocketMainFromDb Listen failed (PORT_DM)");
			_ASSERT_CRASH(false);
			return;
		}

		mSocketMainFromFrontListener = std::make_shared<SocketMainFromFront>();
		mSocketMainFromFrontListener->SetIocp(mIocp);
		if (!mSocketMainFromFrontListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_FM"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[MainServer] SocketMainFromFront Listen failed (PORT_FM)");
			_ASSERT_CRASH(false);
			return;
		}

		mSocketMainFromGameListener = std::make_shared<SocketMainFromGame>();
		mSocketMainFromGameListener->SetIocp(mIocp);
		if (!mSocketMainFromGameListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_GM"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[MainServer] SocketMainFromGame Listen failed (PORT_GM)");
			_ASSERT_CRASH(false);
			return;
		}

		mSocketMainFromLogListener = std::make_shared<SocketMainFromLog>();
		mSocketMainFromLogListener->SetIocp(mIocp);
		if (!mSocketMainFromLogListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_LM"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[MainServer] SocketMainFromLog Listen failed (PORT_LM)");
			_ASSERT_CRASH(false);
			return;
		}

		mSocketMainFromShellListener = std::make_shared<SocketMainFromShell>();
		mSocketMainFromShellListener->SetIocp(mIocp);
		if (!mSocketMainFromShellListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_SM"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[MainServer] SocketMainFromShell Listen failed (PORT_SM)");
			_ASSERT_CRASH(false);
			return;
		}
	}
}

