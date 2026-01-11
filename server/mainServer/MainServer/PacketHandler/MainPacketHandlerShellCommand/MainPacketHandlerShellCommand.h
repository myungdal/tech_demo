// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"
#include "ServerEngine/AppListManager/AppListManager.h"
#include "ServerEngine/Socket/SocketImpl/CommonShellCommandHandler.h"


class SocketMainFromShell;
class SocketMainFromDb;
class SocketMainFromFront;
class SocketBase;
class REQ_SHELL_COMMAND;

class MainPacketHandlerShellCommand final
{
private:
	CommandDispatcherWithResult<std::wstring> mMyShellCommandDispatcher;
	CommonShellCommandHandler mCommonShellCommandHandler;

public:
	MainPacketHandlerShellCommand();

public:
	CommandDispatcherWithResult<std::wstring>& GetMyShellCommandDispatcher() { return mMyShellCommandDispatcher; }
	CommonShellCommandHandler& GetCommonShellCommandHandler() { return mCommonShellCommandHandler; }

private:
	std::wstring OnCommand_help(ArgList& argList);
	std::wstring OnCommand_setTime(ArgList& argList);
	std::wstring OnCommand_gameList(ArgList& argList);

public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromShell& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromDb& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromFront& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class REQ_SHELL_COMMAND& rp, MAYBE_UNUSED SocketMainFromShell& socket);
	HandleResult OnPacket(class SHELL_NOTIFY& rp, MAYBE_UNUSED SocketMainFromDb& socket);
	HandleResult OnPacket(class CM_SHELL_COMMAND& rp, MAYBE_UNUSED SocketMainFromFront& socket);
};

inline std::shared_ptr<MainPacketHandlerShellCommand> gMainPacketHandlerShellCommand = nullptr;
