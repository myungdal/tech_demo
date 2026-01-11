// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"


class DM_REQ_GAME_FINISH;
class SocketMainFromDb;

class Transactor_DM_REQ_GAME_FINISH : public PacketTransactorBase
{
protected:
	SocketMainFromDb& mDbSocket;
	DM_REQ_GAME_FINISH& mRp;

public:
	explicit Transactor_DM_REQ_GAME_FINISH(
		SocketMainFromDb& dbSocket,
		DM_REQ_GAME_FINISH& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
