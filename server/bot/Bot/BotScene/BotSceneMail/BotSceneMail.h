// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Bot/BotScene/Base/BotScene.h"


// 우편 관련 시나리오 씬
// - 우편 생성 치트, 우편 목록 조회, 우편 읽기 처리
class BotSceneMail : public BotScene
{
public:
	explicit BotSceneMail(BotPtr& bot);

public:
	void OnLostConnection() override;

public:
	bool OnDispatchPacket(NetworkPacket& rp) override;

public:
	HandleResult OnPacket(MAYBE_UNUSED NetworkPacket& rp);

	void CD_REQ_MAIL_LIST();
	HandleResult OnPacket(class DC_ACK_MAIL_LIST& rp);

	void CD_REQ_CHEAT_CREATE_MAIL();

	void CD_REQ_MAIL_READ();
	HandleResult OnPacket(class DC_ACK_MAIL_READ& rp);

private:
	MailId mTestMailId = INVALID_UUID;
};
