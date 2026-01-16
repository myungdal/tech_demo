// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbServerSocket.ixx - DbServer Socket Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "DbServer/PCH/pch.h"

// Socket 헤더들
#include "DbServer/Socket/SocketDbFromFront.h"
#include "DbServer/Socket/SocketDbFromGame.h"
#include "DbServer/Socket/SocketDbToLog.h"
#include "DbServer/Socket/SocketDbToMain.h"

export module DbServerSocket;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineNetwork;

export
{
	using ::SocketDbFromFront;
	using ::SocketDbFromGame;
	using ::SocketDbToLog;
	using ::gSocketDbToLog;
	using ::SocketDbToMain;
	using ::gSocketDbToMain;
}

