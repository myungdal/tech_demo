// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketFrontFromClient;
class SocketFrontToMain;

// 채팅 관련 패킷 핸들러
class FrontPacketHandlerChat final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontFromClient& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontToMain& socket) { return HandleResult::NOT_EXISTS; }

	// 클라이언트로부터 채팅 요청
	HandleResult OnPacket(class CF_REQ_CHAT& rp, MAYBE_UNUSED SocketFrontFromClient& socket);

	// Main 서버로부터 채팅 브로드캐스트
	HandleResult OnPacket(class MF_CHAT& rp, MAYBE_UNUSED SocketFrontToMain& socket);
};

inline std::shared_ptr<FrontPacketHandlerChat> gFrontPacketHandlerChat = nullptr;
