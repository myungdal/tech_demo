// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"
#include "ServerEngine/Socket/SocketBase/SocketBase.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 공통적인 명령 처리
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CommonShellCommandHandler
{
private:
	SocketBaseWeakPtr mSocketBaseWeakPtr;

	CommandDispatcherWithResult<std::wstring> mCommandDispatcher;

public:
	CommonShellCommandHandler();

public:
	std::wstring Dispatch(SocketBasePtr socket, const wchar_t* command);

public:
	std::wstring OnCommand_help(ArgList& argList);
	std::wstring OnCommand_appList(ArgList& argList);
	std::wstring OnCommand_mem(ArgList& argList);
	std::wstring OnCommand_commandList(ArgList& argList);
};

