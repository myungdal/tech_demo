// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/NetworkPacket/NetworkPacket.h"

#include "ServerEngine/Socket/Socket.h"
#include "ServerEngine/Network/RemoteRequestPool/RemoteRequestPool.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 요청을 보내고 응답을 받아 트랜젝션을 처리하는 기능을 제공하는 소켓
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _ConcreteClass, size_t _PoolSizeFactor, typename _BaseClass = SocketBase>
class RequestSocket : public Socket<_ConcreteClass, _PoolSizeFactor, _BaseClass>
{
private:
	Lock mRemoteRequestPoolLock;
	RemoteRequestPool mRemoteRequestPool;

public:
	RpcId MakeRemoteRequestId(std::promise<NetworkPacket*>&& promise) override
	{
		WriteLock lock(mRemoteRequestPoolLock);

		return mRemoteRequestPool.MakeRemoteRequestId(std::move(promise));
	}

	std::promise<NetworkPacket*>* GetRemoteRequest(RpcId remoteRequestId) override
	{
		WriteLock lock(mRemoteRequestPoolLock);

		return mRemoteRequestPool.GetRemoteRequest(remoteRequestId);
	}

	void OnRemoteAck(NetworkPacket& rp)
	{
		WriteLock lock(mRemoteRequestPoolLock);

		RpcId remoteRequestId = rp.GetHeader().GetRemoteRequestId(gMyAppType);
		if (std::promise<NetworkPacket*>* remoteRequest = this->GetRemoteRequest(remoteRequestId))
		{
			remoteRequest->set_value(&rp);
		}
	}

private:
	void OnDisconnected() override
	{
		WriteLock lock(mRemoteRequestPoolLock);

		mRemoteRequestPool.FlushAll();

		__super::OnDisconnected();
	}	
};
