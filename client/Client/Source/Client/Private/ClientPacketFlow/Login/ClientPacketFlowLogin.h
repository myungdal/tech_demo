// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "ClientPacketFlow/Base/ClientPacketFlowBase.h"


/**
 * 로그인/초기화 패킷 플로우.
 */
class FClientPacketFlowLogin : public FClientPacketFlowBase
{
private:
	static constexpr int32_t MAX_TIME_SYNC_TRY_CNT = 5;
	static constexpr ClockMs MAX_TIME_SYNC_DELTA = 600ms;

private:
	int32_t mTimeSyncTryCnt = 0;

public:
	DISABLE_COPY(FClientPacketFlowLogin);
	explicit FClientPacketFlowLogin();
	~FClientPacketFlowLogin() = default;

public:
	void OnDispatchPacket(NetworkPacket& rp) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
	
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	
public:
	void OnConnected() override;
	void OnFatalError() override;

	void REQ_GLOBAL_NOW();
	HandleResult OnPacket(class ACK_GLOBAL_NOW& rp);

	void CF_REQ_PACKET_LIST();
	HandleResult OnPacket(class FC_ACK_PACKET_LIST& rp);

	void CM_REQ_SERVER_LIST();
	HandleResult OnPacket(class MC_ACK_SERVER_LIST& rp);

	void CM_REQ_ACCOUNT_USER_LIST();
	HandleResult OnPacket(class MC_ACK_ACCOUNT_USER_LIST& rp);

	void CM_REQ_ACCOUNT_USER_CREATE();
	void OnUiEvent(class UUiEvent_POPUP_SERVER_LIST_OK_CLICKED& uiEvent);
	void OnUiEvent(class UUiEvent_POPUP_USER_NAME_OK_CLICKED_FINAL& uiEvent);
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

