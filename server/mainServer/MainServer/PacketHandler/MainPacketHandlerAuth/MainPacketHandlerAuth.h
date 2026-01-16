// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketMainFromFront;

class MainPacketHandlerAuth final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketMainFromFront& socket) { return HandleResult::NOT_EXISTS; }

public:
	HandleResult OnPacket(class CM_REQ_SERVER_LIST& rp, MAYBE_UNUSED SocketMainFromFront& socket);

	HandleResult OnPacket(class CM_REQ_ACCOUNT_USER_LIST& rp, MAYBE_UNUSED SocketMainFromFront& socket);
	HandleResult OnPacket(class CM_REQ_ACCOUNT_USER_CREATE& rp, MAYBE_UNUSED SocketMainFromFront& socket);
	HandleResult OnPacket(class CM_REQ_ACCOUNT_USER_DELETE& rp, MAYBE_UNUSED SocketMainFromFront& socket);

	HandleResult OnPacket(class CM_REQ_AUTH_TICKET& rp, MAYBE_UNUSED SocketMainFromFront& socket);
	
	HandleResult OnPacket(class CM_REQ_AUTH_MODIFY& rp, MAYBE_UNUSED SocketMainFromFront& socket);
};

inline std::shared_ptr<MainPacketHandlerAuth> gMainPacketHandlerAuth = nullptr;
