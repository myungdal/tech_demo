// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// LogServerPacket.ixx - LogServer PacketHandler Module Interface
module;

#pragma warning(disable: 5028)

#include "LogServer/PCH/pch.h"

#include "LogServer/PacketHandler/LogPacketHandler/LogPacketHandler.h"
#include "LogServer/PacketHandler/LogPacketWorker/LogPacketWorker.h"

export module LogServerPacket;

export import ServerEngineCore;
export import ServerEngineNetwork;
export
{
	using ::LogPacketHandler;
	using ::gLogPacketHandler;
	using ::LogPacketWorker;
	// gLogPacketWorker - not a global singleton
}
