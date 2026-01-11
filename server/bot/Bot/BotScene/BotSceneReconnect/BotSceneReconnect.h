// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Bot/BotScene/Base/BotScene.h"


// 재접속 시나리오 씬
// - 저장된 인증 티켓으로 로그인 재시도
class BotSceneReconnect : public BotScene
{
public:
	explicit BotSceneReconnect(BotPtr& bot);

public:
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) override;
	void OnLostConnection() override;

public:
	bool OnDispatchPacket(NetworkPacket& rp) override;

public:
	HandleResult OnPacket(MAYBE_UNUSED NetworkPacket& rp);

	void CF_REQ_USER_LOGIN();
	HandleResult OnPacket(class FC_ACK_USER_LOGIN& rp);
};
