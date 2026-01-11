// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// GameServer.ixx - GameServer Module Interface
module;

#pragma warning(disable: 5028)

#include "GameServer/PCH/pch.h"

// GameServer 앱 헤더
#include "GameServer/App/GameServerApp.h"
#include "GameServer/GameChannelManager/GameChannelManager.h"
#include "GameServer/GameChannelManager/GameChannel.h"
#include "GameServer/GameChannelManager/EntityFactory.h"

export module GameServer;

// ServerEngine 모듈
export import ServerEngineCore;
export import ServerEngineWorker;
export import ServerEngineNetwork;
export import ServerEngineApp;
export import ServerEngineUtil;
export import ServerEnginePacket;

// GameServer 분할 모듈
export import GameServerSocket;
export import GameServerPacket;
export import GameServerChannel;

export
{
	using ::GameServerApp;
	using ::gGameServerApp;
}
