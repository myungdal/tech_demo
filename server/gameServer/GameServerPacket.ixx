// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// GameServerPacket.ixx - GameServer PacketHandler Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "GameServer/PCH/pch.h"

// PacketHandler 헤더들
#include "GameServer/PacketHandler/GamePacketHandlerGame/GamePacketHandlerGame.h"
#include "GameServer/PacketHandler/GamePacketHandlerStaticData/GamePacketHandlerStaticData.h"
#include "GameServer/PacketHandler/GamePacketHandlerSystem/GamePacketHandlerSystem.h"

export module GameServerPacket;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineNetwork;

export
{
	using ::GamePacketHandlerGame;
	using ::gGamePacketHandlerGame;
	using ::GamePacketHandlerStaticData;
	using ::gGamePacketHandlerStaticData;
	using ::GamePacketHandlerSystem;
	using ::gGamePacketHandlerSystem;
}

