// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// LogServerSocket.ixx - LogServer Socket Module Interface
module;

#pragma warning(disable: 5028)

#include "LogServer/PCH/pch.h"

#include "LogServer/Socket/SocketLogFromDb.h"
#include "LogServer/Socket/SocketLogToMain.h"

export module LogServerSocket;

export import ServerEngineCore;
export import ServerEngineNetwork;
export
{
	using ::SocketLogFromDb;
	using ::SocketLogToMain;
	using ::gSocketLogToMain;
}
