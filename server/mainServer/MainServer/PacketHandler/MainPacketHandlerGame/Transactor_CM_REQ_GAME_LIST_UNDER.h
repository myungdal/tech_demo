// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class CM_REQ_GAME_LIST_UNDER;
class SocketMainFromFront;

class Transactor_CM_REQ_GAME_LIST_UNDER : public PacketTransactorBase
{
protected:
	SocketMainFromFront& mFrontSocket;
	CM_REQ_GAME_LIST_UNDER& mRp;

public:
	explicit Transactor_CM_REQ_GAME_LIST_UNDER(
		SocketMainFromFront& frontSocket,
		CM_REQ_GAME_LIST_UNDER& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
