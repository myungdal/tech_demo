// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbPacketHandlerLogin.h"

#include "DbServer/PacketHandler/DbPacketHandlerLogin/Transactor_FD_REQ_USER_LOGIN.h"
#include "DbServer/PacketHandler/DbPacketHandlerLogin/Transactor_FD_USER_LOGOUT.h"


// 유저 로그인 요청 받음
HandleResult DbPacketHandlerLogin::OnPacket(FD_REQ_USER_LOGIN& rp, MAYBE_UNUSED SocketDbFromFront& socket)
{
	const UserId userId = rp.GetHeader().GetUserId();
	DbUserPtr dbUser = gDbUserManager->FindUser(userId);
	if (nullptr == dbUser)
	{
		_DEBUG_RED;
		return HandleResult::OK;
	}

	_ASSERT_CRASH(tWorkingWorker == dbUser);

	{
		DbUserContext userContext = dbUser->GetContext();
		DbUserSocketState& dbUserSocketState = userContext.GetUserSocketState();
		dbUserSocketState.SetCurrPacketHeader(rp.GetHeader());
	}

	{
		DbUserContext userContext = dbUser->GetContext();
		Transactor_FD_REQ_USER_LOGIN transactor(*dbUser, userContext, socket, rp);
		transactor.Run();
	}

	return HandleResult::OK;
}

// 유저 로그아웃
HandleResult DbPacketHandlerLogin::OnPacket(FD_USER_LOGOUT& rp, MAYBE_UNUSED SocketDbFromFront& socket)
{
	const UserId userId = rp.GetHeader().GetUserId();
	DbUserPtr dbUser = gDbUserManager->FindUser(userId);
	if (nullptr == dbUser)
	{
		_DEBUG_RED;
		return HandleResult::OK;
	}

	_ASSERT_CRASH(tWorkingWorker == dbUser);

	DbUserContext userContext = dbUser->GetContext();
	Transactor_FD_USER_LOGOUT transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

