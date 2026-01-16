// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;


SocketDbToMain::SocketDbToMain()
{
	mShellCommandDispatcher.Register(
		L"help", L"도움말",
		std::bind(&SocketDbToMain::OnCommand_help, this, std::placeholders::_1)
	);
	mShellCommandDispatcher.Register(
		L"reload", L"db 다시 읽기",
		std::bind(&SocketDbToMain::OnCommand_reload, this, std::placeholders::_1)
	);
}
void SocketDbToMain::OnAppAdded(IN OUT AppInfo& appInfo)
{
	if (AppType::LOG_SERVER == appInfo.GetData().Get_appType())
	{
		gSocketDbToLog = std::make_shared<SocketDbToLog>();
		gSocketDbToLog->SetIocp(mIocp);
		gSocketDbToLog->Connect(
			appInfo.GetData().Get_ip(),
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_DL"),
			gDbServerApp->GetMyAppId(),
			gDbServerApp->GetMyAppType(),
			appInfo.GetData().Get_appId(),
			appInfo.GetData().Get_appType()
		);

		appInfo.SetSocket(gSocketDbToLog->GetSocketPtr(), L"OnAppAdded");
	}
}

std::wstring SocketDbToMain::OnShellCommand(MAYBE_UNUSED const wchar_t* command)
{
	return mShellCommandDispatcher.Dispatch(command);
}

std::wstring SocketDbToMain::OnCommand_help(MAYBE_UNUSED ArgList& argList)
{
	std::wstring result;
	result += L"\n";
	result += L"* DbServer 명령:\n";
	for (const auto& [key, handler] : mShellCommandDispatcher.GetHandlerMap())
	{
		result += L"\t";
		result += std::get<0>(handler);
		result += L"\t";
		result += std::get<1>(handler);
		result += L"\n";
	}
	result += L"\n";
	return result;
}

std::wstring SocketDbToMain::OnCommand_reload(MAYBE_UNUSED ArgList& argList)
{
	gStaticDataCatalog =
		std::make_shared<StaticDataCatalog>(
			gAppConfigManager->GetStaticDataIndexListForGameServer(),
			gAppConfigManager->GetStaticDataIndexListForClient()
		);
	if (false == StaticDbLoader::Load(gDbEnv->GetHandle(), *gStaticDataCatalog))
	{
		_ASSERT_CRASH(false);
		return L"FAILED\n";
	}
	gStaticDataCatalog->SetSyncing();
	gStaticDataCatalog->StaticDataToBinary();	

	gStaticDataAccessor = std::make_shared<StaticDataAccessor>();
	gStaticDataAccessor->BuildView();
	gStaticDataCatalog->SetSynced();

	SocketUtil::Send<SHELL_NOTIFY::Writer> wp(*gSocketDbToMain, NOTIFY);
	wp.SetValues(
		L"reloaded"
	);

	return L"OK\n";
}

bool SocketDbToMain::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketDbToMain::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::MD_ACK_GAME_CREATE:
	case PacketTypes::MD_ACK_GAME_USER_ENTER:
	case PacketTypes::MD_ACK_GAME_USER_LEAVE:
		OnRemoteAck(rp);
		break;
	case PacketTypes::MD_REQ_AUTH_TICKET:
	{
		const UserId userId = rp.GetHeader().GetUserId();
		if (INVALID_UUID == userId)
		{
			// 에러 반환 될 것.
			if (tThreadId == INVALID_THREAD_ID)
				PacketUtil::DispatchPacketOnAnyThread(this, &SocketDbToMain::DispatchPacket_async, rp);
			else
				DispatchPacketToHandler(rp);
		}
		else if (DbUserPtr dbUser = gDbUserManager->FindUser(userId))
			PacketUtil::DispatchPacketOnWorker(dbUser, this, &SocketDbToMain::DispatchPacket_async, rp);
		else // 아직 유저가 없다
			PacketUtil::DispatchPacketOnWorker(gDbUserManager, this, &SocketDbToMain::DispatchPacket_async, rp);
		break;
	}
	default:
		if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketDbToMain::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketDbToMain::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketDbToMain::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketMD(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gDbPacketHandlerAuth.get(), rp)) return;

	if (DispatchPacketMD<MainPeerSocket>(mSocket, this, rp) != HandleResult::NOT_EXISTS) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

