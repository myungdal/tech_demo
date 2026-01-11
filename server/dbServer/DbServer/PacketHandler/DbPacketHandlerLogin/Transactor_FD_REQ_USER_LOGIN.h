// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class SP_USER_UPDATE_LOGIN;
class FD_REQ_USER_LOGIN;
class SocketDbFromFront;

class Transactor_FD_REQ_USER_LOGIN : public UserAndPacketTransactor
{
private:
	SocketDbFromFront& mFrontSocket;
	FD_REQ_USER_LOGIN& mRp;

	std::unique_ptr<SP_USER_UPDATE_LOGIN> mSpUserLogin;

public:
	explicit Transactor_FD_REQ_USER_LOGIN(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbFromFront& frontSocket,
		FD_REQ_USER_LOGIN& rp
	);

protected:
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
