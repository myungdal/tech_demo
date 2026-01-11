// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// Shell.ixx - Shell 모듈 인터페이스
module;

#pragma warning(disable: 5028)

// GMF - ServerEngine PCH 필요
#include "Shell/PCH/pch.h"

// Shell 헤더
#include "Shell/App/ShellApp.h"

export module Shell;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ServerEngine 분할 모듈 (우산 모듈 사용 금지 - 4GB 제한)
export import ServerEngineCore;
export import ServerEngineWorker;
export import ServerEngineNetwork;
export import ServerEngineApp;
export import ServerEngineUtil;
export import ServerEnginePacket;

// Shell 분할 모듈
export import ShellSocket;
export import ShellCommand;
export import ShellMcp;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

export
{
	using ::ShellApp;
	using ::gShellApp;
}
