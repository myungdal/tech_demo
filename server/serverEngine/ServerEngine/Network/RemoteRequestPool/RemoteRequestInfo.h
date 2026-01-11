// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class NetworkPacket;

struct RemoteRequestInfo
{
	bool mUsing = false;
	std::promise<NetworkPacket*> mPromise;
	RpcToken mToken = 0;
};

