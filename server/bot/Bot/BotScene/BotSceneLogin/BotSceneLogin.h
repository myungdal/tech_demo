// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Bot/BotScene/Base/BotScene.h"


// 로그인 시나리오 씬
// - 연결, 서버 목록 조회, 계정 인증, 유저 데이터 로드 순서 처리
class BotSceneLogin : public BotScene
{
private:
	static constexpr ClockMs MAX_TIME_SYNC_DELTA = 200ms;

	static inline std::atomic<BotId> mBotIdSyncingStaticData = INVALID_BOT_ID;

public:
	explicit BotSceneLogin(BotPtr& bot);

public:
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) override;
	void OnLostConnection() override;

public:
	bool OnDispatchPacket(NetworkPacket& rp) override;

public:
	HandleResult OnPacket(MAYBE_UNUSED NetworkPacket& rp);

	void CF_CONNECT();

	void REQ_GLOBAL_NOW();
	HandleResult OnPacket(class ACK_GLOBAL_NOW& rp);

	void CF_REQ_PACKET_LIST();
	HandleResult OnPacket(class FC_ACK_PACKET_LIST& rp);

	void CM_REQ_SERVER_LIST();
	HandleResult OnPacket(class MC_ACK_SERVER_LIST& rp);

	void CM_REQ_ACCOUNT_USER_LIST();
	HandleResult OnPacket(class MC_ACK_ACCOUNT_USER_LIST& rp);

	void CM_REQ_ACCOUNT_USER_CREATE();
	HandleResult OnPacket(class MC_ACK_ACCOUNT_USER_CREATE& rp);

	void CM_REQ_AUTH_TICKET();
	HandleResult OnPacket(class MC_ACK_AUTH_TICKET& rp);

	void CF_REQ_USER_LOGIN();
	HandleResult OnPacket(class FC_ACK_USER_LOGIN& rp);

	void CD_REQ_STATIC_DATA_CHECKSUM();
	HandleResult OnPacket(class DC_ACK_STATIC_DATA_CHECKSUM& rp);

	void CD_REQ_STATIC_DATA();
	HandleResult OnPacket(class DC_ACK_STATIC_DATA& rp);

	void CD_REQ_USER_DATA();
	HandleResult OnPacket(class DC_ACK_USER_DATA& rp);
};
