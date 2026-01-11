// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;

bool SocketMainFromDb::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketMainFromDb::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::ACK_SHELL_COMMAND:
	case PacketTypes::DM_ACK_AUTH_TICKET:
		OnRemoteAck(rp);
		break;
	case PacketTypes::DM_REQ_GAME_CREATE:
	case PacketTypes::DM_REQ_GAME_USER_ENTER:
	case PacketTypes::DM_REQ_GAME_USER_LEAVE:	
	case PacketTypes::DM_REQ_GAME_FINISH:
		PacketUtil::DispatchPacketOnWorker(gGameListManager, this, &SocketMainFromDb::DispatchPacket_async, rp);
		break;
	default:
		if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketMainFromDb::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketMainFromDb::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketMainFromDb::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketDM(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gMainPacketHandlerGame.get(), rp)) return;
	if (DispatchPacketToHandler(gMainPacketHandlerShellCommand.get(), rp)) return;

	if (DispatchPacketDM<MainHostSocket>(mSocket, this, rp) != HandleResult::NOT_EXISTS) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

