// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// BridgeServer.ixx - BridgeServer Module Interface
module;

#pragma warning(disable: 5028)

#include "BridgeServer/PCH/pch.h"

// BridgeServer 앱 헤더
#include "BridgeServer/App/BridgeServerApp.h"

export module BridgeServer;

// ServerEngine modules
export import ServerEngineCore;
export import ServerEngineWorker;
export import ServerEngineNetwork;
export import ServerEngineApp;
export import ServerEngineUtil;
export import ServerEnginePacket;

// BridgeServer split modules
export import BridgeServerSocket;
export import BridgeServerPacket;

export
{
	using ::BridgeServerApp;
	using ::gBridgeServerApp;
}
