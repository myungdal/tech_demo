// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketFrontFromClient;
class SocketFrontToGame;

// 게임 채널 관련 패킷 핸들러
class FrontPacketHandlerGame final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontFromClient& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontToGame& socket) { return HandleResult::NOT_EXISTS; }

	// 게임 채널 입장 요청
	HandleResult OnPacket(class CF_REQ_GAME_CHANNEL_USER_ENTER& rp, MAYBE_UNUSED SocketFrontFromClient& socket);

	// 게임 서버로부터 입장 응답
	HandleResult OnPacket(class GF_ACK_GAME_CHANNEL_USER_ENTER& rp, MAYBE_UNUSED SocketFrontToGame& socket);

	// 게임 서버로부터 퇴장 알림
	HandleResult OnPacket(class GF_GAME_CHANNEL_USER_LEAVE& rp, MAYBE_UNUSED SocketFrontToGame& socket);
};

inline std::shared_ptr<FrontPacketHandlerGame> gFrontPacketHandlerGame = nullptr;
