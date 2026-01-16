// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEngineDb;

#include "Transactor_CD_REQ_ITEM_USE.h"

Transactor_CD_REQ_ITEM_USE::Transactor_CD_REQ_ITEM_USE(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromFront& frontSocket,
	CD_REQ_ITEM_USE& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CD_REQ_ITEM_USE::OnUpdate()
{
	const AppId gameAppId = GetCurrPacketHeader().GetAppId(AppType::GAME_SERVER);
	if (gameAppId == INVALID_APP_ID)
		return Result::USER_STATUS_ERROR;

	UserItemTablePtr userItemTable = GetUserCache().Get<UserItemTable>();
	UserItemRowPtr userItemRow = userItemTable->Find(mRp.Get_itemId());
	if (!userItemRow)
		return Result::NOT_ENOUGH_ITEM;

	Result result = userItemRow->IncreaseItemQuantity(-mRp.Get_itemQuantity());
	if (Result::SUCCEEDED != result)
		return result;

	ITEM& item = userItemRow->Data();

	// DB 에 아이템 차감 셋팅
	mSpItemUpdateQuantity = std::make_unique<SP_ITEM_UPDATE_QUANTITY>(
		GetUserDbSession(),
		item.Get_c_item_id(),
		item.Get_c_item_quantity(),
		tClock.GetGlobalNowTs()
	);
	result = DbUtil::ExecuteSp(*mSpItemUpdateQuantity);
	if (Result::SUCCEEDED != result)
		return result;

	auto gameSocket = DbSocketUtil::GetGameSocket(gameAppId);
	if (!gameSocket)
		return Result::RETRY_LATER;

	// 게임 아이템 사용 요청
	SocketUtil::Request<DG_REQ_ITEM_USE::Writer> wp(**gameSocket, REQ, mRp);
	wp.SetValues(
		item.Get_c_item_id(),
		item.Get_c_item_quantity()
	);

	if (!wp.Wait(GetCurrPacketHeader(), OUT mAck))
		return Result::FATAL_ERROR;

	if (Result::SUCCEEDED != mAck->GetHeader().GetPacketResult())
		return mAck->GetHeader().GetPacketResult();

	return Result::SUCCEEDED;
}

void Transactor_CD_REQ_ITEM_USE::OnFinish()
{	
	DbSocketUtil::SendToClient<DC_ACK_ITEM_USE::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.GetHeader().SetDbSeq(AddDbSeq());
	//wp.SetValues(
	//);
}

void Transactor_CD_REQ_ITEM_USE::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_ITEM_USE::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	//wp.SetValues(
	//);
}

void Transactor_CD_REQ_ITEM_USE::OnLog()
{

}
