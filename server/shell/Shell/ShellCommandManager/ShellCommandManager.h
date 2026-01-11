// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Shell 명령어 관리자
// 콘솔에서 입력된 명령어를 처리하고, Main 서버로 전달하여 결과를 수집한다.
class ShellCommandManager
{
private:
	CommandDispatcherWithResult<std::wstring> mCommandDispatcher;	// 명령어 디스패처

public:
	ShellCommandManager();

	// 명령어 실행 및 결과 반환
	// command: 실행할 명령어 문자열
	// return: 명령어 실행 결과
	std::wstring Dispatch(const std::wstring& command);

private:
	// help 명령어 핸들러
	std::wstring OnCommand_help(ArgList& argList);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<ShellCommandManager> gShellCommandManager = nullptr;
