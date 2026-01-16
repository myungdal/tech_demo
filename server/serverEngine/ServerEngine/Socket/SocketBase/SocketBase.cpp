// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "SocketBase.h"

#include "Common/Time/ValuePerTime.h"

#include "Packet/PacketFuncs/PacketLogger.h"

import ServerEngineNetwork;
import ServerEngineUtil;
import ServerEngineWorker;
import ServerEngineApp;


SocketBase::SocketBase()
{		
	mSocketRefCounterPtr = std::make_shared<SocketRefCounter>();
}
SocketBase::~SocketBase()
{
	if (INVALID_SOCKET != mSocket)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}
void SocketBase::SetSocketName(const char* socketName)
{ 
	mSocketNameA = socketName;
	mSocketNameW = *StringUtil::a_to_w(mSocketNameA);
}

void SocketBase::AddRef(MAYBE_UNUSED const wchar_t* reason)
{
	//if (0 == reason[0])
	//	_DEBUG_BREAK;

#if _SOCKET_HISTORY_REF_COUNT		
	mHistory.push({ L"AddRef", reason, mSocketRefCounterPtr->GetRefCnt(), mSocketRefCounterPtr->GetRefCnt() + 1 });

	_DEBUG_LOG(WHITE, L"[{}], AddRef, {}, {}, {}", mSocket, reason, mSocketRefCounterPtr->GetRefCnt(), mSocketRefCounterPtr->GetRefCnt() + 1);
#endif

	mSocketRefCounterPtr->IncreaseRefCnt();
}

void SocketBase::InitRef(MAYBE_UNUSED const wchar_t* reason)
{
	//if (0 == reason[0])
	//	_DEBUG_BREAK;

#if _SOCKET_HISTORY_REF_COUNT
	mHistory.push({ L"InitRef", reason, mSocketRefCounterPtr->GetRefCnt(), mSocketRefCounterPtr->GetRefCnt() + 1 });

	_DEBUG_LOG(WHITE, L"[{}], InitRef, {}, {}, {}", mSocket, reason, mSocketRefCounterPtr->GetRefCnt(), mSocketRefCounterPtr->GetRefCnt() + 1);
#endif

	mSocketRefCounterPtr->InitRefCnt();
}

void SocketBase::ReleaseRef(MAYBE_UNUSED const wchar_t* reason)
{
	//if (0 == reason[0])
	//	_DEBUG_BREAK;

#if _SOCKET_HISTORY_REF_COUNT
	mHistory.push({ L"ReleaseRef", reason, mSocketRefCounterPtr->GetRefCnt(), mSocketRefCounterPtr->GetRefCnt() - 1 });

	_DEBUG_LOG(WHITE, L"[{}], ReleaseRef, {}, {}, {}", mSocket, reason, mSocketRefCounterPtr->GetRefCnt(), mSocketRefCounterPtr->GetRefCnt() - 1);
#endif

	int32_t refCnt = mSocketRefCounterPtr->DecreaseRefCnt();

	if (refCnt > 0)
		return;

	if (refCnt < 0)
	{
		_DEBUG_BREAK;
		_DEBUG_RED;
		return;
	}

	// [FIX] DISCONNECTING 상태에서 강제 종료 처리
	// DisconnectEx가 완료되지 않았지만 강제로 정리해야 하는 경우
	// 1. 콜백 호출 (OnChildDisconnected, OnDisconnected)
	// 2. closesocket() + Cleanup()
	// 3. 이후 DisconnectEx 완료 이벤트는 OnCompletedDisconnect에서 무시됨
	if (SocketState::DISCONNECTING == mSocketState.load())
	{
		// DISCONNECTING → DISCONNECTED 강제 전이
		if (true == ChangeSocketState(SocketState::DISCONNECTED))
		{
			if (nullptr != mParentSocket)
				mParentSocket->OnChildDisconnected();

			OnDisconnected();
		}

		// DISCONNECTED → CLOSED 전이 후 정리
		if (true == ChangeSocketState(SocketState::CLOSED))
		{
			if (INVALID_SOCKET != mSocket)
			{
				closesocket(mSocket);
				mSocket = INVALID_SOCKET;
			}

			Cleanup();
		}
	}
	else if (true == ChangeSocketState(SocketState::CLOSED))
	{
		// [FIX] DISCONNECTED → CLOSED 경로에서도 소켓 핸들 닫기
		// 정상적인 DisconnectEx 완료 후 ReleaseRef에서 여기로 진입
		if (INVALID_SOCKET != mSocket)
		{
			closesocket(mSocket);
			mSocket = INVALID_SOCKET;
		}

		Cleanup();
	}
	else if (true == ChangeSocketState(SocketState::CLOSING))
	{
		CloseAndCleanup();
	}
	else
	{
		_DEBUG_BREAK;
		_DEBUG_RED;
		Cleanup();
	}

	if (true == mHasError)
	{
		Reset();
		mHasError = false;
	}

#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}] Dispose, {}", mSocket, (uint64_t)this, reason);
#endif

	if (OnDispose())
	{
		mSocketRefCounterPtr = std::make_shared<SocketRefCounter>();

		DisposeSocket();
	}
}

