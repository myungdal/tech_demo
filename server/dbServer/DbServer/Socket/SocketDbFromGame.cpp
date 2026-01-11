// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

void SocketDbFromGame::OnHandshakeCompleted(MAYBE_UNUSED bool reconnected)
{

}

void SocketDbFromGame::OnLostSocket()
{
	
}

bool SocketDbFromGame::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketDbFromGame::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::GD_ACK_ITEM_USE:
	case PacketTypes::GD_ACK_GAME_USER_DATA_LOAD:
		OnRemoteAck(rp);
		break;
	default:
		const UserId userId = rp.GetHeader().GetUserId();
		if (INVALID_UUID != userId)
		{
			if (DbUserPtr dbUser = gDbUserManager->FindUser(userId))
				PacketUtil::DispatchPacketOnWorker(dbUser, this, &SocketDbFromGame::DispatchPacket_async, rp);
			else // DB서버가 재시작했을 경우일 것.
				_DEBUG_RED;
		}
		else if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketDbFromGame::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketDbFromGame::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketDbFromGame::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketGD(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gDbPacketHandlerStaticData.get(), rp)) return;
	if (DispatchPacketToHandler(gDbPacketHandlerGame.get(), rp)) return;
	if (DispatchPacketToHandler(gDbPacketHandlerItem.get(), rp)) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

