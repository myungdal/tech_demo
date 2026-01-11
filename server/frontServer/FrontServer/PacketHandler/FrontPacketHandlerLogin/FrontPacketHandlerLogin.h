// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketFrontFromClient;
class SocketFrontToDb;

// 로그인 관련 패킷 핸들러
class FrontPacketHandlerLogin final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontFromClient& socket) { return HandleResult::NOT_EXISTS; }
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketFrontToDb& socket) { return HandleResult::NOT_EXISTS; }

	// 글로벌 시간 동기화 요청
	HandleResult OnPacket(class REQ_GLOBAL_NOW& rp, MAYBE_UNUSED SocketFrontFromClient& socket);

	// 패킷 목록 요청 (디버그용)
	HandleResult OnPacket(class CF_REQ_PACKET_LIST& rp, MAYBE_UNUSED SocketFrontFromClient& socket);

	// 로그인 요청/응답
	HandleResult OnPacket(class CF_REQ_USER_LOGIN& rp, MAYBE_UNUSED SocketFrontFromClient& socket);
	HandleResult OnPacket(class DF_ACK_USER_LOGIN& rp, MAYBE_UNUSED SocketFrontToDb& socket);

	// DB 서버로부터 연결 끊김 알림
	HandleResult OnPacket(class DF_USER_DISCONNECT& rp, MAYBE_UNUSED SocketFrontToDb& socket);
};

inline std::shared_ptr<FrontPacketHandlerLogin> gFrontPacketHandlerLogin = nullptr;
