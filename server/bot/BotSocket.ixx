// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// BotSocket.ixx - Bot Socket Module Interface
module;

#pragma warning(disable: 5028)

#include "Bot/PCH/pch.h"

#include "Bot/Socket/SocketBot.h"
#include "Bot/Socket/SocketBotToFront.h"

export module BotSocket;

export import ServerEngineCore;
export import ServerEngineNetwork;
export
{
	using ::SocketBot;
	using ::SocketBotToFront;
}
