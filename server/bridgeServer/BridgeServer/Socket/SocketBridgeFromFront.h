// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/HostSocket.h"
#include "ServerEngine/Socket/Socket.h"


// FrontServer로부터 연결을 받는 소켓
// [WHY] BridgeServer는 FrontServer로부터 외부 연동 요청을 수신하여 처리함
class SocketBridgeFromFront final : public Socket<SocketBridgeFromFront, MAX_POOL_SIZE_FACTOR_SOCKET_BRIDGE_FROM_FRONT, HostSocket>
{
protected:
	// 패킷 디스패치 (메인 진입점)
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};
