// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class SocketDbFromFront;
class CD_REQ_GAME_CREATE;
class MD_ACK_GAME_CREATE;

class Transactor_CD_REQ_GAME_CREATE : public UserAndPacketTransactor
{
private:
	SocketDbFromFront& mFrontSocket;
	CD_REQ_GAME_CREATE& mRp;
	
	GameSid mGameSid = INVALID_SID;

	MD_ACK_GAME_CREATE* mAck = nullptr;

public:
	explicit Transactor_CD_REQ_GAME_CREATE(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromFront& frontSocket,
		CD_REQ_GAME_CREATE& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
