// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketFrontToMain;
class SocketFrontToDb;
class SocketFrontToGame;
class SocketFrontFromClient;

// 시스템 패킷 핸들러 (바이패스, 방 입퇴장 등)
class FrontPacketHandlerSystem final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontToMain& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontToDb& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontToGame& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontFromClient& socket) { return HandleResult::NOT_EXISTS; }

	// Main 서버로부터 바이패스 응답
	HandleResult OnPacket(class MF_ACK_MC_BYPASS& rp, MAYBE_UNUSED SocketFrontToMain& socket);

	// DB 서버로부터 바이패스 응답
	HandleResult OnPacket(class DF_ACK_DC_BYPASS& rp, MAYBE_UNUSED SocketFrontToDb& socket);

	// Game 서버로부터 바이패스 (특정 유저들)
	HandleResult OnPacket(class GF_GC_BYPASS& rp, MAYBE_UNUSED SocketFrontToGame& socket);

	// Game 서버로부터 바이패스 (게임 내 모든 유저)
	HandleResult OnPacket(class GF_GC_BYPASS_GAME& rp, MAYBE_UNUSED SocketFrontToGame& socket);

	// Main 서버로부터 방 입퇴장 알림
	HandleResult OnPacket(class MF_ROOM_USER_ENTER& rp, MAYBE_UNUSED SocketFrontToMain& socket);
	HandleResult OnPacket(class MF_ROOM_USER_LEAVE& rp, MAYBE_UNUSED SocketFrontToMain& socket);
};

inline std::shared_ptr<FrontPacketHandlerSystem> gFrontPacketHandlerSystem = nullptr;
