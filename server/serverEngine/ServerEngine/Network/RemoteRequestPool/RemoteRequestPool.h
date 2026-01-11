// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Network/RemoteRequestPool/RemoteRequestInfo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NetworkPacket;

class RemoteRequestPool
{
private:
	static constexpr milliseconds TIME_OUT = 30'000ms;

private:
	static constexpr RpcIdx MAX_POOL_SIZE_FACTOR = 10;
	static constexpr RpcIdx MAX_POOL_SIZE = (1 << MAX_POOL_SIZE_FACTOR);
	static constexpr RpcIdx POOL_SIZE_MASK = (MAX_POOL_SIZE - 1);

	std::array<RemoteRequestInfo, MAX_POOL_SIZE> mRemoteRequestPool;

	RpcIdx mIdx = 0;
	RpcToken mToken = 0;

public:
	void FlushAll();
	RpcId MakeRemoteRequestId(std::promise<NetworkPacket*>&& promise);
	std::promise<NetworkPacket*>* GetRemoteRequest(RpcId remoteRequestId);
};

