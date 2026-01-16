// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Bot/BotScene/Base/BotScene.h"


// 연결 해제 시나리오 씬
// - 재접속 비활성화 후 연결 종료
class BotSceneDisconnect : public BotScene
{
public:
	explicit BotSceneDisconnect(BotPtr& bot);

public:
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) override;
	void OnLostConnection() override;

public:
	bool OnDispatchPacket(NetworkPacket& rp) override;

public:
	HandleResult OnPacket(MAYBE_UNUSED NetworkPacket& rp);
	void DISCONNECT();
};
