// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


class SocketDbToMain;
class MD_REQ_AUTH_TICKET;

class Transactor_MD_REQ_AUTH_TICKET : public UserAndPacketTransactor
{
private:
	SocketDbToMain& mMainSocket;
	MD_REQ_AUTH_TICKET& mRp;

public:
	explicit Transactor_MD_REQ_AUTH_TICKET(
		DbUser& dbUser, DbUserContext& userContext,
		SocketDbToMain& mainSocket,
		MD_REQ_AUTH_TICKET& rp
	);

protected:
	// 인증 티켓을 셋팅하는 단계이기 때문에 검증은 생략
	Result OnPreValidate() override;
	Result OnUpdate() override;
	void OnFinish() override;
	void OnError() override;
	void OnLog() override;
};
