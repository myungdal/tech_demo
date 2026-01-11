// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Bot/BotScene/Base/BotScene.h"


// 게임 플레이 시나리오 씬
// - 게임 목록 조회, 게임 생성, 입장/퇴장 처리
class BotSceneGame : public BotScene
{
public:
	explicit BotSceneGame(BotPtr& bot);

public:
	void OnLostConnection() override;

public:
	bool OnDispatchPacket(NetworkPacket& rp) override;

public:
	HandleResult OnPacket(MAYBE_UNUSED NetworkPacket& rp);

	void CD_REQ_CHEAT_CREATE_ITEM_GAME();
	HandleResult OnPacket(class DC_ACK_CHEAT& rp);

	void CM_REQ_GAME_LIST();
	HandleResult OnPacket(class MC_ACK_GAME_LIST_OVER& rp);

	void CD_REQ_GAME_CREATE();
	HandleResult OnPacket(class DC_ACK_GAME_CREATE& rp);

	void CD_REQ_GAME_USER_ENTER();
	HandleResult OnPacket(class DC_ACK_GAME_USER_ENTER& rp);

	void CF_REQ_GAME_CHANNEL_USER_ENTER();
	HandleResult OnPacket(class FC_ACK_GAME_CHANNEL_USER_ENTER& rp);

	void CD_REQ_GAME_USER_LEAVE();
	HandleResult OnPacket(MAYBE_UNUSED class DC_ACK_GAME_USER_LEAVE& rp);
};
