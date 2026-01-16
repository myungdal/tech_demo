// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ClientPacketFlow/Base/ClientPacketFlowBase.h"
#include "Util/CommandDispatcher/CommandDispatcher.h"


/**
 * 공통 패킷 플로우.
 * 쉘/디버그 커맨드 등 전체 모드에서 공통으로 처리하는 패킷을 담당합니다.
 */
class FClientPacketFlowCommon : public FClientPacketFlowBase
{
protected:
	CommandDispatcher<> mCommandDispatcher;

public:
	FClientPacketFlowCommon();

public:
	void OnDispatchPacket(NetworkPacket& rp) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
	HandleResult OnPacket(class SHELL_NOTIFY& rp);
	void OnCommand_print(ArgList& argList);
	void OnCommand_pause(ArgList& argList);
	void OnCommand_resume(ArgList& argList);
	void OnCommand_rewind(ArgList& argList);
	void OnCommand_reloaded(ArgList& argList);
	void OnCommand_debug(ArgList& argList);
	HandleResult OnPacket(class DC_ACK_CHEAT& rp);	
};

