// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class GD_REQ_GAME_FINISH;
class MD_ACK_GAME_FINISH;
class SocketDbFromGame;

class Transactor_GD_REQ_GAME_FINISH : public UserAndPacketTransactor
{
private:
	SocketDbFromGame& mGameSocket;
	GD_REQ_GAME_FINISH& mRp;

	MD_ACK_GAME_FINISH* mAck = nullptr;

public:
	explicit Transactor_GD_REQ_GAME_FINISH(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromGame& gameSocket,
		GD_REQ_GAME_FINISH& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