int32_t SocketBase::GetRefCnt() const
{
	return mSocketRefCounterPtr->GetRefCnt();
}

void SocketBase::ResetRefCnt()
{
	mSocketRefCounterPtr->ResetRefCnt();
}

SocketPtr<SocketBase> SocketBase::GetSocketPtr()
{ 
	return SocketBasePtr().Set(this, L"GetSocketPtr"); 
}

void SocketBase::Reset()
{
	mSocket = NetworkUtil::CreateSocket();

	//NetworkUtil::SetSendBufSize(mSocket, 0);
	//NetworkUtil::SetRecvBufSize(mSocket, 0);
	NetworkUtil::SetKeepAlive(mSocket, 60000, 10000);
	mIocp->BindCompletionHandle(mSocket, this);
}

bool SocketBase::Open()
{
	if (false == ChangeSocketState(SocketState::OPENED))
		return false;

	if (INVALID_SOCKET == mSocket)
	{
		Reset();
	}		

	return true;
}

bool SocketBase::CloseAndCleanup()
{
#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}]", mSocket, (uint64_t)this);
#endif

	if (false == ChangeSocketState(SocketState::CLOSED))
		return false;

	if (INVALID_SOCKET != mSocket)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}

	Cleanup();

	return true;
}

void SocketBase::Cleanup()
{
	mRecvBufHead = 0;
	mRecvBufTail = 0;

	{
		WriteLock lock(mSendBufferQueueLock);

		while (false == mSendBufferQueue.empty())
		{
			const auto& [frontBufPtr, frontBufSize] = mSendBufferQueue.front();

			SendBuffer::Push(frontBufPtr);

			mSendBufferQueue.pop();
		}

		mLastSendBufferOffset = 0;
	}

	mParentSocket = nullptr;
}

bool SocketBase::ChangeSocketState(SocketState socketState)
{
	constexpr SocketState expectedArray[_IDX(SocketState::MAX)] = {
		SocketState::CLOSING,
		SocketState::CLOSED,
		SocketState::OPENED,
		SocketState::CONNECTING,
		SocketState::CONNECTED,
		SocketState::DISCONNECTING,
		SocketState::DISCONNECTED
	};

	SocketState expected = expectedArray[_IDX(socketState)];
	if (true == mSocketState.compare_exchange_strong(expected, socketState))
		return true;

	if (SocketState::CLOSING == socketState)
	{
		expected = SocketState::CONNECTING;
		if (true == mSocketState.compare_exchange_strong(expected, socketState))
			return true;

		expected = SocketState::CLOSED;
		if (true == mSocketState.compare_exchange_strong(expected, socketState))
			return true;

		// [FIX] OPENED → CLOSING 경로 추가
		// OPENED 상태에서 비정상 종료 시 복구 가능하도록
		expected = SocketState::OPENED;
		if (true == mSocketState.compare_exchange_strong(expected, socketState))
			return true;

		// NOTE: DISCONNECTING → CLOSING은 허용하지 않음
		// DISCONNECTING 상태의 강제 종료는 ReleaseRef에서 전용 처리함
	}

	// [FIX] 재접속을 위해 DISCONNECTED -> CLOSED 경로 추가
	// DisconnectEx 완료 후 DISCONNECTED 상태에서 CLOSED로 전이할 수 있어야 
	// Open() -> Connect()가 가능해짐
	if (SocketState::CLOSED == socketState)
	{
		expected = SocketState::DISCONNECTED;
		if (true == mSocketState.compare_exchange_strong(expected, socketState))
			return true;
	}

#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}] {} to {}", mSocket, (uint64_t)this, (int)mSocketState.load(), (int)socketState);
#endif

	return false;
}

