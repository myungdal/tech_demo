// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class FD_USER_LOGOUT;
class SocketDbFromFront;

class Transactor_FD_USER_LOGOUT : public UserAndPacketTransactor
{
private:
	SocketDbFromFront& mFrontSocket;
	FD_USER_LOGOUT& mRp;

public:
	explicit Transactor_FD_USER_LOGOUT(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromFront& frontSocket,
		FD_USER_LOGOUT& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
