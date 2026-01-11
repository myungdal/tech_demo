// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbPacketHandlerStaticData.h"

#include "DbServer/PacketHandler/DbPacketHandlerStaticData/Transactor_CD_REQ_STATIC_DATA.h"
#include "DbServer/PacketHandler/DbPacketHandlerStaticData/Transactor_CD_REQ_STATIC_DATA_CHECKSUM.h"
#include "DbServer/PacketHandler/DbPacketHandlerStaticData/Transactor_gD_REQ_STATIC_DATA.h"
#include "DbServer/PacketHandler/DbPacketHandlerStaticData/Transactor_GD_REQ_STATIC_DATA_CHECKSUM.h"


// 정적데이터 체크섬 요청 받음.
HandleResult DbPacketHandlerStaticData::OnPacket(CD_REQ_STATIC_DATA_CHECKSUM& rp, MAYBE_UNUSED SocketDbFromFront& socket)
{
	const UserId userId = rp.GetHeader().GetUserId();
	DbUserPtr dbUser = gDbUserManager->FindUser(userId);
	if (nullptr == dbUser)
	{
		DbSocketUtil::SendToClient<DC_ACK_STATIC_DATA_CHECKSUM::Writer> wp(socket, ACK, rp, Result::RETRY_LATER);
		wp.SetValues(
			{},
			nullptr, 0,
			nullptr, 0
		);
		return HandleResult::OK;
	}

	_ASSERT_CRASH(tWorkingWorker == dbUser);

	DbUserContext userContext = dbUser->GetContext();
	Transactor_CD_REQ_STATIC_DATA_CHECKSUM transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 정적데이터 요청 받음.
HandleResult DbPacketHandlerStaticData::OnPacket(CD_REQ_STATIC_DATA& rp, MAYBE_UNUSED SocketDbFromFront& socket)
{
	const UserId userId = rp.GetHeader().GetUserId();
	DbUserPtr dbUser = gDbUserManager->FindUser(userId);
	if (nullptr == dbUser)
	{
		DbSocketUtil::SendToClient<DC_ACK_STATIC_DATA::Writer> wp(socket, ACK, rp, Result::RETRY_LATER);
		wp.SetValues(
			rp.Get_index(),
			1,
			0,
			nullptr, 0
		);
		return HandleResult::OK;
	}

	_ASSERT_CRASH(tWorkingWorker == dbUser);

	DbUserContext userContext = dbUser->GetContext();
	Transactor_CD_REQ_STATIC_DATA transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 정적데이터 체크섬 요청 받음 (게임서버용)
HandleResult DbPacketHandlerStaticData::OnPacket(GD_REQ_STATIC_DATA_CHECKSUM& rp, MAYBE_UNUSED SocketDbFromGame& socket)
{
	Transactor_GD_REQ_STATIC_DATA_CHECKSUM transactor(socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

// 정적데이터 요청 받음 (게임서버용)
HandleResult DbPacketHandlerStaticData::OnPacket(GD_REQ_STATIC_DATA& rp, MAYBE_UNUSED SocketDbFromGame& socket)
{
	Transactor_GD_REQ_STATIC_DATA transactor(socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