bool SocketBase::Listen(const wchar_t* ip, uint16_t port, int32_t backlog, int32_t maxConnectionCount)
{
#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}]", mSocket, (uint64_t)this);
#endif

	if (false == Open())
		return false;

	mAddr = NetworkUtil::GetAddr(ip, port);

	if (SOCKET_ERROR == bind(mSocket, reinterpret_cast<SOCKADDR*>(&mAddr), sizeof(SOCKADDR_IN)))
		return false;

	if (SOCKET_ERROR == listen(mSocket, SOMAXCONN))
		return false;

	mMaxBacklog = backlog;
	mMaxConnectionCount = maxConnectionCount;

	for (int32_t i = 0; i < mMaxBacklog; ++i)
	{
		SocketBase* socket = AcquireSocket();
		socket->SetIocp(mIocp);
		if (true == socket->Open())
		{
			socket->Accept(this);
			mCurrentBacklog.fetch_add(1);
		}
		else
		{
			_DEBUG_RED;
			_DEBUG_BREAK;
		}
	}

	return true;
}

void SocketBase::OnChildConnected()
{
	mCurrentConnectionCount.fetch_add(1);
}

void SocketBase::OnChildDisconnected()
{
	const int32_t currentConnectionCount = mCurrentConnectionCount.fetch_sub(1) - 1;
	const int32_t currentBacklog = mCurrentBacklog.load();

#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}] currentConnectionCount: {}, currentBacklog: {}", mSocket, (uint64_t)this, currentConnectionCount, currentBacklog);
#endif

	UpdateBacklog(currentConnectionCount, currentBacklog);
}

void SocketBase::UpdateBacklog(int32_t currentConnectionCount, int32_t currentBacklog)
{
	// 이미 최대 백로그면 리턴
	if (currentBacklog >= mMaxBacklog)
		return;

	// 백로그 + 접속이 한계를 넘었으면 리턴
	if (currentConnectionCount + currentBacklog >= mMaxConnectionCount)
		return;

	SocketBase* socket = AcquireSocket();
	socket->SetIocp(mIocp);
	if (true == socket->Open())
	{
		socket->Accept(this);
		mCurrentBacklog.fetch_add(1);
	}
	else
	{
		_DEBUG_RED;
		_DEBUG_BREAK;
	}
}

bool SocketBase::Connect(const wchar_t* ip, uint16_t port, bool reconnect)
{
#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}]", mSocket, (uint64_t)this);
#endif

	if (false == Open())
	{
		const int errorCode = WSAGetLastError();
		_DEBUG_LOG(RED, L"SocketBase::Connect, {}", errorCode);

		_DEBUG_RED;
		return false;
	}

	if (false == ChangeSocketState(SocketState::CONNECTING))
	{
		const int errorCode = WSAGetLastError();
		_DEBUG_LOG(RED, L"SocketBase::Connect, {}", errorCode);

		_DEBUG_RED;
		return false;
	}

	// 실패 시 CONNECTING 상태가 남아 재연결을 막지 않도록 롤백 처리
	auto rollback = [this]()
	{
		mHasError = true;

		// CONNECTING -> CLOSING -> CLOSED 로 전이시키며 소켓 정리
		if (true == ChangeSocketState(SocketState::CLOSING))
		{
			CloseAndCleanup();
		}
	};

	if (false == NetworkUtil::SetReuseAddr(mSocket, true))
	{
		const int errorCode = WSAGetLastError();
		_DEBUG_LOG(RED, L"SocketBase::Connect, {}", errorCode);

		_DEBUG_RED;
		rollback();
		return false;
	}
	
	if  (false == NetworkUtil::BindAnyIpAddress(mSocket))
	{
		const int errorCode = WSAGetLastError();
		_DEBUG_LOG(RED, L"SocketBase::Connect, {}", errorCode);

		_DEBUG_RED;
		rollback();
		return false;
	}


	Overlapped* overlapped = Overlapped::Acquire(IoType::CONNECT, nullptr, 0);
	mAddr = NetworkUtil::GetAddr(ip, port);

	if (!reconnect)
		InitRef(L"CONNECT");

	mIsReconnect = reconnect;

	if (TRUE == ConnectEx(mSocket, reinterpret_cast<SOCKADDR*>(&mAddr), sizeof(SOCKADDR_IN), nullptr, 0, 0, static_cast<OVERLAPPED*>(overlapped)))
		return true;

	const int errorCode = WSAGetLastError();

	if (WSA_IO_PENDING == errorCode)
		return true;

	mHasError = true;
	overlapped->Dispose();
	ReleaseRef(L"CONNECT");

	return false;
}

