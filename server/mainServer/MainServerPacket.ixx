// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// MainServerPacket.ixx - MainServer PacketHandler Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "MainServer/PCH/pch.h"

// PacketHandler 헤더들
#include "MainServer/PacketHandler/MainPacketHandlerAuth/MainPacketHandlerAuth.h"
#include "MainServer/PacketHandler/MainPacketHandlerChat/MainPacketHandlerChat.h"
#include "MainServer/PacketHandler/MainPacketHandlerGame/MainPacketHandlerGame.h"
#include "MainServer/PacketHandler/MainPacketHandlerRoom/MainPacketHandlerRoom.h"
#include "MainServer/PacketHandler/MainPacketHandlerShellCommand/MainPacketHandlerShellCommand.h"
#include "MainServer/PacketHandler/MainPacketHandlerSystem/MainPacketHandlerSystem.h"

export module MainServerPacket;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineNetwork;

export
{
	using ::MainPacketHandlerAuth;
	using ::gMainPacketHandlerAuth;
	using ::MainPacketHandlerChat;
	using ::gMainPacketHandlerChat;
	using ::MainPacketHandlerGame;
	using ::gMainPacketHandlerGame;
	using ::MainPacketHandlerRoom;
	using ::gMainPacketHandlerRoom;
	using ::MainPacketHandlerShellCommand;
	using ::gMainPacketHandlerShellCommand;
	using ::MainPacketHandlerSystem;
	using ::gMainPacketHandlerSystem;
}

