// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import GameServer;


bool SocketGameFromFront::OnDispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::FG_CG_BYPASS:
	{
		const FG_CG_BYPASS& bypassPacket = *reinterpret_cast<const FG_CG_BYPASS*>(rp.GetPacketBufPtr());
		NetworkPacket& nestedPacket = *const_cast<NetworkPacket*>(reinterpret_cast<const NetworkPacket*>(bypassPacket.Get_nestedPacket()));
		//nestedPacket.SetHeader(rp.GetCurrPacketHeader());
		DispatchPacket(nestedPacket);
		break;
	}
	default:
	{
		DispatchPacket(rp);
		break;
	}
	}
	return true;
}

void SocketGameFromFront::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case INVALID_PACKET_TYPE:
		break;
	default:
		const GameChannelIndex gameChannelIndex = rp.GetHeader().GetGameChannelIndex();
		if (INVALID_GAME_CHANNEL_INDEX != gameChannelIndex)
		{
			GameChannelPtr gameChannel = gGameChannelManager->GetGameChannel(gameChannelIndex);
			if (gameChannel)
				PacketUtil::DispatchPacketOnWorker(gameChannel, this, &SocketGameFromFront::DispatchPacket_async, rp);
			else
				_DEBUG_RED;
		}
		else if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketGameFromFront::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketGameFromFront::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketGameFromFront::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp) {
			if (DispatchPacketCG(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			if (DispatchPacketFG(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};	

	if (DispatchPacketToHandler(gGamePacketHandlerGame.get(), rp)) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