bool SocketBase::IsConnected() const
{
	return (SocketState::CONNECTED == mSocketState.load()) ? true : false;
}

bool SocketBase::Send(NetworkPacketWriter& wp, EncryptionOption encryptionOption)
{
#if _PACKET_SEND_LOG
	if (SocketBase::PACKET_SEND_LOG)
		PacketLogger::LogPacket(L"Send", GetSocketNameW(), mSocket, *wp.GetNetworkPacketPtr());
#endif

	return SendImpl(wp, encryptionOption);
}

bool SocketBase::SendBypass(NetworkPacketWriter& bypassPacket, MAYBE_UNUSED NetworkPacketWriter& nestedPacket)
{
#if _PACKET_SEND_LOG
	if (SocketBase::PACKET_SEND_LOG)
		PacketLogger::LogPacket(L"Send", GetSocketNameW(), mSocket, *nestedPacket.GetNetworkPacketPtr());
#endif

	return SendImpl(bypassPacket, EncryptionOption::ENCRYPTION);
}

bool SocketBase::SendImpl(NetworkPacketWriter& wp, EncryptionOption encryptionOption)
{
#ifdef _DEBUG
	// 카운팅
	{
		static std::atomic<size_t> totalCnt = 0;

		const size_t _totalCnt = ++totalCnt;
		const size_t _size = wp.GetPacketSize();
		const PacketType _packetType = wp.GetPacketType();

		static std::array<std::atomic<size_t>, PacketTypes::END> cntArray;
		static std::array<std::atomic<size_t>, PacketTypes::END> sizeArray;
		static std::array<std::atomic<size_t>, PacketTypes::END> curSizeArray;
		if (1 == _totalCnt)
		{
			for (size_t i = 0; i < PacketTypes::END; ++i)
			{
				cntArray[i] = 0;
				sizeArray[i] = 0;
				curSizeArray[i] = 0;
			}
		}

		++cntArray[_packetType];
		sizeArray[_packetType] += _size;
		curSizeArray[_packetType] = _size;

		if (0 == (_totalCnt % 1000))
		{
			_DEBUG_LOG(WHITE, L"<COUNTING>");

			for (size_t i = 0; i < PacketTypes::END; ++i)
			{
				if (0 == cntArray[i])
					continue;

				_DEBUG_LOG(WHITE, L"packetType: {}, cnt: {}, size: {}({})",
					GetPacketTypeString(static_cast<PacketType>(static_cast<PacketType::PrimitiveType>(i))),
					cntArray[i].load(),
					sizeArray[i].load(),
					curSizeArray[i].load()
				);
			}

			_DEBUG_LOG(WHITE, L"</COUNTING>");
		}
	}
	// 통계
	{
		static std::atomic<size_t> sendCntTotal = 0;
		static std::atomic<size_t> sendSizeTotal = 0;

		const size_t _sendCntTotal = ++sendCntTotal;
		const size_t _sendSize = wp.GetPacketSize();
		sendSizeTotal += _sendSize;

		static std::array<ValuePerTime<1024>, MAX_THREAD_COUNT> valuePerTimeArray;
		static std::array<std::atomic<float>, MAX_THREAD_COUNT> sendCntPerSecResultArray;
		static std::array<std::atomic<float>, MAX_THREAD_COUNT> sendSizePerSecResultArray;

		static std::array<size_t, MAX_THREAD_COUNT> sendCountArray;

		if (1 == _sendCntTotal)
		{
			for (size_t i = 0; i < MAX_THREAD_COUNT; ++i)
			{
				sendCntPerSecResultArray[i] = 0.f;
				sendSizePerSecResultArray[i] = 0.f;
			}
		}

		valuePerTimeArray[tThreadId].Update(_sendSize, tClock.GetLocalNowMs());
		sendCntPerSecResultArray[tThreadId] = valuePerTimeArray[tThreadId].GetUpdatePerTime();
		sendSizePerSecResultArray[tThreadId] = valuePerTimeArray[tThreadId].GetValuePerTime();

		++sendCountArray[tThreadId];

		if (0 == (_sendCntTotal % 1000))
		{
			float sendCntPerSecResult = 0.f;
			float sendSizePerSecResult = 0.f;
			for (size_t i = 0; i < MAX_THREAD_COUNT; ++i)
			{
				if (0 == sendCountArray[i])
					continue;

				sendCntPerSecResult += sendCntPerSecResultArray[i];
				sendSizePerSecResult += sendSizePerSecResultArray[i];
			}

			_DEBUG_LOG(WHITE, L"sendCntPerSec: {:.2f}, sendSizePerSec: {:.0f}, sendCntTotal: {}, sendSizeTotal: {}",
				sendCntPerSecResult,
				sendSizePerSecResult,
				sendCntTotal.load(),
				sendSizeTotal.load()
			);
		}
	}
#endif

	if (false == IsConnected())
	{
		SendBuffer::Push(wp.GetPacketBufPtr());
		return false;
	}

	uint8_t* bufPtr = wp.GetPacketBufPtr();
	PacketSize packetSize = wp.GetPacketSize();

	if (MAX_PACKET_BUFFER_SIZE < packetSize)
	{
		_DEBUG_BREAK;
		SendBuffer::Push(wp.GetPacketBufPtr());
		return false;
	}

	if (EncryptionOption::NO_ENCRYPTION != encryptionOption)
	{
		if (false == HasEncryptionKey())
		{
			SendBuffer::Push(wp.GetPacketBufPtr());
			return false;
		}

		OnTryEncrypt(bufPtr, packetSize);
	}

	{
		WriteLock lock(mSendBufferQueueLock);

		const size_t queueSize = mSendBufferQueue.size();

		if (queueSize == 0)
		{
			mSendBufferQueue.emplace(bufPtr, packetSize);
			return FlushSend();
		}

		if (queueSize > GetMaxSendBufferQueueSize())
		{
			_DEBUG_LOG(RED, L"(GetMaxSendBufferQueueSize() < queueSize)");
			SendBuffer::Push(wp.GetPacketBufPtr());
			return false;
		}

		auto& [ptr, size] = mSendBufferQueue.back();

		if ((size + packetSize) < MAX_PACKET_BUFFER_SIZE)
		{
			std::copy_n(bufPtr, packetSize, ptr + size);
			size += packetSize;
			SendBuffer::Push(bufPtr);
		}
		else
		{
			mSendBufferQueue.emplace(bufPtr, packetSize);
		}
	}

	return true;
}

