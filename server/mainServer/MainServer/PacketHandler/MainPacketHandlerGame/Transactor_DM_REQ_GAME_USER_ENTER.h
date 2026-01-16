// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class DM_REQ_GAME_USER_ENTER;
class GM_ACK_GAME_USER_ENTER;
class SocketMainFromDb;

class Transactor_DM_REQ_GAME_USER_ENTER : public PacketTransactorBase
{
protected:
	SocketMainFromDb& mDbSocket;
	DM_REQ_GAME_USER_ENTER& mRp;

	GM_ACK_GAME_USER_ENTER* mAck = nullptr;

public:
	explicit Transactor_DM_REQ_GAME_USER_ENTER(
		SocketMainFromDb& dbSocket,
		DM_REQ_GAME_USER_ENTER& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
