// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// FrontServerChat.ixx - FrontServer Chat Module Interface
module;

#pragma warning(disable: 5028)

#include "FrontServer/PCH/pch.h"

// Chatting header
#include "FrontServer/Chatting/ChattingDelayCalculator.h"

// FrontData headers
#include "FrontServer/FrontData/FrontDataManager.h"
#include "FrontServer/FrontData/FrontUser.h"

export module FrontServerChat;

export import ServerEngineCore;
export import ServerEngineApp;
export
{
	using ::ChattingDelayCalculator;
	using ::gChattingDelayCalculator;
	using ::FrontDataManager;
	using ::gFrontDataManager;
	using ::FrontUser;
}