bool SocketBase::FlushSend()
{
	if (false == IsConnected())
		return false;

	const auto& [frontBufPtr, frontBufSize] = mSendBufferQueue.front();

	_ASSERT_CRASH(frontBufSize > mLastSendBufferOffset);

	PacketSize sendSize = frontBufSize - mLastSendBufferOffset;
	if (MAX_SEND_SIZE < sendSize)
		sendSize = MAX_SEND_SIZE;

	Overlapped* overlapped = Overlapped::Acquire(IoType::SEND, reinterpret_cast<CHAR*>(frontBufPtr) + mLastSendBufferOffset, sendSize);

	AddRef(L"SEND");
	DWORD sent = 0;
	if (SOCKET_ERROR != WSASend(mSocket, &(overlapped->mWsaBuf), 1, &sent, 0, static_cast<OVERLAPPED*>(overlapped), nullptr))
	{
		if (sendSize != sent)
		{
			_DEBUG_RED;
			_DEBUG_BREAK;
		}
		return true;
	}

	const int errorCode = WSAGetLastError();

	if (WSA_IO_PENDING == errorCode)
	{
		return true;
	}

	_DEBUG_RED;

	mHasError = true;
	overlapped->Dispose();
	ReleaseRef(L"SEND");
	HandleCommonError(errorCode);

	return false;
}

void SocketBase::Disconnect()
{
#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}]", mSocket, (uint64_t)this);
#endif

	if (false == ChangeSocketState(SocketState::DISCONNECTING))
	{
		if ((SocketState::CLOSING != mSocketState) &&
			(SocketState::DISCONNECTING != mSocketState))
		{
			_DEBUG_BREAK;
		}

		return;
	}

	Overlapped* overlapped = Overlapped::Acquire(IoType::DISCONNECT, nullptr, 0);

	AddRef(L"DISCONNECT");

	if (TRUE == DisconnectEx(mSocket, static_cast<OVERLAPPED*>(overlapped), TF_REUSE_SOCKET, 0))
	{
		return;
	}

	const int errorCode = WSAGetLastError();

	if (WSA_IO_PENDING == errorCode)
	{
		return;
	}

	mHasError = true;

	overlapped->Dispose();

	ReleaseRef(L"DISCONNECT");
}

