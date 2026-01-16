// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "Transactor_CD_REQ_CHEAT.h"

Transactor_CD_REQ_CHEAT::Transactor_CD_REQ_CHEAT(
	DbUser& dbUser, DbUserContext& userContext, 
	SocketDbFromFront& frontSocket,
	CD_REQ_CHEAT& rp
)
	:
	UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CD_REQ_CHEAT::OnUpdate()
{
	return gCheatManager->DispatchCommand(
		mRp.Get_command(), 
		mFrontSocket, 		
		GetUserSocketState(),
		GetUserCache(),
		GetMainDbSession(),
		GetUserDbSession()
	);
}
void Transactor_CD_REQ_CHEAT::OnFinish()
{
	DbUserDataMaker dbUserDataMaker(GetUserCache());

	bool isCompleted = false;
	bool isFirst = true;

	do
	{
		const USER_DATA::Writer& userData = dbUserDataMaker.MakeUserData(TEMP_BUF, isCompleted);

		DbSocketUtil::SendToClient<DC_ACK_CHEAT::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
		wp.GetHeader().SetDbSeq(AddDbSeq());
		if (isFirst)
		{
			isFirst = false;

			wp.SetValues(
				GetUserCacheDiffWp(),
				userData,
				isCompleted ? 1 : 0
			);
		}
		else
		{
			wp.SetValues(
				USER_CACHE_DIFF_WRITER(nullptr),
				userData,
				isCompleted ? 1 : 0
			);
		}

	} while (false == isCompleted);
}

void Transactor_CD_REQ_CHEAT::OnError()
{
	DbSocketUtil::SendToClient<DC_ACK_CHEAT::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		USER_CACHE_DIFF_WRITER(nullptr),
		USER_DATA_WRITER(nullptr),
		1
	);
}

void Transactor_CD_REQ_CHEAT::OnLog()
{

}
