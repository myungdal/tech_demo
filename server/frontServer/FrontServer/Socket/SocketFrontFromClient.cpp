// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;
import ServerEngineCommon;

// [WHY] FrontSocketUtil은 import FrontServer를 통해 FrontServerSocket 모듈에서 이미 export됨


SocketFrontFromClient::SocketFrontFromClient()
{
	UsageMeter<SocketFrontFromClient, SocketFrontFromClient>::OnInit(0);
}

void SocketFrontFromClient::OnSizeSend(DWORD bytes)
{
	UsageMeter<SocketFrontFromClient, SocketFrontFromClient>::OnDecrease(bytes);
}

void SocketFrontFromClient::OnSizeRecv(DWORD bytes)
{
	UsageMeter<SocketFrontFromClient, SocketFrontFromClient>::OnIncrease(bytes);
}

void SocketFrontFromClient::OnEncrypt(uint8_t* buf, int32_t len, EncryptionKey encryptionKey)
{
	NetworkUtil::Encrypt(buf, len, encryptionKey);
}

void SocketFrontFromClient::OnHandshakeCompleted(MAYBE_UNUSED bool reconnected)
{
	mSyncTimeTryCnt = 0;
	mFrontUser.reset();
}

void SocketFrontFromClient::OnLostSocket()
{
	SocketPtrType ptr(this, L"OnLostSocket");
	WorkerManager::Run(gFrontDataManager, this, &SocketFrontFromClient::OnLostSocket_async, ptr);
}

void SocketFrontFromClient::OnLostSocket_async(MAYBE_UNUSED SocketPtrType& ptr)
{
	FrontUserPtr frontUser = GetFrontUser();
	if (nullptr == frontUser)
		return;

	const PacketHeader& packetHeader = frontUser->GetCurrPacketHeader();
	if (false == gFrontDataManager->DeleteUser(packetHeader.GetUserId()))
		return;

	if (auto dbSocket = FrontSocketUtil::GetDbSocket(packetHeader.GetAppId(AppType::DB_SERVER)))
	{
		SocketUtil::Send<FD_USER_LOGOUT::Writer> wp(**dbSocket, NOTIFY);
		wp.SetHeader(packetHeader);
	}
}

bool SocketFrontFromClient::OnDispatchPacket(NetworkPacket& rp)
{
	// CM Bypass 처리
	if ((PacketTypes::CM_PACKET_START < rp.GetPacketType()) &&
		(PacketTypes::CM_PACKET_END > rp.GetPacketType()))
	{
		OnDispatchPacketBypassCM(rp);
		return true;
	}

	// CD Bypass 처리
	if ((PacketTypes::CD_PACKET_START < rp.GetPacketType()) &&
		(PacketTypes::CD_PACKET_END > rp.GetPacketType()))
	{
		OnDispatchPacketBypassCD(rp);
		return true;
	}

	// CB Bypass 처리
	if ((PacketTypes::CB_PACKET_START < rp.GetPacketType()) &&
		(PacketTypes::CB_PACKET_END > rp.GetPacketType()))
	{
		OnDispatchPacketBypassCB(rp);
		return true;
	}

	// CG Bypass 처리
	if ((PacketTypes::CG_PACKET_START < rp.GetPacketType()) &&
		(PacketTypes::CG_PACKET_END > rp.GetPacketType()))
	{
		OnDispatchPacketBypassCG(rp);
		return true;
	}

	DispatchPacket(rp);
	return true;
}

void SocketFrontFromClient::OnDispatchPacketBypassCM(NetworkPacket& rp)
{
	SocketPtr<SocketFrontToMain> mainSocket = FrontSocketUtil::GetMainSocketPtr();
	PacketUtil::ForwardBypassPacketReq<FM_REQ_CM_BYPASS::Writer>(*this, mainSocket, rp);
}

void SocketFrontFromClient::OnDispatchPacketBypassCD(NetworkPacket& rp)
{
	SocketPtr<SocketFrontToDb> dbSocket;
	if (FrontUserPtr frontUser = GetFrontUser())
	{
		if (frontUser->GetCurrPacketHeader().ValidateState(rp.GetHeader()))
		{
			const AppId dbAppId = rp.GetHeader().GetAppId(AppType::DB_SERVER);
			dbSocket = FrontSocketUtil::GetDbSocket(dbAppId);
			PacketUtil::ForwardBypassPacketReq<FD_REQ_CD_BYPASS::Writer>(*this, dbSocket, rp);
		}
	}
}

void SocketFrontFromClient::OnDispatchPacketBypassCB(NetworkPacket& rp)
{
	SocketPtr<SocketFrontToBridge> bridgeSocket = FrontSocketUtil::PickBridgeSocketPtr();
	PacketUtil::ForwardBypassPacketReq<FB_REQ_CB_BYPASS::Writer>(*this, bridgeSocket, rp);
}

void SocketFrontFromClient::OnDispatchPacketBypassCG(NetworkPacket& rp)
{
	SocketPtr<SocketFrontToGame> gameSocket;
	if (FrontUserPtr frontUser = GetFrontUser())
	{
		// [TODO] 헤더 상태 검증을 복구해야 한다. (재전송/순서 역전/세션 탈취 등 방지)
		// 현재는 우회 중이므로 잘못된 헤더 상태의 패킷도 전달될 수 있다.
		//if (frontUser->GetCurrPacketHeader().ValidateState(rp.GetHeader()))
		{
			const AppId gameAppId = rp.GetHeader().GetAppId(AppType::GAME_SERVER);
			gameSocket = FrontSocketUtil::GetGameSocket(gameAppId);
			PacketUtil::ForwardBypassPacketNormal<FG_CG_BYPASS::Writer>(*this, gameSocket, rp);
		}
	}
}

void SocketFrontFromClient::DispatchPacket(NetworkPacket& rp)
{
	if (tThreadId == INVALID_THREAD_ID)
		PacketUtil::DispatchPacketOnAnyThread(this, &SocketFrontFromClient::DispatchPacket_async, rp);
	else
		DispatchPacketToHandler(rp);
}

void SocketFrontFromClient::DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp)
{
	DispatchPacketToHandler(**tp);
}

void SocketFrontFromClient::DispatchPacketToHandler(NetworkPacket& rp)
{
	auto DispatchPacketToHandler = [this](auto handler, NetworkPacket& rp) 
		{
			if (DispatchPacketCF(mSocket, handler, rp, *this) != HandleResult::NOT_EXISTS) return true;
			return false;
		};

	if (DispatchPacketToHandler(gFrontPacketHandlerChat.get(), rp)) return;
	if (DispatchPacketToHandler(gFrontPacketHandlerLogin.get(), rp)) return;
	if (DispatchPacketToHandler(gFrontPacketHandlerGame.get(), rp)) return;
	if (DispatchPacketToHandler(gFrontPacketHandlerSystem.get(), rp)) return;

	_DEBUG_LOG(RED, L"{}", GetPacketTypeString(rp.GetPacketType()));
}

