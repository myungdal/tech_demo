// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// LogServer.ixx - LogServer Module Interface
module;

#pragma warning(disable: 5028)

#include "LogServer/PCH/pch.h"

// LogServer 앱 헤더
#include "LogServer/App/LogServerApp.h"

export module LogServer;

// ServerEngine modules
export import ServerEngineCore;
export import ServerEngineWorker;
export import ServerEngineNetwork;
export import ServerEngineApp;
export import ServerEngineDb;
export import ServerEngineUtil;
export import ServerEnginePacket;

// LogServer split modules
export import LogServerSocket;
export import LogServerPacket;

export
{
	using ::LogServerApp;
	using ::gLogServerApp;
}
