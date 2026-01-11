// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/DbAndPacketTransactor.h"


class CM_REQ_AUTH_MODIFY;
class SocketMainFromFront;

class Transactor_CM_REQ_AUTH_MODIFY : public DbAndPacketTransactor
{
protected:
	SocketMainFromFront& mFrontSocket;
	CM_REQ_AUTH_MODIFY& mRp;

public:
	explicit Transactor_CM_REQ_AUTH_MODIFY(
		SocketMainFromFront& frontSocket,
		CM_REQ_AUTH_MODIFY& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
