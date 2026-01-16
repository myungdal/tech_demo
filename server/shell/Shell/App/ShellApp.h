// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"
#include "ServerEngine/Thread/ThreadImpl/ConsoleThread.h"
#include "Shell/ShellMcpManager/ShellMcpThread.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Shell 서버 애플리케이션
// MCP(Model Context Protocol) 및 콘솔 명령을 통한 서버 상호작용을 담당한다.
class ShellApp final : public AppBase
{
private:
	std::shared_ptr<ConsoleThread> mConsoleThread = nullptr;	// 콘솔 명령 처리 스레드
	std::shared_ptr<ShellMcpThread> mMcpThread = nullptr;		// MCP 서버 스레드

public:
	explicit ShellApp(AppArg appArg);

	// 메인 루프 실행 (콘솔 및 MCP 스레드 시작)
	void Run() override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<ShellApp> gShellApp = nullptr;
