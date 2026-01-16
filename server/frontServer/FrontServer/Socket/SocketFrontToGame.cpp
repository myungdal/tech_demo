// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;

bool SocketFrontToGame::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketFrontToGame::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::GF_ACK_GAME_CHANNEL_USER_ENTER:
	case PacketTypes::GF_GAME_CHANNEL_USER_LEAVE:
		PacketUtil::DispatchPacketOnWorker(gFrontDataManager, this, &SocketFrontToGame::DispatchPacket_async, rp);
		break;
	default:
		if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketFrontToGame::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketFrontToGame::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketFrontToGame::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketGC(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			if (DispatchPacketGF(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gFrontPacketHandlerGame.get(), rp)) return;
	if (DispatchPacketToHandler(gFrontPacketHandlerSystem.get(), rp)) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

