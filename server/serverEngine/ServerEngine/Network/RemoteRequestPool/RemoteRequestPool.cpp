// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "RemoteRequestPool.h"

#include "Packet/NetworkPacket/NetworkPacket.h"


void RemoteRequestPool::FlushAll()
{
	for (RpcIdx i = 0; i < MAX_POOL_SIZE; ++i)
	{
		RemoteRequestInfo& info = mRemoteRequestPool[i];
		if (info.mUsing)
		{
			info.mUsing = false;
			info.mPromise.set_value(nullptr);
			info.mPromise = {};
		}
	}
}

RpcId RemoteRequestPool::MakeRemoteRequestId(std::promise<NetworkPacket*>&& promise)
{
	RpcIdx idx = ++mIdx;
	idx &= POOL_SIZE_MASK;

	RemoteRequestInfo& info = mRemoteRequestPool[idx];
	if (true == info.mUsing)
	{
		// [WHY] Ring-buffer 방식이라 idx가 랩되면 아직 처리 중인 슬롯을 재사용할 수 있다.
		// [FIX] 기존 요청은 실패(nullptr)로 완료 처리하고 새 요청을 할당한다.
		_DEBUG_RED;
		info.mPromise.set_value(nullptr);
	}

	info.mUsing = true;
	info.mPromise = std::move(promise);
	info.mToken = --mToken;

	return RpcId{ idx, info.mToken };
}

std::promise<NetworkPacket*>* RemoteRequestPool::GetRemoteRequest(RpcId remoteRequestId)
{
	const RpcIdx& idx = remoteRequestId.mIdx;
	const RpcToken& token = remoteRequestId.mToken;

	if (MAX_POOL_SIZE <= idx)
	{
		_DEBUG_RED;
		_DEBUG_BREAK;
		return nullptr;
	}

	RemoteRequestInfo& info = mRemoteRequestPool[idx];
	if (token != info.mToken)
	{
		_DEBUG_RED;
		//_DEBUG_BREAK;
		return nullptr;
	}

	if (info.mUsing == false)
	{
		// 이미 완료/취소된 요청이다. 중복 응답 또는 오래된 RpcId일 수 있다.
		return nullptr;
	}

	info.mUsing = false;

	return &info.mPromise;
}

