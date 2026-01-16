// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class GD_REQ_STATIC_DATA;
class SocketDbFromGame;

class Transactor_GD_REQ_STATIC_DATA : public PacketTransactorBase
{
private:
	SocketDbFromGame& mGameSocket;
	GD_REQ_STATIC_DATA& mRp;

	StaticDataIndex mStaticDataIndex = INVALID_STATIC_DATA_INDEX;

public:
	explicit Transactor_GD_REQ_STATIC_DATA(
		SocketDbFromGame& gameSocket,
		GD_REQ_STATIC_DATA& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
