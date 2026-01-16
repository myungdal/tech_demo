// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class DM_REQ_GAME_CREATE;
class GM_ACK_GAME_CREATE;
class Game;
class SocketMainFromDb;

class Transactor_DM_REQ_GAME_CREATE : public PacketTransactorBase
{
protected:
	SocketMainFromDb& mDbSocket;
	DM_REQ_GAME_CREATE& mRp;

	Game* mGamePtr = nullptr;

	GM_ACK_GAME_CREATE* mAck = nullptr;

public:
	explicit Transactor_DM_REQ_GAME_CREATE(
		SocketMainFromDb& dbSocket,
		DM_REQ_GAME_CREATE& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
