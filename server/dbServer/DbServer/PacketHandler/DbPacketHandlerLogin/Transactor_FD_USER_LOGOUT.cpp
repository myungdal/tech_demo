// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEngineDb;

#include "Transactor_FD_USER_LOGOUT.h"

Transactor_FD_USER_LOGOUT::Transactor_FD_USER_LOGOUT(
	DbUser& dbUser, DbUserContext& userContext,
	SocketDbFromFront& frontSocket,
	FD_USER_LOGOUT& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_FD_USER_LOGOUT::OnUpdate()
{
	if (GetCurrPacketHeader().GetGameId() != INVALID_UUID)
	{
		if (auto mainSocket = DbSocketUtil::GetMainSocketPtr())
		{
			SocketUtil::Request<DM_REQ_GAME_USER_LEAVE::Writer> wp(**mainSocket, REQ, mRp);
			MD_ACK_GAME_USER_LEAVE* ack = nullptr;
			[[maybe_unused]] bool result = wp.Wait(GetCurrPacketHeader(), OUT ack);
		}
	}
	const time_t& nowTt = tClock.GetGlobalNowTt();
	const TIMESTAMP_STRUCT& nowTs = tClock.GetGlobalNowTs();
	const time_t& dateLogin = GetUserCache().GetUser().Ref_c_date_login();
	const time_t playTime = (nowTt > dateLogin) ? (nowTt - dateLogin) : 0;

	// DB에 유저 로그아웃
	SP_USER_UPDATE_LOGOUT spUserLogout(
		GetUserDbSession(),
		GetUserId(),
		playTime,
		nowTs
	);
	Result result = DbUtil::ExecuteSp(spUserLogout);
	if (Result::SUCCEEDED != result)
		return result;

	// 로그아웃 시간 기록
	GetUserCache().GetUser().Ref_c_date_logout() = nowTt;

	// 세션 초기화
	GetUserSocketState().SetSocket(nullptr);

	return Result::SUCCEEDED;
}

void Transactor_FD_USER_LOGOUT::OnFinish()
{	
}

void Transactor_FD_USER_LOGOUT::OnError()
{
}

void Transactor_FD_USER_LOGOUT::OnLog()
{

}
