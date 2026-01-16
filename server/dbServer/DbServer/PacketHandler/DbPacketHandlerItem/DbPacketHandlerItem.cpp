// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbPacketHandlerItem.h"

#include "DbServer/PacketHandler/DbPacketHandlerItem/Transactor_CD_REQ_ITEM_USE.h"


// 게임 아이템 사용 요청
HandleResult DbPacketHandlerItem::OnPacket(CD_REQ_ITEM_USE& rp, MAYBE_UNUSED SocketDbFromFront& socket)
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
	Transactor_CD_REQ_ITEM_USE transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 게임에서 아이템이 변한 경우
HandleResult DbPacketHandlerItem::OnPacket(class GD_GAME_USER_ITEM_UPDATE& rp, MAYBE_UNUSED SocketDbFromGame& socket)
{
	rp;

	return HandleResult::OK;
}

