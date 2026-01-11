// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// BridgeServerSocket.ixx - BridgeServer Socket Module Interface
module;

#pragma warning(disable: 5028)

#include "BridgeServer/PCH/pch.h"

#include "BridgeServer/Socket/SocketBridgeFromFront.h"
#include "BridgeServer/Socket/SocketBridgeToMain.h"

export module BridgeServerSocket;

export import ServerEngineCore;
export import ServerEngineNetwork;
export
{
	using ::SocketBridgeFromFront;
	using ::SocketBridgeToMain;
	using ::gSocketBridgeToMain;
}
