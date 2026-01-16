// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class SocketBridgeFromFront;

// 인증 관련 패킷 핸들러
// [WHY] 외부 인증 서비스와의 연동 처리 (OAuth, 플랫폼 인증 등)
class BridgePacketHandlerAuth final
{
public:
	HandleResult OnPacket(MAYBE_UNUSED class NetworkPacket& rp, MAYBE_UNUSED SocketBridgeFromFront& socket) { return HandleResult::NOT_EXISTS; }
};

inline std::shared_ptr<BridgePacketHandlerAuth> gBridgePacketHandlerAuth = nullptr;
