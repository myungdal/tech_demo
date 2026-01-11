// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// FrontServerSocket.ixx - FrontServer Socket Module Interface
module;

#pragma warning(disable: 5028)

#include "FrontServer/PCH/pch.h"

// Socket headers
#include "FrontServer/Socket/SocketFrontFromClient.h"
#include "FrontServer/Socket/SocketFrontToBridge.h"
#include "FrontServer/Socket/SocketFrontToDb.h"
#include "FrontServer/Socket/SocketFrontToGame.h"
#include "FrontServer/Socket/SocketFrontToMain.h"

// Util header
#include "FrontServer/Util/FrontSocketUtil/FrontSocketUtil.h"

export module FrontServerSocket;

export import ServerEngineCore;
export import ServerEngineNetwork;
export import ServerEngineUtil;
export
{
	using ::SocketFrontFromClient;
	using ::SocketFrontToBridge;
	using ::SocketFrontToDb;
	using ::SocketFrontToGame;
	using ::SocketFrontToMain;
	using ::gSocketFrontToMain;
}

// FrontSocketUtil namespace export
export namespace FrontSocketUtil
{
	using ::FrontSocketUtil::GetMainSocketPtr;
	using ::FrontSocketUtil::GetDbSocket;
	using ::FrontSocketUtil::GetGameSocket;
	using ::FrontSocketUtil::PickBridgeSocketPtr;
}
