// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketPtr/SocketWeakPtr.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename _SocketType>
struct RemoteSocketInfo
{
	bool mUsing = false;
	SocketWeakPtr<_SocketType> mRemoteSocket;
	RpcToken mToken = 0;
};

