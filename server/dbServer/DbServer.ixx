// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbServer.ixx - DbServer Module Interface
module;

#pragma warning(disable: 5028)

// DbServer 기본 PCH (pch_serverEngine.h + DbUserPtr 포함)
#include "DbServer/PCH/pch.h"

// DbServer 관련 헤더들
#include "DbServer/App/DbServerApp.h"
#include "DbServer/DbUserManager/DbUserManager.h"
// DbUserContext, DbUserSocketState는 DbUser.h보다 먼저 include (forward declaration 해결)
#include "DbServer/DbUserManager/DbUserSocketState.h"
#include "DbServer/DbUserManager/DbUserContext.h"
#include "DbServer/DbUserManager/DbUser.h"
#include "DbServer/StaticDb/StaticDbLoader.h"
#include "DbServer/CheatManager/CheatManager.h"

export module DbServer;

// ServerEngine 모듈
export import ServerEngineCore;
export import ServerEngineWorker;
export import ServerEngineNetwork;
export import ServerEngineApp;
export import ServerEngineDb;
export import ServerEngineUtil;

// DbServer 하위 모듈
export import DbServerSocket;
export import DbServerPacket;
export import DbServerUtil;
export import DbServerStaticDb;

export
{
	using ::DbServerApp;
	using ::gDbServerApp;
	using ::DbUserManager;
	using ::gDbUserManager;
	using ::DbUser;
	using ::DbUserPtr;
	using ::DbUserContext;
	using ::DbUserSocketState;
	using ::CheatManager;
	using ::gCheatManager;
}
