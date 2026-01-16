// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "Transactor_CD_REQ_USER_DATA.h"

Transactor_CD_REQ_USER_DATA::Transactor_CD_REQ_USER_DATA(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromFront& frontSocket,
	CD_REQ_USER_DATA& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CD_REQ_USER_DATA::OnUpdate()
{
	return Result::SUCCEEDED;
}
void Transactor_CD_REQ_USER_DATA::OnFinish()
{	
	DbUserDataMaker dbUserDataMaker(GetUserCache());

	bool isCompleted = false;

	do
	{
		DbSocketUtil::SendToClient<DC_ACK_USER_DATA::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
		wp.GetHeader().SetDbSeq(AddDbSeq());

		const USER_DATA::Writer& userData = dbUserDataMaker.MakeUserData(TEMP_BUF, isCompleted);

		wp.SetValues(
			userData,
			isCompleted ? 1 : 0
		);

	} while (false == isCompleted);
}

void Transactor_CD_REQ_USER_DATA::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_USER_DATA::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		{},
		1
	);
}

void Transactor_CD_REQ_USER_DATA::OnLog()
{

}
