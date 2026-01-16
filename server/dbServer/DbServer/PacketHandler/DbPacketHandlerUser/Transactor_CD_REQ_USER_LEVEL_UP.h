// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class CD_REQ_USER_LEVEL_UP;
class SP_USER_UPDATE_EXP;
class SocketDbFromFront;

class Transactor_CD_REQ_USER_LEVEL_UP : public UserAndPacketTransactor
{
private:
	SocketDbFromFront& mFrontSocket;
	CD_REQ_USER_LEVEL_UP& mRp;

	std::unique_ptr<SP_USER_UPDATE_EXP> mSpUserUpdateExp;

public:
	explicit Transactor_CD_REQ_USER_LEVEL_UP(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromFront& frontSocket,
		CD_REQ_USER_LEVEL_UP& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
