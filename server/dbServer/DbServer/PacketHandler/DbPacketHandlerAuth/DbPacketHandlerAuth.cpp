// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbServer/PacketHandler/DbPacketHandlerAuth/Transactor_MD_REQ_AUTH_TICKET.h"


HandleResult DbPacketHandlerAuth::OnPacket(MD_REQ_AUTH_TICKET& rp, MAYBE_UNUSED SocketDbToMain& socket)
{
	DbUserPtr dbUser = nullptr;
	const UserId userId = rp.GetHeader().GetUserId();
	if (INVALID_UUID != userId)
	{ 		
		dbUser = gDbUserManager->FindUser(userId);
		if (nullptr == dbUser)
		{
			_ASSERT_CRASH(tWorkingWorker == gDbUserManager);

			// User 생성 시도
			UserDbSession dbSession(CommitType::AUTO, rp.GetHeader().GetDbShardIdx());
			dbUser = gDbUserManager->CreateUser(dbSession, rp.Get_accountUser());
		}
		else
		{
			_ASSERT_CRASH(tWorkingWorker == dbUser);
		}
	}
	else
	{
		_DEBUG_RED;
	}
	
	if (nullptr != dbUser)
	{
		DbUserContext userContext = dbUser->GetContext();
		Transactor_MD_REQ_AUTH_TICKET transactor(*dbUser, userContext, socket, rp);
		transactor.Run();
	}
	else
	{
		// 실패 응답.
		SocketUtil::Send<DM_ACK_AUTH_TICKET::Writer> wp(socket, ACK, rp, Result::CACHE_ERROR);
	}

	return HandleResult::OK;
}

