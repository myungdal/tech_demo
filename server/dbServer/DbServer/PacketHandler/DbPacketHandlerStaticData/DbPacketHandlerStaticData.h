// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketDbFromFront;
class SocketDbFromGame;

class DbPacketHandlerStaticData final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketDbFromFront& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketDbFromGame& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class CD_REQ_STATIC_DATA_CHECKSUM& rp, MAYBE_UNUSED SocketDbFromFront& socket);
	HandleResult OnPacket(class CD_REQ_STATIC_DATA& rp, MAYBE_UNUSED SocketDbFromFront& socket);

	HandleResult OnPacket(class GD_REQ_STATIC_DATA_CHECKSUM& rp, MAYBE_UNUSED SocketDbFromGame& socket);
	HandleResult OnPacket(class GD_REQ_STATIC_DATA& rp, MAYBE_UNUSED SocketDbFromGame& socket);
};

inline std::shared_ptr<DbPacketHandlerStaticData> gDbPacketHandlerStaticData = nullptr;
