// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class CD_REQ_GAME_USER_ENTER;
class MD_ACK_GAME_USER_ENTER;
class SocketDbFromFront;

class Transactor_CD_REQ_GAME_USER_ENTER : public UserAndPacketTransactor
{
private:
	SocketDbFromFront& mFrontSocket;
	CD_REQ_GAME_USER_ENTER& mRp;

	MD_ACK_GAME_USER_ENTER* mAck = nullptr;

public:
	explicit Transactor_CD_REQ_GAME_USER_ENTER(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromFront& frontSocket,
		CD_REQ_GAME_USER_ENTER& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
