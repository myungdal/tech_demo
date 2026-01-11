// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"

#include "MainServer/AccountManager/Account.h"


class SocketMainFromFront;
class CM_REQ_ACCOUNT_USER_LIST;

class Transactor_CM_REQ_ACCOUNT_USER_LIST : public PacketTransactorBase
{
protected:
	SocketMainFromFront& mFrontSocket;
	CM_REQ_ACCOUNT_USER_LIST& mRp;

	const AccountUserList* mAccountUserListPtr = nullptr;

public:
	explicit Transactor_CM_REQ_ACCOUNT_USER_LIST(
		SocketMainFromFront& frontSocket,
		CM_REQ_ACCOUNT_USER_LIST& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
