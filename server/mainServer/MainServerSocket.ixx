// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// MainServerSocket.ixx - MainServer Socket Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "MainServer/PCH/pch.h"

// Socket 헤더들
#include "MainServer/Socket/SocketMainFromBridge.h"
#include "MainServer/Socket/SocketMainFromDb.h"
#include "MainServer/Socket/SocketMainFromFront.h"
#include "MainServer/Socket/SocketMainFromGame.h"
#include "MainServer/Socket/SocketMainFromLog.h"
#include "MainServer/Socket/SocketMainFromShell.h"

export module MainServerSocket;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineNetwork;

export
{
	using ::SocketMainFromBridge;
	using ::SocketMainFromDb;
	using ::SocketMainFromFront;
	using ::SocketMainFromGame;
	using ::SocketMainFromLog;
	using ::SocketMainFromShell;
}

