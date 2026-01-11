// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketDbToMain;

class DbPacketHandlerAuth final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketDbToMain& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class MD_REQ_AUTH_TICKET& rp, MAYBE_UNUSED SocketDbToMain & socket);
};

inline std::shared_ptr<DbPacketHandlerAuth> gDbPacketHandlerAuth = nullptr;
