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

class DbPacketHandlerItem final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketDbFromFront& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketDbFromGame& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class CD_REQ_ITEM_USE& rp, MAYBE_UNUSED SocketDbFromFront& socket);
	HandleResult OnPacket(class GD_GAME_USER_ITEM_UPDATE& rp, MAYBE_UNUSED SocketDbFromGame& socket);
};

inline std::shared_ptr<DbPacketHandlerItem> gDbPacketHandlerItem = nullptr;
