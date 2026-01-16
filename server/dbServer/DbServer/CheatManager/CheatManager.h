// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"


class SocketDbFromFront;
class DbUserSocketState;
class UserCacheAccessor;
class MainDbSession;
class UserDbSession;

class CheatManager final
{
private:
	CommandDispatcherWithResult<
		Result,
		SocketDbFromFront&,
		DbUserSocketState&,
		UserCacheAccessor&,
		MainDbSession&,
		UserDbSession&
	> mCheatDispatcher;

public:
	CheatManager();

public:
	Result DispatchCommand(
		const wchar_t* command,
		SocketDbFromFront& frontSocket, 
		DbUserSocketState& userSocketState,
		UserCacheAccessor& userCache,
		MainDbSession& mainDbSession,
		UserDbSession& userDbSession
	);

private:
	Result CreateItem(
		ArgList& argList,
		SocketDbFromFront& frontSocket,
		DbUserSocketState& userSocketState,
		UserCacheAccessor& userCache,
		MainDbSession& mainDbSession,
		UserDbSession& userDbSession
	);

private:
	Result CreateMail(
		ArgList& argList,
		SocketDbFromFront& frontSocket,
		DbUserSocketState& userSocketState,
		UserCacheAccessor& userCache,
		MainDbSession& mainDbSession,
		UserDbSession& userDbSession
	);
};

inline std::shared_ptr<CheatManager> gCheatManager = nullptr;
