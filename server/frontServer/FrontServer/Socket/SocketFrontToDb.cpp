// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;

void SocketFrontToDb::OnHandshakeCompleted(MAYBE_UNUSED bool reconnected)
{
	_DEBUG_LOG(WHITE, L"[{}]", mSocket);
}

bool SocketFrontToDb::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketFrontToDb::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::DF_ACK_USER_LOGIN:
	case PacketTypes::DF_USER_DISCONNECT:
		PacketUtil::DispatchPacketOnWorker(gFrontDataManager, this, &SocketFrontToDb::DispatchPacket_async, rp);
		break;
	default:
		if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketFrontToDb::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketFrontToDb::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketFrontToDb::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketDC(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			if (DispatchPacketDF(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gFrontPacketHandlerLogin.get(), rp)) return;
	if (DispatchPacketToHandler(gFrontPacketHandlerSystem.get(), rp)) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