void SocketBase::OnCompleted(Overlapped& overlapped, DWORD bytes, bool error)
{
	if (true == mHasError)
	{
		switch (overlapped.mIoType)
		{
		case IoType::ACCEPT:
		case IoType::CONNECT:
			break;
		case IoType::SEND:
			ReleaseRef(L"SEND (OnCompleted)");
			break;
		case IoType::PRE_RECV:
			ReleaseRef(L"PRE_RECV (OnCompleted)");
			break;
		case IoType::RECV:
			ReleaseRef(L"RECV (OnCompleted)");
			break;
		case IoType::DISCONNECT:
			OnCompletedDisconnect();
			ReleaseRef(L"DISCONNECT (OnCompleted)");
			ReleaseRef(L"Close");
			break;
		default:
			_DEBUG_BREAK;
			Disconnect();
			break;
		}
	}
	else if (true == error)
	{
		mHasError = true;

		if (IoType::CONNECT == overlapped.mIoType)
		{			
			if (true == ChangeSocketState(SocketState::CLOSING))
			{
				//CloseAndCleanup();
				//// 연결 중 에러 발생한 소켓을 외부에서 다시 사용할 수 있기 때문에 리셋 해준다.
				//Reset();
				OnConnected(false);
				ReleaseRef(L"ERROR, IoType::CONNECT (OnCompleted)");
			}
			else
			{
				_DEBUG_BREAK;
			}
		}
		else if (IoType::ACCEPT == overlapped.mIoType)
		{
			_DEBUG_BREAK;

			ReleaseRef(L"ERROR, IoType::ACCEPT (OnCompleted)");
		}
		else
		{
			switch (overlapped.mIoType)
			{
			case IoType::SEND:
				ReleaseRef(L"SEND (OnCompleted)");
				break;
			case IoType::PRE_RECV:
				ReleaseRef(L"PRE_RECV (OnCompleted)");
				break;
			case IoType::RECV:
				ReleaseRef(L"RECV (OnCompleted)");
				break;
			case IoType::DISCONNECT:
				ReleaseRef(L"DISCONNECT (OnCompleted)");
				break;
			default:
				_DEBUG_BREAK;
				break;
			}

			if (true == ChangeSocketState(SocketState::DISCONNECTING))
			{
				OnCompletedDisconnect();
				//ReleaseRef(L"ERROR, Close (OnCompleted)");
			}
		}
	}
	else
	{
		switch (overlapped.mIoType)
		{
		case IoType::ACCEPT:
			OnCompletedAccept(overlapped);
			break;
		case IoType::CONNECT:
			OnCompletedConnect(overlapped);
			break;
		case IoType::SEND:
			OnCompletedSend(overlapped, bytes);
			ReleaseRef(L"SEND (OnCompleted)");
			break;
		case IoType::PRE_RECV:
			OnCompletedPreRecv();
			ReleaseRef(L"PRE_RECV (OnCompleted)");
			break;
		case IoType::RECV:
			OnCompletedRecv(bytes);
			ReleaseRef(L"RECV (OnCompleted)");
			break;
		case IoType::DISCONNECT:
			OnCompletedDisconnect();
			ReleaseRef(L"DISCONNECT (OnCompleted)");
			ReleaseRef(L"Close");
			break;
		default:
			_DEBUG_BREAK;
			Disconnect();
			break;
		}
	}
}

void SocketBase::OnCompletedAccept(const Overlapped& overlapped)
{
	SocketBase* accepted = reinterpret_cast<SocketBase*>(overlapped.mWsaBuf.buf);

#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}] accepted: {}", mSocket, (uint64_t)this, uint64_t(accepted));
#endif

	int peerAddrLen = sizeof(SOCKADDR_IN);
	if ((false == NetworkUtil::UpdateAcceptContext(mSocket, accepted->mSocket)) ||
		(SOCKET_ERROR == getpeername(accepted->mSocket, reinterpret_cast<SOCKADDR*>(&accepted->mAddr), &peerAddrLen)) ||
		(false == accepted->ChangeSocketState(SocketState::CONNECTING)))
	{
		// [FIX] Accept 실패는 개별 소켓의 문제이지 리스너 자체의 에러가 아님
		// 리스너의 mHasError를 설정하면 이후 모든 Accept가 처리되지 않아 리스너가 정지됨
		// mHasError = true;  // 제거
		_DEBUG_LOG(RED, L"[{}][{:x}] Accept failed for socket {:x}", mSocket, (uint64_t)this, (uint64_t)accepted);
		accepted->mHasError = true;  // accepted 소켓에만 에러 설정
		accepted->ReleaseRef(L"ACCEPT");
	}
	else
	{
		accepted->OnCompletedConnect(overlapped);
	}

	const int32_t currentConnectionCount = mCurrentConnectionCount.load();
	const int32_t currentBacklog = mCurrentBacklog.fetch_sub(1) - 1;

#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}] currentConnectionCount: {}, currentBacklog: {}", mSocket, (uint64_t)this, currentConnectionCount, currentBacklog);
#endif

	UpdateBacklog(currentConnectionCount, currentBacklog);
}

