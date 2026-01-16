// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Network/RemoteSocketPool/RemoteSocketInfo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename _SocketType>
class RemoteSocketPool
{
private:
	static constexpr RpcIdx MAX_POOL_SIZE_FACTOR = 10;
	static constexpr RpcIdx MAX_POOL_SIZE = (1 << MAX_POOL_SIZE_FACTOR);
	static constexpr RpcIdx POOL_SIZE_MASK = (MAX_POOL_SIZE - 1);

	using RemoteSocketInfoType = RemoteSocketInfo<_SocketType>;
	using RemoteSocketInfoPool = std::array<RemoteSocketInfoType, MAX_POOL_SIZE>;

	RemoteSocketInfoPool mRemoteSocketInfoPool;

	RpcIdx mIdx = 0;
	RpcToken mToken = 0;

public:
	RemoteSocketPool()
	{
		for (RpcIdx i = 0; i < MAX_POOL_SIZE; ++i)
		{
			RemoteSocketInfoType& info = mRemoteSocketInfoPool[i];
			info.mRemoteSocket = nullptr;
			info.mToken = --mToken;
		}
	}

	RpcId MakeRemoteSocketId(_SocketType* remoteSocket)
	{
		if (nullptr == remoteSocket)
		{
			_DEBUG_RED;
			_DEBUG_BREAK;
			return INVALID_RPC_ID;
		}

		RpcIdx idx = ++mIdx;
		idx &= POOL_SIZE_MASK;

		RemoteSocketInfoType& info = mRemoteSocketInfoPool[idx];
		if (true == info.mUsing)
		{
			// [WHY] Ring-buffer 방식이라 idx가 랩되면 아직 살아있는 슬롯을 재사용할 수 있다.
			// 이 경우 호출자는 오래된 RpcId를 보관하고 있을 가능성이 높으므로 경고 로그만 남긴다.
			_DEBUG_RED;
		}

		info.mUsing = true;
		info.mRemoteSocket.Set(remoteSocket, L"RemoteSocketPool, WeakPtr");
		info.mToken = --mToken;

		return RpcId{ idx, info.mToken };
	}

	SocketPtr<_SocketType> GetSocket(RpcId remoteSocketId)
	{
		const RpcIdx& idx = remoteSocketId.mIdx;
		const RpcToken& token = remoteSocketId.mToken;

		if (MAX_POOL_SIZE <= idx)
		{
			_DEBUG_RED;
			_DEBUG_BREAK;
			return {};
		}

		RemoteSocketInfoType& info = mRemoteSocketInfoPool[idx];
		if (token != info.mToken)
		{
			_DEBUG_RED;
			//_DEBUG_BREAK;
			return {};
		}

		if (false == info.mUsing)
		{
			// 이미 회수된(또는 무효화된) RpcId다.
			return {};
		}

		info.mUsing = false;

		auto remoteSocket = info.mRemoteSocket.Lock();
		return remoteSocket;
	}
};

