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
#include "ServerEngine/Network/RemoteSocketPool/RemoteSocketPool.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 다른 서버로 패킷을 넘기는 기능을 제공하는 소켓
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _RemoteSocket, typename _ConcreteClass, size_t _PoolSizeFactor, typename _BaseClass = SocketBase>
class BypassSocket : public Socket<_ConcreteClass, _PoolSizeFactor, _BaseClass>
{
private:
	Lock mRemoteSocketPoolLock;
	RemoteSocketPool<_RemoteSocket> mRemoteSocketPool;	

public:
	RpcId MakeRemoteSocketId(_RemoteSocket* remoteSocket)
	{
		WriteLock lock(mRemoteSocketPoolLock);

		return mRemoteSocketPool.MakeRemoteSocketId(remoteSocket);
	}

	SocketPtr<_RemoteSocket> GetSocket(RpcId id)
	{
		WriteLock lock(mRemoteSocketPoolLock);

		return mRemoteSocketPool.GetSocket(id);
	}
};
