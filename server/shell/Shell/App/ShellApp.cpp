// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Shell;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ShellApp::ShellApp(AppArg appArg)
	:
	AppBase(AppType::SHELL, appArg)
{
	Initialize();
	gAppListManager = std::make_shared<AppListManager>();
	// Shell 서버 역할 준비
	{
		// [WHY] Shell 서버는 디버깅용이므로 패킷 로그를 찍지 않는다.
		SocketBase::PACKET_SEND_LOG = false;
		SocketBase::PACKET_RECV_LOG = false;

		gShellMcpManager = std::make_shared<ShellMcpManager>();
		mMcpThread = std::make_shared<ShellMcpThread>();

		gShellCommandManager = std::make_shared<ShellCommandManager>();

		mConsoleThread = std::make_shared<ConsoleThread>(
			[](std::promise<std::wstring>& promise, const wchar_t* cmd)
			{
				promise.set_value(gShellCommandManager->Dispatch(cmd));
			});

		gShellPacketHandler = std::make_shared<ShellPacketHandler>();
	}

	// 서버간 연결
	{
		gSocketShellToMain = std::make_shared<SocketShellToMain>();

		gSocketShellToMain->SetIocp(mIocp);
		// [WHY] Connect 실패 시 PeerSocket의 재접속 로직이 동작한다.
		gSocketShellToMain->Connect(
			gAppConfigManager->GetVariableStr(L"MAIN_SERVER_IP"),
			gAppConfigManager->GetVariableNum<uint16_t>(L"PORT_SM"),
			GetMyAppId(),
			GetMyAppType(),
			gAppConfigManager->GetFirstAppId(AppType::MAIN_SERVER),
			AppType::MAIN_SERVER
		);
	}
}

void ShellApp::Run()
{
	if (mMcpThread)
		mMcpThread->Start();

	if (mConsoleThread)
	{
		mConsoleThread->Start();
		AppBase::Run();
	}
}