void SocketBase::OnCompletedConnect(const Overlapped& overlapped)
{
#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}]", mSocket, (uint64_t)this);
#endif

	if (overlapped.Internal)
	{
		ChangeSocketState(SocketState::OPENED);
		OnConnected(false);
		return;
	}

	if (false == ChangeSocketState(SocketState::CONNECTED))
		return;

	if (nullptr != mParentSocket)
		mParentSocket->OnChildConnected();

	OnConnected(true);

	PreRecv();
}

void SocketBase::OnCompletedDisconnect()
{
#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}]", mSocket, (uint64_t)this);
#endif

	// [FIX] 이미 강제 종료된 경우 무시
	// ReleaseRef에서 DISCONNECTING 상태의 강제 종료 처리로 이미 CLOSED 상태일 수 있음
	// DisconnectEx 완료 이벤트가 뒤늦게 오는 경우 상태 충돌 방지
	const SocketState currentState = mSocketState.load();
	if (SocketState::CLOSED == currentState || 
		SocketState::CLOSING == currentState ||
		SocketState::DISCONNECTED == currentState)
	{
		return;
	}

	if (false == ChangeSocketState(SocketState::DISCONNECTED))
		return;

	if (nullptr != mParentSocket)
		mParentSocket->OnChildDisconnected();

	OnDisconnected();
}

void SocketBase::OnCompletedSend(const Overlapped& overlapped, DWORD bytes)
{
	OnSizeSend(bytes);

	if (false == IsConnected())
	{
		_DEBUG_RED;
		return;
	}

	{
		WriteLock lock(mSendBufferQueueLock);

		if (true == mSendBufferQueue.empty())
		{
			// 끊겨서 정리된 것이 아니면 문제.
			if (true == IsConnected())
				_ASSERT_CRASH(false);

			return;
		}

		_ASSERT_CRASH(bytes <= overlapped.mWsaBuf.len);

		mLastSendBufferOffset += bytes;

		const auto& [frontBufPtr, frontBufSize] = mSendBufferQueue.front();

		_ASSERT_CRASH(mLastSendBufferOffset <= frontBufSize);

		// 다 보냈다면, 버퍼 반환.
		if (mLastSendBufferOffset == frontBufSize)
		{
			mLastSendBufferOffset = 0;

			SendBuffer::Push(frontBufPtr);

			mSendBufferQueue.pop();

			if (true == mSendBufferQueue.empty())
			{
				return;
			}
		}

		FlushSend();
	}
}

void SocketBase::OnCompletedPreRecv()
{
	if (false == IsConnected())
		return;

	Overlapped* overlapped = Overlapped::Acquire(IoType::RECV, reinterpret_cast<CHAR*>(mRecvBuf + mRecvBufTail), static_cast<ULONG>(MAX_RECV_BUF_SIZE - mRecvBufTail));
	DWORD bytes = 0;
	DWORD flags = 0;

	AddRef(L"RECV");
	if (SOCKET_ERROR != WSARecv(mSocket, &(overlapped->mWsaBuf), 1, &bytes, &flags, static_cast<OVERLAPPED*>(overlapped), nullptr))
		return;

	const int errorCode = WSAGetLastError();

	if (WSA_IO_PENDING == errorCode)
		return;

	mHasError = true;
	overlapped->Dispose();
	ReleaseRef(L"RECV");
	HandleCommonError(errorCode);
}

