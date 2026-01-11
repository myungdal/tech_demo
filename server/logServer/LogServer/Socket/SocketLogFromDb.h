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


class SocketLogFromDb final : public Socket<SocketLogFromDb, MAX_POOL_SIZE_FACTOR_SOCKET_LOG_FROM_DB, HostSocket>
{
protected:
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp, LogPacketWorkerPtr worker);
	void DispatchPacketToHandler(NetworkPacket& rp, LogPacketWorkerPtr worker);
};
