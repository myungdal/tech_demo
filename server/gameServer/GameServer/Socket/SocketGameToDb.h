// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/PeerSocket.h"
#include "ServerEngine/Socket/Socket.h"


class SocketGameFromFront;

class SocketGameToDb final : public Socket<SocketGameToDb, MAX_POOL_SIZE_FACTOR_SOCKET_GAME_TO_DB, PeerSocket>
{
protected:
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) final;

protected:
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};
