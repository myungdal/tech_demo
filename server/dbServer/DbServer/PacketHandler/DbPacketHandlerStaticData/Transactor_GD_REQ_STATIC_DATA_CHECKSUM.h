// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class GD_REQ_STATIC_DATA_CHECKSUM;
class SocketDbFromGame;

class Transactor_GD_REQ_STATIC_DATA_CHECKSUM : public PacketTransactorBase
{
private:
	SocketDbFromGame& mGameSocket;
	GD_REQ_STATIC_DATA_CHECKSUM& mRp;

public:
	explicit Transactor_GD_REQ_STATIC_DATA_CHECKSUM(
		SocketDbFromGame& gameSocket,
		GD_REQ_STATIC_DATA_CHECKSUM& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
