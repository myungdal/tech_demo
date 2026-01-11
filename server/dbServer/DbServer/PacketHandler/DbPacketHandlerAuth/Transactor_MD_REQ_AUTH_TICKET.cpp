// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbServer/PacketHandler/DbPacketHandlerAuth/Transactor_MD_REQ_AUTH_TICKET.h"


Transactor_MD_REQ_AUTH_TICKET::Transactor_MD_REQ_AUTH_TICKET(
	DbUser& dbUser, DbUserContext& userContext,
	SocketDbToMain& mainSocket,
	MD_REQ_AUTH_TICKET& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mMainSocket(mainSocket),
	mRp(rp)
{
}
// 인증 티켓을 셋팅하는 단계이기 때문에 검증은 생략
Result Transactor_MD_REQ_AUTH_TICKET::OnPreValidate()
{
	return Result::SUCCEEDED;
}

Result Transactor_MD_REQ_AUTH_TICKET::OnUpdate()
{
	// 인증 티켓 셋팅
	GetUserSocketState().SetAuthTicket(mRp.Get_authTicket());

	return Result::SUCCEEDED;
}

void Transactor_MD_REQ_AUTH_TICKET::OnFinish()
{
	SocketUtil::Send<DM_ACK_AUTH_TICKET::Writer> wp(mMainSocket, ACK, mRp, GetResult());
}

void Transactor_MD_REQ_AUTH_TICKET::OnError()
{
	SocketUtil::Send<DM_ACK_AUTH_TICKET::Writer> wp(mMainSocket, ACK, mRp, GetResult());
}

void Transactor_MD_REQ_AUTH_TICKET::OnLog()
{
	DbSocketUtil::SendLog<INSERT_LOG_ACHIEVEMENT::Writer> wp(
		UuidUtil::GenerateUuid(),
		UuidUtil::GenerateLogId(),
		123,
		123,
		123,
		tClock.GetGlobalNowTt(),
		tClock.GetGlobalNowTt()
	);
}
