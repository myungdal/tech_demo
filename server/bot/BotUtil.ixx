// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// BotUtil.ixx - Bot Util Module Interface
module;

#pragma warning(disable: 5028)

#include "Bot/PCH/pch.h"

// BotScenarioUtil.h를 GMF에서 include (패킷 헤더 의존성 해결)
#include "Bot/Util/BotScenarioUtil/BotScenarioUtil.h"
#include "Bot/Util/BotSocketUtil/BotSocketUtil.h"

export module BotUtil;

export import ServerEngineCore;
export import ServerEngineUtil;
export
{
	// BotScenarioUtil 함수 및 템플릿 export
	using ::ValidatePacket;
	using ::DispatchPacket;
	using ::DispatchPacketToParent;

	using ::BotSocketUtil;
}
