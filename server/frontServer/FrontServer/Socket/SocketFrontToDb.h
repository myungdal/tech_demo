// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/PeerSocket.h"
#include "ServerEngine/Socket/BypassSocket.h"


class SocketFrontFromClient;

// DB 서버로 연결되는 소켓
class SocketFrontToDb final : public BypassSocket<SocketFrontFromClient, SocketFrontToDb, MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_DB, PeerSocket>
{
protected:
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) final;

	// 패킷 디스패치
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};
