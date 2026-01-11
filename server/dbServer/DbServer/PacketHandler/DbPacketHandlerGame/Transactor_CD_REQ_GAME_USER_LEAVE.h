// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class CD_REQ_GAME_USER_LEAVE;
class MD_ACK_GAME_USER_LEAVE;
class SocketDbFromFront;

class Transactor_CD_REQ_GAME_USER_LEAVE : public UserAndPacketTransactor
{
private:
	SocketDbFromFront& mFrontSocket;
	CD_REQ_GAME_USER_LEAVE& mRp;

	MD_ACK_GAME_USER_LEAVE* mAck = nullptr;

public:
	explicit Transactor_CD_REQ_GAME_USER_LEAVE(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromFront& frontSocket,
		CD_REQ_GAME_USER_LEAVE& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
