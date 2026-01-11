// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "Transactor_CD_REQ_GAME_USER_LEAVE.h"


Transactor_CD_REQ_GAME_USER_LEAVE::Transactor_CD_REQ_GAME_USER_LEAVE(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromFront& frontSocket,
	CD_REQ_GAME_USER_LEAVE& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CD_REQ_GAME_USER_LEAVE::OnUpdate()
{
	const AppId gameAppId = GetCurrPacketHeader().GetAppId(AppType::GAME_SERVER);
	if (gameAppId == INVALID_APP_ID)
		return Result::USER_STATUS_ERROR;

	auto mainSocket = DbSocketUtil::GetMainSocketPtr();
	if (!mainSocket)
	{
		_DEBUG_RED;
		return Result::FATAL_ERROR;
	}

	SocketUtil::Request<DM_REQ_GAME_USER_LEAVE::Writer> wp(**mainSocket, REQ, mRp);

	if (!wp.Wait(GetCurrPacketHeader(), OUT mAck))
		return Result::FATAL_ERROR;

	if (Result::SUCCEEDED != mAck->GetHeader().GetPacketResult())
		return mAck->GetHeader().GetPacketResult();

	return Result::SUCCEEDED;
}

void Transactor_CD_REQ_GAME_USER_LEAVE::OnFinish()
{	
	DbSocketUtil::SendToClient<DC_ACK_GAME_USER_LEAVE::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.GetHeader().SetDbSeq(AddDbSeq());
}

void Transactor_CD_REQ_GAME_USER_LEAVE::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_GAME_USER_LEAVE::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
}

void Transactor_CD_REQ_GAME_USER_LEAVE::OnLog()
{

}
