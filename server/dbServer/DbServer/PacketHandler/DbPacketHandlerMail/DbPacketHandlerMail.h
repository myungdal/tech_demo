// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketDbFromFront;

class DbPacketHandlerMail final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketDbFromFront& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class CD_REQ_MAIL_LIST& rp, MAYBE_UNUSED SocketDbFromFront& socket);
	HandleResult OnPacket(class CD_REQ_MAIL_READ& rp, MAYBE_UNUSED SocketDbFromFront& socket);
};

inline std::shared_ptr<DbPacketHandlerMail> gDbPacketHandlerMail = nullptr;
