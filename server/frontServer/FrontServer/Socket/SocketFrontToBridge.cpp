// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;

void SocketFrontToBridge::OnHandshakeCompleted(MAYBE_UNUSED bool reconnected)
{
	_DEBUG_LOG(WHITE, L"[{}]", mSocket);
}

bool SocketFrontToBridge::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketFrontToBridge::DispatchPacket(NetworkPacket& rp)
{
	if (tThreadId == INVALID_THREAD_ID)
		PacketUtil::DispatchPacketOnAnyThread(this, &SocketFrontToBridge::DispatchPacket_async, rp);
	else
		DispatchPacketToHandler(rp);
}

void SocketFrontToBridge::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketFrontToBridge::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketBC(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			if (DispatchPacketBF(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

