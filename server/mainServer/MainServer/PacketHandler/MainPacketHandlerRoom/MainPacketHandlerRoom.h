// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketMainFromFront;

class MainPacketHandlerRoom final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromFront& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class FM_ROOM_USER_ENTER& rp, MAYBE_UNUSED SocketMainFromFront& socket);
	HandleResult OnPacket(class FM_ROOM_USER_LEAVE& rp, MAYBE_UNUSED SocketMainFromFront& socket);
};

inline std::shared_ptr<MainPacketHandlerRoom> gMainPacketHandlerRoom = nullptr;
