// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbServerPacket.ixx - DbServer PacketHandler Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "DbServer/PCH/pch.h"

// PacketHandler 헤더들
#include "DbServer/PacketHandler/DbPacketHandlerAuth/DbPacketHandlerAuth.h"
#include "DbServer/PacketHandler/DbPacketHandlerCheat/DbPacketHandlerCheat.h"
#include "DbServer/PacketHandler/DbPacketHandlerGame/DbPacketHandlerGame.h"
#include "DbServer/PacketHandler/DbPacketHandlerItem/DbPacketHandlerItem.h"
#include "DbServer/PacketHandler/DbPacketHandlerLogin/DbPacketHandlerLogin.h"
#include "DbServer/PacketHandler/DbPacketHandlerMail/DbPacketHandlerMail.h"
#include "DbServer/PacketHandler/DbPacketHandlerStaticData/DbPacketHandlerStaticData.h"
#include "DbServer/PacketHandler/DbPacketHandlerUser/DbPacketHandlerUser.h"

// PacketTransactor 헤더
#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"

export module DbServerPacket;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineNetwork;

export
{
	using ::DbPacketHandlerAuth;
	using ::gDbPacketHandlerAuth;
	using ::DbPacketHandlerCheat;
	using ::gDbPacketHandlerCheat;
	using ::DbPacketHandlerGame;
	using ::gDbPacketHandlerGame;
	using ::DbPacketHandlerItem;
	using ::gDbPacketHandlerItem;
	using ::DbPacketHandlerLogin;
	using ::gDbPacketHandlerLogin;
	using ::DbPacketHandlerMail;
	using ::gDbPacketHandlerMail;
	using ::DbPacketHandlerStaticData;
	using ::gDbPacketHandlerStaticData;
	using ::DbPacketHandlerUser;
	using ::gDbPacketHandlerUser;
	using ::UserAndPacketTransactor;
}

