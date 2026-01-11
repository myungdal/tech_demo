// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class DM_REQ_GAME_USER_LEAVE;
class GM_ACK_GAME_USER_LEAVE;
class SocketMainFromDb;

class Transactor_DM_REQ_GAME_USER_LEAVE : public PacketTransactorBase
{
protected:
	SocketMainFromDb& mDbSocket;
	DM_REQ_GAME_USER_LEAVE& mRp;

	GM_ACK_GAME_USER_LEAVE* mAck = nullptr;

public:
	explicit Transactor_DM_REQ_GAME_USER_LEAVE(
		SocketMainFromDb& dbSocket,
		DM_REQ_GAME_USER_LEAVE& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
