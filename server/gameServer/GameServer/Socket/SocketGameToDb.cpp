// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import GameServer;


void SocketGameToDb::OnHandshakeCompleted(MAYBE_UNUSED bool reconnected)
{
	_DEBUG_LOG(WHITE, L"[{}]", mSocket);

	// 첫 실행은 직접하지만, 실패하면 타이머를 돌려야 해서 호출 형식이 어색하다, 문제는 아님.
	SocketPtrType ptr(this, L"SendReqStaticDataChecksum_timer");
	gGamePacketHandlerStaticData->SendReqStaticDataChecksum_timer(ptr);
}

bool SocketGameToDb::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketGameToDb::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{	
	case INVALID_PACKET_TYPE:
		break;
	default:
		const GameChannelIndex gameChannelIndex = rp.GetHeader().GetGameChannelIndex();
		if (INVALID_GAME_CHANNEL_INDEX != gameChannelIndex)
		{
			if (GameChannelPtr gameChannel = gGameChannelManager->GetGameChannel(gameChannelIndex))
				PacketUtil::DispatchPacketOnWorker(gameChannel, this, &SocketGameToDb::DispatchPacket_async, rp);
			else
				_DEBUG_RED;
		}
		else if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketGameToDb::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketGameToDb::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketGameToDb::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketDG(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gGamePacketHandlerGame.get(), rp)) return;
	if (DispatchPacketToHandler(gGamePacketHandlerStaticData.get(), rp)) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

