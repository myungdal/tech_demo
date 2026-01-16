// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketMainFromFront;

class MainPacketHandlerChat final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromFront& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class FM_CHAT& rp, MAYBE_UNUSED SocketMainFromFront& socket);
};

inline std::shared_ptr<MainPacketHandlerChat> gMainPacketHandlerChat = nullptr;
