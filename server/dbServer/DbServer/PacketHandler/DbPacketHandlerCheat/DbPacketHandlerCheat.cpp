// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbPacketHandlerCheat.h"

#include "DbServer/PacketHandler/DbPacketHandlerCheat/Transactor_CD_REQ_CHEAT.h"


// 치트 요청 받음
HandleResult DbPacketHandlerCheat::OnPacket(CD_REQ_CHEAT& rp, MAYBE_UNUSED SocketDbFromFront& socket)
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
	Transactor_CD_REQ_CHEAT transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

