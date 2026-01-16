// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;


bool SocketMainFromGame::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketMainFromGame::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::ACK_SHELL_COMMAND:
	case PacketTypes::GM_ACK_GAME_LOAD:
	case PacketTypes::GM_ACK_GAME_CREATE:
	case PacketTypes::GM_ACK_GAME_USER_ENTER:
	case PacketTypes::GM_ACK_GAME_USER_LEAVE:
		OnRemoteAck(rp);
		break;
	case PacketTypes::GM_GAME_SERVER_READY:
		PacketUtil::DispatchPacketOnWorker(gGameListManager, this, &SocketMainFromGame::DispatchPacket_async, rp);
		break;
	default:
		if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketMainFromGame::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketMainFromGame::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketMainFromGame::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketGM(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gMainPacketHandlerGame.get(), rp)) return;

	if (DispatchPacketGM<MainHostSocket>(mSocket, this, rp) != HandleResult::NOT_EXISTS) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

