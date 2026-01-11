// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

bool SocketDbFromFront::OnDispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::FD_REQ_CD_BYPASS:
		PacketUtil::DispatchNestedPacket<FD_REQ_CD_BYPASS>(rp, this, &SocketDbFromFront::DispatchPacket);
		break;
	default:
		DispatchPacket(rp);
		break;
	}
	return true;
}

void SocketDbFromFront::DispatchPacket(NetworkPacket& rp)
{
	const UserId userId = rp.GetHeader().GetUserId();
	if (INVALID_UUID != userId)
	{
		if (DbUserPtr dbUser = gDbUserManager->FindUser(userId))
			PacketUtil::DispatchPacketOnWorker(dbUser, this, &SocketDbFromFront::DispatchPacket_async, rp);
		else // DB서버가 재시작했을 경우일 것.
			_DEBUG_RED;
	}
	else if (tThreadId == INVALID_THREAD_ID)
		PacketUtil::DispatchPacketOnAnyThread(this, &SocketDbFromFront::DispatchPacket_async, rp);
	else
		DispatchPacketToHandler(rp);
}

void SocketDbFromFront::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketDbFromFront::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketCD(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			if (DispatchPacketFD(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gDbPacketHandlerCheat.get(), rp)) return;
	if (DispatchPacketToHandler(gDbPacketHandlerLogin.get(), rp)) return;
	if (DispatchPacketToHandler(gDbPacketHandlerStaticData.get(), rp)) return;
	if (DispatchPacketToHandler(gDbPacketHandlerUser.get(), rp)) return;
	if (DispatchPacketToHandler(gDbPacketHandlerGame.get(), rp)) return;
	if (DispatchPacketToHandler(gDbPacketHandlerMail.get(), rp)) return;
	if (DispatchPacketToHandler(gDbPacketHandlerItem.get(), rp)) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

