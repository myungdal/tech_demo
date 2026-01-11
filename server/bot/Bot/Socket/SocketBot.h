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


// 봇용 기본 소켓 클래스
class SocketBot : public PeerSocket
{
protected:
	BotPtr mBot = nullptr;

public:
	void SetBot(BotPtr bot) noexcept { mBot = bot; }

protected:
	void OnEncrypt(uint8_t* buf, int32_t len, EncryptionKey encryptionKey) final;
};
