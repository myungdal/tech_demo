// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;
import ServerEngineDb;

#include "Transactor_CM_REQ_AUTH_MODIFY.h"

Transactor_CM_REQ_AUTH_MODIFY::Transactor_CM_REQ_AUTH_MODIFY(
	SocketMainFromFront& frontSocket,
	CM_REQ_AUTH_MODIFY& rp
)
	:
	DbAndPacketTransactor(rp.GetHeader()),
	mFrontSocket(frontSocket),
	mRp(rp)
{
}
Result Transactor_CM_REQ_AUTH_MODIFY::OnUpdate()
{
	const ACCOUNT& account = mRp.Get_account();
	const bool canModifyAccount = gAccountManager->CanModifyAccount(account);
	if (false == canModifyAccount)
		return Result::AUTH_ERROR;

	SP_ACCOUNT_UPDATE_TOKEN spAccountUpdateToken(
		GetMainDbSession(),
		account.Get_c_account_id().GetData(),
		account.Get_c_device_token(),
		account.Get_c_google_token(),
		account.Get_c_apple_token(),
		tClock.GetGlobalNowTs()
	);
	Result result = DbUtil::ExecuteSp(spAccountUpdateToken);
	if (Result::SUCCEEDED != result)
		return result;

	if (false == gAccountManager->ModifyAccount(account))
		return Result::FATAL_ERROR;

	return Result::SUCCEEDED;
}

void Transactor_CM_REQ_AUTH_MODIFY::OnFinish()
{
	MainSocketUtil::SendToClient<MC_ACK_AUTH_MODIFY::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		mRp.Get_account()
	);
}

void Transactor_CM_REQ_AUTH_MODIFY::OnError()
{
	MainSocketUtil::SendToClient<MC_ACK_AUTH_MODIFY::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
	wp.SetValues(
		mRp.Get_account()
	);
}

void Transactor_CM_REQ_AUTH_MODIFY::OnLog()
{

}
