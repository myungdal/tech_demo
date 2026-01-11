// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Bot/Socket/SocketBot.h"


// 봇이 FrontServer와 통신하는 소켓
class SocketBotToFront final : public Socket<SocketBotToFront, MAX_POOL_SIZE_FACTOR_SOCKET_BOT_TO_FRONT, SocketBot>
{
protected:
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) final;
	void OnDisconnected() final;
	bool OnDispatchPacket(NetworkPacket& rp) final;
};

using SocketBotToFrontWeakPtr = SocketWeakPtr<SocketBotToFront>;
