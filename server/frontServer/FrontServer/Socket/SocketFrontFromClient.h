// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/HostSocket.h"
#include "ServerEngine/Socket/Socket.h"


class SocketFrontFromClient final : public Socket<SocketFrontFromClient, MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_FROM_CLIENT, HostSocket>
{
private:
	static constexpr size_t MAX_SEND_BUFFER_QUEUE_SIZE = 64;
	static constexpr int32_t MAX_SYNC_TIME_TRY_CNT = 5;

	int32_t mSyncTimeTryCnt = 0;
	FrontUserWeakPtr mFrontUser;

public:
	SocketFrontFromClient();

	// 시간 동기화 시도 횟수 증가. 최대치 초과 시 false 반환
	bool AddSyncTimeTryCnt() { return (++mSyncTimeTryCnt) <= MAX_SYNC_TIME_TRY_CNT; }
	FrontUserPtr GetFrontUser() const { return mFrontUser.lock(); }
	void SetFrontUser(const FrontUserPtr& frontUser) { mFrontUser = frontUser; }

protected:
	size_t GetMaxSendBufferQueueSize() const final { return MAX_SEND_BUFFER_QUEUE_SIZE; }

	// SocketBase 오버라이드
	void OnSizeSend(DWORD bytes) final;
	void OnSizeRecv(DWORD bytes) final;
	void OnEncrypt(uint8_t* buf, int32_t len, EncryptionKey encryptionKey) final;
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) final;
	void OnLostSocket() final;	
	void OnLostSocket_async(MAYBE_UNUSED SocketPtrType& ptr);

	// 패킷 디스패치
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void OnDispatchPacketBypassCM(NetworkPacket& rp);
	void OnDispatchPacketBypassCD(NetworkPacket& rp);
	void OnDispatchPacketBypassCB(NetworkPacket& rp);
	void OnDispatchPacketBypassCG(NetworkPacket& rp);
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};
