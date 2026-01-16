// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// MainServer.ixx - MainServer Module Interface
module;

// C5028 경고 비활성화: 모듈 경계 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "MainServer/PCH/pch.h"

// MainServer 전용 헤더들
#include "MainServer/App/MainServerApp.h"
#include "MainServer/AccountManager/AccountManager.h"
#include "MainServer/AccountManager/Account.h"
#include "MainServer/AccountManager/AccountStore.h"
#include "MainServer/ServerListManager/ServerListManager.h"
#include "MainServer/ServerListManager/Server.h"
#include "MainServer/GameListManager/GameListManager.h"
#include "MainServer/GameListManager/Game.h"
#include "MainServer/GameListManager/GameRegistry.h"
#include "MainServer/RoomListManager/RoomListManager.h"
#include "MainServer/RoomListManager/Room.h"

export module MainServer;

// ServerEngine 모듈
export import ServerEngineCore;
export import ServerEngineWorker;
export import ServerEngineNetwork;
export import ServerEngineApp;
export import ServerEngineDb;
export import ServerEngineUtil;

// MainServer 하위 모듈
export import MainServerSocket;
export import MainServerPacket;
export import MainServerUtil;

export
{
	using ::MainServerApp;
	using ::gMainServerApp;
	using ::AccountManager;
	using ::gAccountManager;
	using ::Account;
	using ::AccountUserList;
	using ::AccountStore;
	using ::ServerListManager;
	using ::gServerListManager;
	using ::Server;
	using ::GameListManager;
	using ::gGameListManager;
	using ::Game;
	using ::GameRegistry;
	using ::RoomListManager;
	using ::gRoomListManager;
	using ::Room;
}
