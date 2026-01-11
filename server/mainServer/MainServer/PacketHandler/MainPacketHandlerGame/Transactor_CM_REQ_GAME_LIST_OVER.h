// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class CM_REQ_GAME_LIST_OVER;
class SocketMainFromFront;

class Transactor_CM_REQ_GAME_LIST_OVER : public PacketTransactorBase
{
protected:
	SocketMainFromFront& mFrontSocket;
	CM_REQ_GAME_LIST_OVER& mRp;

public:
	explicit Transactor_CM_REQ_GAME_LIST_OVER(
		SocketMainFromFront& frontSocket,
		CM_REQ_GAME_LIST_OVER& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
