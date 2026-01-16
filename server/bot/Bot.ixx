// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// Bot.ixx - Bot Module Interface
module;

#pragma warning(disable: 5028)

#include "Bot/PCH/pch.h"

// ServerEngine headers
#include "ServerEngine/AppConfigManager/AppConfigData.h"
#include "ServerEngine/Worker/WorkerBase/Worker.h"
#include "ServerEngine/Socket/SocketImpl/PeerSocket.h"
#include "ServerEngine/Log/LogInternal/LogWriterFile.h"

// Bot headers
#include "Bot/App/BotApp.h"

export module Bot;

// ServerEngine modules
export import ServerEngineCore;
export import ServerEngineNetwork;
export import ServerEngineWorker;
export import ServerEngineApp;
export import ServerEngineUtil;
export import ServerEngineLog;
export import ServerEnginePacket;

// Bot split modules
export import BotSocket;
export import BotScene;
export import BotUtil;

export
{
	using ::BotApp;
	using ::gBotApp;
}
