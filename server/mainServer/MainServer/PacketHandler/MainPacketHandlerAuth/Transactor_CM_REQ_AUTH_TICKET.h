// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class CM_REQ_AUTH_TICKET;
class DM_ACK_AUTH_TICKET;
class ACCOUNT_USER;
class SocketMainFromFront;

class Transactor_CM_REQ_AUTH_TICKET : public PacketTransactorBase
{
protected:
	SocketMainFromFront& mFrontSocket;
	CM_REQ_AUTH_TICKET& mRp;

	AccountPtr mAccountPtr = nullptr;
	const ACCOUNT_USER* mAccountUserPtr = nullptr;

	DM_ACK_AUTH_TICKET* mAck = nullptr;

public:
	explicit Transactor_CM_REQ_AUTH_TICKET(
		SocketMainFromFront& frontSocket,
		CM_REQ_AUTH_TICKET& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
