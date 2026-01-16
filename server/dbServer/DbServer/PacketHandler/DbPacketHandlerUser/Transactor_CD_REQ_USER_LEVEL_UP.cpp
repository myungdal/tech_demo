// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEngineDb;
import ServerEnginePacket;

#include "Transactor_CD_REQ_USER_LEVEL_UP.h"

Transactor_CD_REQ_USER_LEVEL_UP::Transactor_CD_REQ_USER_LEVEL_UP(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromFront& frontSocket,
	CD_REQ_USER_LEVEL_UP& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CD_REQ_USER_LEVEL_UP::OnUpdate()
{
	std::shared_ptr<UserExpDocLookup> userExpDocLookup = gStaticDataAccessor->Get<UserExpDocLookup>();
	if (!userExpDocLookup)
		return Result::RETRY_LATER;
	ACCOUNT_USER& accountUser = GetUserCache().GetAccountUser();
	USER& user = GetUserCache().GetUser();
	const UserLevel userLevel = accountUser.Get_c_user_level();
	const UserExp userExp = user.Get_c_user_exp();
	const UserExpDoc* userExpDoc = userExpDocLookup->FindByExp(userExp);

	if (!userExpDoc)
		return Result::WRONG_VALUE;
		
	if (!userExpDoc->mUserExp)
		return Result::WRONG_VALUE;

	if (userExpDoc->mUserExp->Get_c_user_level() <= userLevel)
		return Result::WRONG_VALUE;

	// 다음 레벨을 구한다		
	const UserLevel nextUserLevel = (userLevel + 1);

	const UserExpDoc* nextUserExpDoc = userExpDocLookup->FindByLevel(nextUserLevel);

	if (!nextUserExpDoc)
		return Result::WRONG_VALUE;
		
	if (!nextUserExpDoc->mUserExp)
		return Result::WRONG_VALUE;

	mSpUserUpdateExp = std::make_unique<SP_USER_UPDATE_EXP>(
		GetUserDbSession(),
		GetUserId(),
		userExp,
		nextUserLevel,
		tClock.GetGlobalNowTs()
	);
	Result result = DbUtil::ExecuteSp(*mSpUserUpdateExp);
	if (Result::SUCCEEDED != result)
		return result;

	// 유저 레벨 보상 우편 지급

	// 유저 레벨 셋팅
	accountUser.Ref_c_user_level() = nextUserLevel;

	return Result::SUCCEEDED;
}

void Transactor_CD_REQ_USER_LEVEL_UP::OnFinish()
{
	ACCOUNT_USER& accountUser = GetUserCache().GetAccountUser();
	const UserLevel userLevel = accountUser.Get_c_user_level();

	DbSocketUtil::SendToClient<DC_ACK_USER_LEVEL_UP::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.GetHeader().SetDbSeq(AddDbSeq());
	wp.SetValues(
		userLevel
	);
}

void Transactor_CD_REQ_USER_LEVEL_UP::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_USER_LEVEL_UP::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		MIN_USER_LEVEL
	);
}

void Transactor_CD_REQ_USER_LEVEL_UP::OnLog()
{

}
