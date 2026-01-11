// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ShellMcp.ixx - Shell MCP 모듈 인터페이스
module;

#pragma warning(disable: 5028)

// GMF - ServerEngine PCH 필요 (ThreadId 등)
#include "Shell/PCH/pch.h"

#include "Shell/ShellMcpManager/ShellMcpManager.h"
#include "Shell/ShellMcpManager/ShellMcpThread.h"

export module ShellMcp;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
export import ServerEngineCore;
export import ServerEngineWorker;
export import ServerEngineUtil;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
export
{
	using ::ShellMcpManager;
	using ::gShellMcpManager;
	using ::ShellMcpThread;
}
