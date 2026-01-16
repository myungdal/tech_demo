// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// GameServerSocket.ixx - GameServer Socket Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "GameServer/PCH/pch.h"

// Socket 헤더들
#include "GameServer/Socket/SocketGameFromFront.h"
#include "GameServer/Socket/SocketGameToDb.h"
#include "GameServer/Socket/SocketGameToMain.h"

// Util 헤더
#include "GameServer/Util/GameSocketUtil/GameSocketUtil.h"

export module GameServerSocket;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineNetwork;
export import ServerEngineUtil;

export
{
	using ::SocketGameFromFront;
	using ::SocketGameToDb;
	using ::SocketGameToMain;
	using ::gSocketGameToMain;
	using ::GameSocketUtil;
}

