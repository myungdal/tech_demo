// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class SP_ITEM_UPDATE_QUANTITY;
class CD_REQ_ITEM_USE;
class GD_ACK_ITEM_USE;
class SocketDbFromFront;

class Transactor_CD_REQ_ITEM_USE : public UserAndPacketTransactor
{
private:
	SocketDbFromFront& mFrontSocket;
	CD_REQ_ITEM_USE& mRp;

	std::unique_ptr<SP_ITEM_UPDATE_QUANTITY> mSpItemUpdateQuantity;

	GD_ACK_ITEM_USE* mAck = nullptr;

public:
	explicit Transactor_CD_REQ_ITEM_USE(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromFront& frontSocket,
		CD_REQ_ITEM_USE& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
