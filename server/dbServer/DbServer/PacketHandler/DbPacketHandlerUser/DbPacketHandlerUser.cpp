// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbPacketHandlerUser.h"

#include "DbServer/PacketHandler/DbPacketHandlerUser/Transactor_CD_REQ_USER_DATA.h"
#include "DbServer/PacketHandler/DbPacketHandlerUser/Transactor_CD_REQ_USER_LEVEL_UP.h"


// 유저 데이터 요청 받음.
HandleResult DbPacketHandlerUser::OnPacket(CD_REQ_USER_DATA& rp, MAYBE_UNUSED SocketDbFromFront& socket)
{
	const UserId userId = rp.GetHeader().GetUserId();
	DbUserPtr dbUser = gDbUserManager->FindUser(userId);
	if (nullptr == dbUser)
	{
		DbSocketUtil::SendToClient<DC_ACK_USER_DATA::Writer> wp(socket, ACK, rp, Result::RETRY_LATER);
		wp.SetValues(
			{},
			1
		);
		return HandleResult::OK;
	}

	_ASSERT_CRASH(tWorkingWorker == dbUser);

	DbUserContext userContext = dbUser->GetContext();
	Transactor_CD_REQ_USER_DATA transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 유저 레벨업 요청 받음
HandleResult DbPacketHandlerUser::OnPacket(CD_REQ_USER_LEVEL_UP& rp, MAYBE_UNUSED SocketDbFromFront& socket)
{
	const UserId userId = rp.GetHeader().GetUserId();
	DbUserPtr dbUser = gDbUserManager->FindUser(userId);
	if (nullptr == dbUser)
	{
		DbSocketUtil::SendToClient<DC_ACK_USER_LEVEL_UP::Writer> wp(socket, ACK, rp, Result::RETRY_LATER);
		wp.SetValues(
			INVALID_USER_LEVEL
		);
		return HandleResult::OK;
	}

	_ASSERT_CRASH(tWorkingWorker == dbUser);

	DbUserContext userContext = dbUser->GetContext();
	Transactor_CD_REQ_USER_LEVEL_UP transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

