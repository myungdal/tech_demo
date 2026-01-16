// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbPacketHandlerGame.h"

#include "DbServer/PacketHandler/DbPacketHandlerGame/Transactor_CD_REQ_GAME_CREATE.h"
#include "DbServer/PacketHandler/DbPacketHandlerGame/Transactor_CD_REQ_GAME_USER_ENTER.h"
#include "DbServer/PacketHandler/DbPacketHandlerGame/Transactor_CD_REQ_GAME_USER_LEAVE.h"
#include "DbServer/PacketHandler/DbPacketHandlerGame/Transactor_GD_REQ_GAME_FINISH.h"


// 게임 생성 요청 받음
HandleResult DbPacketHandlerGame::OnPacket(CD_REQ_GAME_CREATE& rp, MAYBE_UNUSED SocketDbFromFront& socket)
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
	Transactor_CD_REQ_GAME_CREATE transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 게임 유저 입장 요청 받음
HandleResult DbPacketHandlerGame::OnPacket(CD_REQ_GAME_USER_ENTER& rp, MAYBE_UNUSED SocketDbFromFront& socket)
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
	Transactor_CD_REQ_GAME_USER_ENTER transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 게임 유저 퇴장 요청 받음
HandleResult DbPacketHandlerGame::OnPacket(CD_REQ_GAME_USER_LEAVE& rp, MAYBE_UNUSED SocketDbFromFront& socket)
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
	Transactor_CD_REQ_GAME_USER_LEAVE transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 게임 종료 요청 받음
HandleResult DbPacketHandlerGame::OnPacket(GD_REQ_GAME_FINISH& rp, MAYBE_UNUSED SocketDbFromGame& socket)
{
	// 유저 캐시 검증.
	const UserId userId = rp.GetHeader().GetUserId();
	DbUserPtr dbUser = gDbUserManager->FindUser(userId);
	if (nullptr == dbUser)
	{
		// 발생할 수 없는 상황.
		_ASSERT_CRASH(false);
		return HandleResult::OK;
	}

	_ASSERT_CRASH(tWorkingWorker == dbUser);

	DbUserContext userContext = dbUser->GetContext();
	Transactor_GD_REQ_GAME_FINISH transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

