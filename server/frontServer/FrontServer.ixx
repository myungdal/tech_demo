// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// FrontServer.ixx - FrontServer Module Interface
module;

#pragma warning(disable: 5028)

#include "FrontServer/PCH/pch.h"

// FrontServer 핵심 헤더들
#include "FrontServer/App/FrontServerApp.h"
#include "FrontServer/FrontData/FrontDataManager.h"
#include "FrontServer/FrontData/FrontUser.h"
#include "FrontServer/Chatting/ChattingDelayCalculator.h"

export module FrontServer;

// ServerEngine modules
export import ServerEngineCore;
export import ServerEngineWorker;
export import ServerEngineNetwork;
export import ServerEngineApp;
export import ServerEngineUtil;

// FrontServer split modules
export import FrontServerSocket;
export import FrontServerPacket;
export import FrontServerChat;

export
{
	using ::FrontServerApp;
	using ::gFrontServerApp;
}
