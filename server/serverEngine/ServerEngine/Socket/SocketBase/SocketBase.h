// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// SocketBase.h - 소켓 기본 클래스
#pragma once

#include "ServerEngine/Network/NetworkManager.h"
#include "ServerEngine/Socket/SocketPtr/SocketRefCounter.h"
#include "ServerEngine/Socket/SocketPtr/SocketWeakPtr.h"
#if _SOCKET_HISTORY_REF_COUNT
#	include <concurrent_queue.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SocketBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Iocp;
class NetworkPacketWriter;
class NetworkPacket;

// 소켓 기본 클래스 (모든 소켓의 베이스)
class SocketBase : public CompletionHandler
{
public:
	// 수신 버퍼 최대 크기 (1MB)
	static constexpr size_t MAX_RECV_BUF_SIZE = (1 << 20);
	
	// 송신 최대 크기 (1KB)
	static constexpr size_t MAX_SEND_SIZE = 1024;
	
	// 패킷 송수신 로그 플래그
	static inline bool PACKET_SEND_LOG = true;
	static inline bool PACKET_RECV_LOG = true;

	// 소켓 상태
	enum class SocketState : uint8_t
	{
		CLOSED = 0,         // 닫힘
		OPENED = 1,         // 열림
		CONNECTING = 2,     // 연결 중
		CONNECTED = 3,      // 연결됨
		DISCONNECTING = 4,  // 연결 해제 중
		DISCONNECTED = 5,   // 연결 해제됨
		CLOSING = 6,        // 닫는 중
		MAX
	};

protected:
	// IOCP 핸들러
	std::shared_ptr<Iocp> mIocp = nullptr;
	
	// 소켓 핸들
	SOCKET mSocket = INVALID_SOCKET;
	
	// 소켓 주소
	SOCKADDR_IN mAddr{};

	// 참조 카운터 (스마트 포인터용)
	template<typename SocketBase> friend class SocketPtr;
	template<typename SocketBase> friend class SocketWeakPtr;
	SocketRefCounterPtr mSocketRefCounterPtr = nullptr;
	
	// 소켓 상태
	std::atomic<SocketState> mSocketState = SocketState::CLOSED;
	
	// 재연결 플래그
	std::atomic<bool> mIsReconnect = false;

	// 수신 버퍼
	uint8_t mRecvBuf[MAX_RECV_BUF_SIZE] = {};
	size_t mRecvBufHead = 0;
	size_t mRecvBufTail = 0;

	// 송신 버퍼 큐 (Lock 기반)
	using SendBufferInfo = std::pair<uint8_t*, PacketSize>;
	Lock mSendBufferQueueLock;
	std::queue<SendBufferInfo> mSendBufferQueue;
	PacketSize mLastSendBufferOffset = 0;

	// 소켓 이름
	const char* mSocketNameA = "";
	std::wstring mSocketNameW;

	// 부모 소켓 (Accept된 소켓의 경우)
	SocketBase* mParentSocket = nullptr;

	// Backlog 설정 및 현재값
	int32_t mMaxBacklog = 0;
	std::atomic<int32_t> mCurrentBacklog = 0;

	// 연결 개수 설정 및 현재값
	int32_t mMaxConnectionCount = 0;
	std::atomic<int32_t> mCurrentConnectionCount = 0;

	// 에러 발생 여부
	std::atomic<bool> mHasError = false;

#if _SOCKET_HISTORY_REF_COUNT
private:
	// 디버기 용도, 동시성 문제 존재
	struct HistoryData
	{
		const wchar_t* mT = L"";
		const wchar_t* mR = L"";
		int mB = 0;
		int mA = 0;
	};
	Concurrency::concurrent_queue<HistoryData> mHistory;
#endif

public:
	SocketBase();
	virtual ~SocketBase();

	void SetSocketName(const char* socketName);
	const char* GetSocketNameA() const { return mSocketNameA; }
	const wchar_t* GetSocketNameW() const { return mSocketNameW.data(); }

	void SetIocp(std::shared_ptr<Iocp> iocp) noexcept { mIocp = iocp; }

protected:
	virtual SocketBase* AcquireSocket() = 0;
	virtual void DisposeSocket() = 0;

public:
	void AddRef(MAYBE_UNUSED const wchar_t* reason);
	void InitRef(MAYBE_UNUSED const wchar_t* reason);
	void ReleaseRef(MAYBE_UNUSED const wchar_t* reason);
	int32_t GetRefCnt() const;
	void ResetRefCnt();
	SOCKET GetRawSocket() const noexcept { return mSocket; }
	SocketPtr<SocketBase> GetSocketPtr();

private:
	void Reset();
	bool Open();
	bool CloseAndCleanup();
	void Cleanup();
	bool ChangeSocketState(SocketState socketState);
	
public:
	bool Listen(const wchar_t* ip, uint16_t port, int32_t backlog, int32_t maxConnectionCount);

private:
	void OnChildConnected();
	void OnChildDisconnected();
	void UpdateBacklog(int32_t currentConnectionCount, int32_t currentBacklog);

public:
	bool Connect(const wchar_t* ip, uint16_t port, bool reconnect);
	bool IsConnected() const;	
	bool FlushSend();
	void Disconnect();
	
	friend struct SocketUtil;
	friend struct MainSocketUtil;
	friend struct DbSocketUtil;
	friend struct BotSocketUtil;
#ifdef TEST_BUILD
	friend class TestSocket;
#endif
private:	
	bool Send(NetworkPacketWriter& wp, EncryptionOption encryptionOption = EncryptionOption::ENCRYPTION);
	bool SendBypass(NetworkPacketWriter& bypassPacket, MAYBE_UNUSED NetworkPacketWriter& nestedPacket);

private:
	bool SendImpl(NetworkPacketWriter& wp, EncryptionOption encryptionOption);

private:
	void OnCompleted(Overlapped& overlapped, DWORD bytes, bool error) final;

	void OnCompletedAccept(const Overlapped& overlapped);
	void OnCompletedConnect(const Overlapped& overlapped);
	void OnCompletedDisconnect();
	void OnCompletedSend(const Overlapped& overlapped, DWORD bytes);
	void OnCompletedPreRecv();
	void OnCompletedRecv(DWORD bytes);

	void Accept(SocketBase* parentSocket);
	void PreRecv();
	void HandleCommonError(int errorCode);

protected:
	virtual void OnSizeSend(DWORD /*bytes*/) {}
	virtual void OnSizeRecv(DWORD /*bytes*/) {}

	virtual void OnConnected(bool /*succeed*/) {}
	virtual void OnDisconnected() {}
	virtual bool OnDispose() { return true; }
	virtual Result OnRecv(uint8_t* /*buf*/, size_t /*len*/, OUT size_t& /*recved*/) { return Result::SUCCEEDED; }
	void OnLogPacket(MAYBE_UNUSED NetworkPacket& packet);

	virtual bool HasEncryptionKey() const { return true; }
	virtual void OnTryEncrypt(uint8_t* /*buf*/, int32_t /*len*/) {}
	virtual size_t GetMaxSendBufferQueueSize() const { return SIZE_MAX; }

protected:
	virtual RpcId MakeRemoteRequestId(MAYBE_UNUSED std::promise<NetworkPacket*>&& promise) { return RpcId{}; }
	virtual std::promise<NetworkPacket*>* GetRemoteRequest(MAYBE_UNUSED RpcId remoteRequestId) { return nullptr; }
};

using SocketBasePtr = SocketPtr<SocketBase>;
using SocketBaseWeakPtr = SocketWeakPtr<SocketBase>;
