// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class UserDbSession;
class SocketDbFromFront;

class DbPacketHandlerCheat final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketDbFromFront& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class CD_REQ_CHEAT& rp, MAYBE_UNUSED SocketDbFromFront& socket);
};

inline std::shared_ptr<DbPacketHandlerCheat> gDbPacketHandlerCheat = nullptr;
