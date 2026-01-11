// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Shell;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SocketShellToMain::OnAppAdded(IN OUT MAYBE_UNUSED AppInfo& appInfo)
{
}

bool SocketShellToMain::OnDispatchPacket(NetworkPacket& rp)
{
	DispatchPacket(rp);
	return true;
}

void SocketShellToMain::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::ACK_SHELL_COMMAND:
		OnRemoteAck(rp);
		break;

	default:
		if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketShellToMain::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;
	}
}

void SocketShellToMain::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketShellToMain::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketMS(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gShellPacketHandler.get(), rp)) return;

	if (DispatchPacketMS<MainPeerSocket>(mSocket, this, rp) != HandleResult::NOT_EXISTS) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}
