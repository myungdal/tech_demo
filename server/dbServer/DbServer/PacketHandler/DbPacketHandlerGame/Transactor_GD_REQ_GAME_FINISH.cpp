// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "Transactor_GD_REQ_GAME_FINISH.h"


Transactor_GD_REQ_GAME_FINISH::Transactor_GD_REQ_GAME_FINISH(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromGame& gameSocket,
	GD_REQ_GAME_FINISH& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mGameSocket(gameSocket),
	mRp(rp)
{
}
Result Transactor_GD_REQ_GAME_FINISH::OnUpdate()
{
	const AppId gameAppId = GetCurrPacketHeader().GetAppId(AppType::GAME_SERVER);
	if (gameAppId == INVALID_APP_ID)
		return Result::USER_STATUS_ERROR;

	auto mainSocket = DbSocketUtil::GetMainSocketPtr();
	if (!mainSocket)
	{
		_DEBUG_RED;
		return Result::RETRY_LATER;
	}

	SocketUtil::Request<DM_REQ_GAME_FINISH::Writer> wp(**mainSocket, REQ, mRp);

	if (!wp.Wait(GetCurrPacketHeader(), OUT mAck))
		return Result::RETRY_LATER;

	if (Result::SUCCEEDED != mAck->GetHeader().GetPacketResult())
		return mAck->GetHeader().GetPacketResult();

	return Result::SUCCEEDED;
}

void Transactor_GD_REQ_GAME_FINISH::OnFinish()
{	
	SocketUtil::Send<DG_ACK_GAME_FINISH::Writer> wp(mGameSocket, ACK, mRp, GetResult());
	wp.GetHeader().SetDbSeq(AddDbSeq());
}

void Transactor_GD_REQ_GAME_FINISH::OnError()
{
	SocketUtil::Send<DG_ACK_GAME_FINISH::Writer> wp(mGameSocket, ACK, mRp, GetResult());
}

void Transactor_GD_REQ_GAME_FINISH::OnLog()
{

}
