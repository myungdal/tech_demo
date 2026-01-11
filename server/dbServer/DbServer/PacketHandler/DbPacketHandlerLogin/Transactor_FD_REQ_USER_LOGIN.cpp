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

#include "Transactor_FD_REQ_USER_LOGIN.h"

Transactor_FD_REQ_USER_LOGIN::Transactor_FD_REQ_USER_LOGIN(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromFront& frontSocket,
	FD_REQ_USER_LOGIN& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_FD_REQ_USER_LOGIN::OnUpdate()
{
	if (mRp.Get_authTicket() == INVALID_AUTH_TICKET)
		return Result::AUTH_ERROR;
	if (GetUserSocketState().GetAuthTicket() != mRp.Get_authTicket())
		return Result::AUTH_ERROR;
	if (false == gStaticDataCatalog->IsSynced())
		return Result::RETRY_LATER;
	mSpUserLogin = std::make_unique<SP_USER_UPDATE_LOGIN>(
		GetUserDbSession(),
		GetUserId(),
		tClock.GetGlobalNowTs()
	);
	Result result = DbUtil::ExecuteSp(*mSpUserLogin);
	if (Result::SUCCEEDED != result)
		return result;
	// 로그인 시간 기록
	GetUserCache().GetUser().Ref_c_date_login() = tClock.GetGlobalNowTt();

	// 세션 셋팅
	GetUserSocketState().SetSocket(&mFrontSocket);

	// 한번 사용 된 인증 티켓 무효화
	GetUserSocketState().SetAuthTicket(INVALID_AUTH_TICKET);

	return Result::SUCCEEDED;
}

void Transactor_FD_REQ_USER_LOGIN::OnFinish()
{	
	// 로그인 성공 응답
	SocketUtil::Send<DF_ACK_USER_LOGIN::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.GetHeader().SetDbSeq(AddDbSeq());
	wp.SetValues(
		GetUserCache().GetAccountUser(),
		GetUserCache().GetUser(),
		gStaticDataCatalog->GetMyChecksumForClient()
	);
}

void Transactor_FD_REQ_USER_LOGIN::OnError()
{
	SocketUtil::Send<DF_ACK_USER_LOGIN::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		{},
		{},
		{}
	);
}

void Transactor_FD_REQ_USER_LOGIN::OnLog()
{

}
