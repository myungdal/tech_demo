// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/MainHostSocket.h"
#include "ServerEngine/Socket/RequestSocket.h"


class SocketMainFromFront final : public RequestSocket<SocketMainFromFront, MAX_POOL_SIZE_FACTOR_SOCKET_MAIN_FROM_FRONT, MainHostSocket>
{
protected:
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) override;
	void OnLostSocket() override;

protected:
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};
