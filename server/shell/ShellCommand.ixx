// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ShellCommand.ixx - Shell 명령어 모듈 인터페이스
module;

#pragma warning(disable: 5028)

// GMF - ServerEngine PCH 필요 (StringUtil 등)
#include "Shell/PCH/pch.h"

#include "Shell/ShellCommandManager/ShellCommandManager.h"

export module ShellCommand;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
export import ServerEngineCore;
export import ServerEngineUtil;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
export
{
	using ::ShellCommandManager;
	using ::gShellCommandManager;
}
