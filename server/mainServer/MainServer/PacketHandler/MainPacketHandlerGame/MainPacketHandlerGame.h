// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketMainFromGame;
class SocketMainFromFront;
class SocketMainFromDb;

class MainPacketHandlerGame final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromGame& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromFront& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromDb& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(MAYBE_UNUSED class GM_GAME_SERVER_READY& rp, MAYBE_UNUSED SocketMainFromGame& socket);
	HandleResult OnPacket(class CM_REQ_GAME_LIST_OVER& rp, MAYBE_UNUSED SocketMainFromFront& socket);
	HandleResult OnPacket(class CM_REQ_GAME_LIST_UNDER& rp, MAYBE_UNUSED SocketMainFromFront& socket);
	HandleResult OnPacket(class DM_REQ_GAME_CREATE& rp, MAYBE_UNUSED SocketMainFromDb& socket);
	HandleResult OnPacket(class DM_REQ_GAME_USER_ENTER& rp, MAYBE_UNUSED SocketMainFromDb& socket);
	HandleResult OnPacket(class DM_REQ_GAME_USER_LEAVE& rp, MAYBE_UNUSED SocketMainFromDb& socket);
	HandleResult OnPacket(class DM_REQ_GAME_FINISH& rp, MAYBE_UNUSED SocketMainFromDb& socket);
};

inline std::shared_ptr<MainPacketHandlerGame> gMainPacketHandlerGame = nullptr;
