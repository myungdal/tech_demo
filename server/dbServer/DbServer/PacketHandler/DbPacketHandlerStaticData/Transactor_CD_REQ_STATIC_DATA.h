// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class CD_REQ_STATIC_DATA;
class SocketDbFromFront;

class Transactor_CD_REQ_STATIC_DATA : public UserAndPacketTransactor
{
private:
	SocketDbFromFront& mFrontSocket;
	CD_REQ_STATIC_DATA& mRp;

	StaticDataIndex mStaticDataIndex = INVALID_STATIC_DATA_INDEX;

public:
	explicit Transactor_CD_REQ_STATIC_DATA(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromFront& frontSocket,
		CD_REQ_STATIC_DATA& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
