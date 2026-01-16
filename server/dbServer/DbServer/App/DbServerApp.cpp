// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;


DbServerApp::DbServerApp(AppArg appArg)
	: 
	AppBase(AppType::DB_SERVER, appArg)
{
	Initialize();
	// Shell과 Db서버와 Main서버는 OnRemoteAck 함수를 사용하기 때문에, 여분의 Iocp 를 생성한다.
	CreateIocpThread();

	const AppConfigData& myAppConfig = GetMyAppConfig();

	// 어플리케이션 관리자 생성
	gAppListManager = std::make_shared<AppListManager>();	

	// DB서버 역할 준비.
	{
		gDbEnv = std::make_shared<DbEnv>();
		
		gMainDbConnPool = std::make_shared<MainDbConnPool>(gDbEnv->GetHandle(), myAppConfig.mMainDbConnPoolCount);
		gUserDbConnPool = std::make_shared<UserDbConnPool>(gDbEnv->GetHandle(), myAppConfig.mUserDbConnPoolCount);
		gDbUserManager = std::make_shared<DbUserManager>();

		gStaticDataCatalog =
			std::make_shared<StaticDataCatalog>(
				gAppConfigManager->GetStaticDataIndexListForGameServer(), 
				gAppConfigManager->GetStaticDataIndexListForClient()
			);
		if (false == StaticDbLoader::Load(gDbEnv->GetHandle(), *gStaticDataCatalog))
		{
			_ASSERT_CRASH(false);
			return;
		}
		gStaticDataCatalog->SetSyncing();
		gStaticDataCatalog->StaticDataToBinary();		
		
		gStaticDataAccessor = std::make_shared<StaticDataAccessor>();
		gStaticDataAccessor->BuildView();
		gStaticDataCatalog->SetSynced();

		gDbPacketHandlerAuth = std::make_shared<DbPacketHandlerAuth>();
		gDbPacketHandlerCheat = std::make_shared<DbPacketHandlerCheat>();
		gDbPacketHandlerGame = std::make_shared<DbPacketHandlerGame>();
		gDbPacketHandlerLogin = std::make_shared<DbPacketHandlerLogin>();
		gDbPacketHandlerMail = std::make_shared<DbPacketHandlerMail>();
		gDbPacketHandlerStaticData = std::make_shared<DbPacketHandlerStaticData>();
		gDbPacketHandlerUser = std::make_shared<DbPacketHandlerUser>();
		gDbPacketHandlerItem = std::make_shared<DbPacketHandlerItem>();

		gCheatManager = std::make_shared<CheatManager>();
	}

	// 서버간 연결.
	{
		gSocketDbToMain = std::make_shared<SocketDbToMain>();
		gSocketDbToMain->SetIocp(mIocp);
		// Connect 실패 시 PeerSocket의 재접속 로직이 동작함
		gSocketDbToMain->Connect(
			gAppConfigManager->GetVariableStr(L"MAIN_SERVER_IP"), 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_DM"),
			GetMyAppId(),
			GetMyAppType(),
			gAppConfigManager->GetFirstAppId(AppType::MAIN_SERVER),
			AppType::MAIN_SERVER
		);

		mSocketDbFromFrontListener = std::make_shared<SocketDbFromFront>();
		mSocketDbFromFrontListener->SetIocp(mIocp);
		if (!mSocketDbFromFrontListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_FD"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[DbServer] SocketDbFromFront Listen failed (PORT_FD)");
			_ASSERT_CRASH(false);
			return;
		}

		mSocketDbFromGameListener = std::make_shared<SocketDbFromGame>();
		mSocketDbFromGameListener->SetIocp(mIocp);
		if (!mSocketDbFromGameListener->Listen(
			L"0.0.0.0", 
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_GD"), 
			myAppConfig.mBacklog, 
			myAppConfig.mMaxConnectionCount))
		{
			_DEBUG_LOG(RED, L"[DbServer] SocketDbFromGame Listen failed (PORT_GD)");
			_ASSERT_CRASH(false);
			return;
		}
	}
}

