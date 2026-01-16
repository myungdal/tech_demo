// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Thread/ThreadBase/Thread.h"
#include "Shell/ShellMcpManager/ShellMcpManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MCP 서버 전용 스레드
// ShellMcpManager::Run()을 별도 스레드에서 실행한다.
class ShellMcpThread : public Thread
{
public:
	ShellMcpThread();
	~ShellMcpThread() override;

protected:
	// 스레드 메인 루프 (stop_token으로 종료 지원)
	void Run(std::stop_token stopToken) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<ShellMcpThread> gShellMcpThread = nullptr;
