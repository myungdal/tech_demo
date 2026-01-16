// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;

#include "Transactor_CD_REQ_GAME_CREATE.h"

Transactor_CD_REQ_GAME_CREATE::Transactor_CD_REQ_GAME_CREATE(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromFront& frontSocket,
	CD_REQ_GAME_CREATE& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}

Result Transactor_CD_REQ_GAME_CREATE::OnUpdate()
{
	// 아이템 테이블 참조
	UserItemTablePtr userItemTable = GetUserCache().Get<UserItemTable>();
	UserItemRowPtr userItemRow = userItemTable->Find(mRp.Get_itemId());
	if (!userItemRow)
		return Result::NOT_ENOUGH_ITEM;
	
	const ItemDoc* itemDoc = userItemRow->GetDoc();
	if (!itemDoc)
		return Result::INVALID_UUID;

	const STATIC_ITEM* staticItem = itemDoc->mStaticItem;
	if (!staticItem)
		return Result::INVALID_UUID;

	ITEM& item = userItemRow->Data();

	// DB에 아이템 차감
	Result result = ItemUtil::DeleteItem(GetUserDbSession(), GetUserCache(), item.Get_c_item_id(), 1);
	if (Result::SUCCEEDED != result)
		return result;

	// 아이템 효과에 의해 GameSid 가 반환되어야 할 것

	auto mainSocket = DbSocketUtil::GetMainSocketPtr();
	if (!mainSocket)
	{
		_DEBUG_RED;
		return Result::FATAL_ERROR;
	}

	// 게임 생성 요청
	SocketUtil::Request<DM_REQ_GAME_CREATE::Writer> wp(**mainSocket, REQ, mRp);
	wp.SetValues(
		mGameSid
	);
	
	if (!wp.Wait(GetCurrPacketHeader(), OUT mAck))
		return Result::FATAL_ERROR;
	
	if (Result::SUCCEEDED != mAck->GetHeader().GetPacketResult())
		return mAck->GetHeader().GetPacketResult();

	return Result::SUCCEEDED;
}

void Transactor_CD_REQ_GAME_CREATE::OnFinish()
{	
	DbSocketUtil::SendToClient<DC_ACK_GAME_CREATE::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.GetHeader().SetDbSeq(AddDbSeq());
	wp.SetValues(
		mRp.Get_itemId(),
		GetUserCacheDiffWp().GetPacket(),
		mAck->Get_game()
	);
}

void Transactor_CD_REQ_GAME_CREATE::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_GAME_CREATE::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		INVALID_UUID,
		{},
		{}
	);
}

void Transactor_CD_REQ_GAME_CREATE::OnLog()
{

}
