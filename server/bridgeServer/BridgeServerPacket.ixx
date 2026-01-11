// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// BridgeServerPacket.ixx - BridgeServer PacketHandler Module Interface
module;

#pragma warning(disable: 5028)

#include "BridgeServer/PCH/pch.h"

#include "BridgeServer/PacketHandler/BridgePacketHandlerAuth/BridgePacketHandlerAuth.h"

export module BridgeServerPacket;

export import ServerEngineCore;
export import ServerEngineNetwork;
export
{
	using ::BridgePacketHandlerAuth;
	using ::gBridgePacketHandlerAuth;
}