void SocketBase::OnCompletedRecv(DWORD bytes)
{
	OnSizeRecv(bytes);

	if (false == IsConnected())
		return;

	if (0 == bytes)
	{
		Disconnect();
		return;
	}

	mRecvBufTail += bytes;

	if (MAX_RECV_BUF_SIZE <= mRecvBufTail)
	{
		Disconnect();
		return;
	}

	const size_t bufLen = mRecvBufTail - mRecvBufHead;
	size_t recved = 0;
	Result result = OnRecv(mRecvBuf + mRecvBufHead, bufLen, recved);

	if (Result::SUCCEEDED != result)
	{
		_DEBUG_RED;
		Disconnect();
		return;
	}

	if (bufLen < recved)
	{
		_DEBUG_RED;
		_DEBUG_BREAK;
		Disconnect();
		return;
	}

	mRecvBufHead += recved;

	PreRecv();
}

void SocketBase::Accept(SocketBase* parentSocket)
{
	mParentSocket = parentSocket;
	const SOCKET listenSocket = parentSocket->mSocket;

#if _SOCKET_HISTORY_CONNECTION
	_DEBUG_LOG(WHITE, L"[{}][{:x}] listenSocket: {}", mSocket, (uint64_t)this, listenSocket);
#endif

	Overlapped* overlapped = Overlapped::Acquire(IoType::ACCEPT, reinterpret_cast<CHAR*>(this), 0);
	DWORD bytes = 0;

	InitRef(L"ACCEPT");
	if (TRUE == AcceptEx(
		listenSocket, mSocket, mRecvBuf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, &bytes, static_cast<OVERLAPPED*>(overlapped)))
		return;

	const int errorCode = WSAGetLastError();

	if (WSA_IO_PENDING == errorCode)
		return;

	mHasError = true;
	overlapped->Dispose();
	ReleaseRef(L"ACCEPT");
}

void SocketBase::PreRecv()
{
	if (false == IsConnected())
		return;

	const size_t bufSize = mRecvBufTail - mRecvBufHead;
	_ASSERT_CRASH(mRecvBufTail >= mRecvBufHead);
	_ASSERT_CRASH(MAX_RECV_BUF_SIZE >= bufSize);

	if (0 == bufSize)
	{
		// 버퍼가 비었으면 포인터만 리셋 (복사 불필요)
		mRecvBufHead = mRecvBufTail = 0;
	}
	else if (mRecvBufHead > 0)
	{
		// Head가 0이 아닌 경우에만 복사 수행 (최적화)
		// 메모리를 앞으로 이동할 때에는 copy 를 사용해야 한다
		std::copy_n(mRecvBuf + mRecvBufHead, bufSize, mRecvBuf);
		mRecvBufHead = 0;
		mRecvBufTail = bufSize;
	}
	// else: bufSize > 0 && mRecvBufHead == 0 → 데이터가 이미 버퍼 시작에 있으므로 복사 불필요

	Overlapped* overlapped =
		Overlapped::Acquire(IoType::PRE_RECV, reinterpret_cast<CHAR*>(mRecvBuf + mRecvBufTail), 0);
	DWORD bytes = 0;
	DWORD flags = 0;

	AddRef(L"PRE_RECV");
	if (SOCKET_ERROR != WSARecv(mSocket, &(overlapped->mWsaBuf), 1, &bytes, &flags, static_cast<OVERLAPPED*>(overlapped), nullptr))
		return;

	const int errorCode = WSAGetLastError();

	if (WSA_IO_PENDING == errorCode)
		return;

	mHasError = true;
	overlapped->Dispose();
	ReleaseRef(L"PRE_RECV");
	HandleCommonError(errorCode);
}

void SocketBase::HandleCommonError(int errorCode)
{
	_DEBUG_LOG(RED, L"[{}][{:x}] errorCode: {}", mSocket, (uint64_t)this, errorCode);

	if ((ERROR_CONNECTION_REFUSED == errorCode) ||
		(WSAENOTCONN == errorCode) ||
		(WSAENOTSOCK == errorCode))
	{
		return;
	}

	if ((WSAECONNRESET == errorCode) ||
		(WSAECONNABORTED == errorCode) ||
		(WSAEINTR == errorCode))
	{
		Disconnect();
		return;
	}
	else if (WSAEINVAL == errorCode)
	{
		return;
	}

	_ASSERT_CRASH(false);
}

void SocketBase::OnLogPacket(MAYBE_UNUSED NetworkPacket& packet)
{
#if _PACKET_RECV_LOG
	if (SocketBase::PACKET_RECV_LOG)
		PacketLogger::LogPacket(L"Recv", GetSocketNameW(), mSocket, packet);
#endif
}
