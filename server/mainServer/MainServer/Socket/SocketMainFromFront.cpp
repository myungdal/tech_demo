// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;


void SocketMainFromFront::OnHandshakeCompleted(MAYBE_UNUSED bool reconnected)
{
	_TODO("Front 연결/재연결 시 RoomManager 상태 동기화(대기열/룸 세션 정리 또는 복구) 필요");
	__super::OnHandshakeCompleted(reconnected);
}

void SocketMainFromFront::OnLostSocket()
{
	_TODO("Front 연결 끊김 시 RoomManager에 연결 상실 이벤트 전달(세션/룸 정리, 재시도 정책) 필요");
	__super::OnLostSocket();
}

bool SocketMainFromFront::OnDispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::FM_REQ_CM_BYPASS:
		PacketUtil::DispatchNestedPacket<FM_REQ_CM_BYPASS>(rp, this, &SocketMainFromFront::DispatchPacket);
		break;
	default:
		DispatchPacket(rp);
		break;
	}

	return true;
}

void SocketMainFromFront::DispatchPacket(NetworkPacket& rp)
{
	switch (rp.GetPacketType())
	{
	case PacketTypes::ACK_SHELL_COMMAND:
		OnRemoteAck(rp);
		break;
	case PacketTypes::CM_SHELL_COMMAND:
		// 요청자에게 RemoteReq를 걸 경우 패킷 TryRecv를 걸기 위하여 비동기로 처리해야 한다
		PacketUtil::DispatchPacketOnAnyThread(this, &SocketMainFromFront::DispatchPacket_async, rp);
		break;
	case PacketTypes::CM_REQ_ACCOUNT_USER_LIST:
	case PacketTypes::CM_REQ_ACCOUNT_USER_CREATE:
	case PacketTypes::CM_REQ_ACCOUNT_USER_DELETE:
	case PacketTypes::CM_REQ_AUTH_TICKET:
	case PacketTypes::CM_REQ_AUTH_MODIFY:
		PacketUtil::DispatchPacketOnWorker(gAccountManager, this, &SocketMainFromFront::DispatchPacket_async, rp);
		break;
	case PacketTypes::FM_ROOM_USER_ENTER:
	case PacketTypes::FM_ROOM_USER_LEAVE:
		PacketUtil::DispatchPacketOnWorker(gRoomListManager, this, &SocketMainFromFront::DispatchPacket_async, rp);
		break;
	case PacketTypes::CM_REQ_GAME_LIST_OVER:
	case PacketTypes::CM_REQ_GAME_LIST_UNDER:
		PacketUtil::DispatchPacketOnWorker(gGameListManager, this, &SocketMainFromFront::DispatchPacket_async, rp);
		break;
	case PacketTypes::CM_REQ_SERVER_LIST:
		PacketUtil::DispatchPacketOnWorker(gServerListManager, this, &SocketMainFromFront::DispatchPacket_async, rp);
		break;
	default:
		if (tThreadId == INVALID_THREAD_ID)
			PacketUtil::DispatchPacketOnAnyThread(this, &SocketMainFromFront::DispatchPacket_async, rp);
		else
			DispatchPacketToHandler(rp);
		break;	
	}
}

void SocketMainFromFront::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketMainFromFront::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp)
		{
			if (DispatchPacketCM(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			if (DispatchPacketFM(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gMainPacketHandlerAuth.get(), rp)) return;
	if (DispatchPacketToHandler(gMainPacketHandlerRoom.get(), rp)) return;
	if (DispatchPacketToHandler(gMainPacketHandlerGame.get(), rp)) return;
	if (DispatchPacketToHandler(gMainPacketHandlerShellCommand.get(), rp)) return;
	if (DispatchPacketToHandler(gMainPacketHandlerSystem.get(), rp)) return;

	if (DispatchPacketFM<MainHostSocket>(mSocket, this, rp) != HandleResult::NOT_EXISTS) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

