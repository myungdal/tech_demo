// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;

void SocketBotToFront::OnHandshakeCompleted(MAYBE_UNUSED bool reconnected)
{
	_DEBUG_LOG(WHITE, L"[{}]", mSocket);
	WorkerManager::Run(mBot, mBot.get(), &Bot::OnHandshakeCompletedFront_async, reconnected);
}

void SocketBotToFront::OnDisconnected()
{
	_DEBUG_LOG(WHITE, L"[{}]", mSocket);
	WorkerManager::Run(mBot, mBot.get(), &Bot::OnLostConnectionFront_async);
}

bool SocketBotToFront::OnDispatchPacket(NetworkPacket& rp)
{
	PacketTemp tp(rp);

	// 사라질 일이 없기 때문에 AddRef 생략.
	WorkerManager::Run(mBot, mBot.get(), &Bot::OnDispatchPacketFront_async, tp);

	return true;
}

