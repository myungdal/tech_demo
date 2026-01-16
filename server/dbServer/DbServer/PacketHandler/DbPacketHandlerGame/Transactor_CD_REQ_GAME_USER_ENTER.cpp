// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "Transactor_CD_REQ_GAME_USER_ENTER.h"

Transactor_CD_REQ_GAME_USER_ENTER::Transactor_CD_REQ_GAME_USER_ENTER(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromFront& frontSocket,
	CD_REQ_GAME_USER_ENTER& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CD_REQ_GAME_USER_ENTER::OnUpdate()
{
	AppId gameAppId = GetCurrPacketHeader().GetAppId(AppType::GAME_SERVER);
	if (gameAppId != INVALID_APP_ID)
		return Result::USER_STATUS_ERROR;
	auto mainSocket = DbSocketUtil::GetMainSocketPtr();
	if (!mainSocket)
	{
		_DEBUG_RED;
		return Result::FATAL_ERROR;
	}

	{
		SocketUtil::Request<DM_REQ_GAME_USER_ENTER::Writer> wp(**mainSocket, REQ, mRp);
		wp.SetValues(
			mRp.Get_gameId()
		);

		if (!wp.Wait(GetCurrPacketHeader(), OUT mAck))
			return Result::FATAL_ERROR;

		if (Result::SUCCEEDED != mAck->GetHeader().GetPacketResult())
			return mAck->GetHeader().GetPacketResult();
	}
	
	gameAppId = mAck->GetHeader().GetAppId(AppType::GAME_SERVER);
	auto gameSocket = DbSocketUtil::GetGameSocket(gameAppId);
	if (!gameSocket)
		return Result::RETRY_LATER;
	
	// 유저 데이터 로드 하라고 요청하기 (account, user)
	{
		SocketUtil::Request<DG_REQ_GAME_USER_DATA_LOAD::Writer> wp(**gameSocket, REQ, *mAck);
		wp.SetValues(
			GetUserCache().GetAccountUser(),
			GetUserCache().GetUser()
		);

		if (!wp.Wait(GetCurrPacketHeader(), OUT mAck))
			return Result::FATAL_ERROR;

		if (Result::SUCCEEDED != mAck->GetHeader().GetPacketResult())
			return mAck->GetHeader().GetPacketResult();
	}

	// 여기서 스택을 보내야 할 듯
	// WORLD_USER_STAT_ALL::...
	// GetUserCache().Get<UserCharacterTable>();

	return Result::SUCCEEDED;
}

void Transactor_CD_REQ_GAME_USER_ENTER::OnFinish()
{	
	DbSocketUtil::SendToClient<DC_ACK_GAME_USER_ENTER::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.GetHeader().SetDbSeq(AddDbSeq());
}

void Transactor_CD_REQ_GAME_USER_ENTER::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_GAME_USER_ENTER::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
}

void Transactor_CD_REQ_GAME_USER_ENTER::OnLog()
{

}
