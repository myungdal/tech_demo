// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// FrontServerPacket.ixx - FrontServer PacketHandler Module Interface
module;

#pragma warning(disable: 5028)

#include "FrontServer/PCH/pch.h"

// PacketHandler headers
#include "FrontServer/PacketHandler/FrontPacketHandlerChat/FrontPacketHandlerChat.h"
#include "FrontServer/PacketHandler/FrontPacketHandlerGame/FrontPacketHandlerGame.h"
#include "FrontServer/PacketHandler/FrontPacketHandlerLogin/FrontPacketHandlerLogin.h"
#include "FrontServer/PacketHandler/FrontPacketHandlerSystem/FrontPacketHandlerSystem.h"

export module FrontServerPacket;

export import ServerEngineCore;
export import ServerEngineNetwork;
export
{
	using ::FrontPacketHandlerChat;
	using ::gFrontPacketHandlerChat;
	using ::FrontPacketHandlerGame;
	using ::gFrontPacketHandlerGame;
	using ::FrontPacketHandlerLogin;
	using ::gFrontPacketHandlerLogin;
	using ::FrontPacketHandlerSystem;
	using ::gFrontPacketHandlerSystem;
}
