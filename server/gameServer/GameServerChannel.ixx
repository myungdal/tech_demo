// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// GameServerChannel.ixx - GameServer Channel Module Interface
module;

// C5028 경고 비활성화: 모듈과 헤더 혼합 사용 시 alignment 불일치 경고
#pragma warning(disable: 5028)

#include "GameServer/PCH/pch.h"

// GameChannelManager 헤더들
#include "GameServer/GameChannelManager/GameChannelManager.h"
#include "GameServer/GameChannelManager/GameChannel.h"
#include "GameServer/GameChannelManager/EntityFactory.h"
#include "GameServer/GameChannelManager/Internal/GameChannelStateMap.h"
#include "GameServer/GameChannelManager/Internal/GameChannelUpdater.h"
#include "GameServer/GameChannelManager/Internal/GameChannelUserTable.h"

export module GameServerChannel;

// 필요한 모듈 import
export import ServerEngineCore;
export import ServerEngineApp;

export
{
	using ::GameChannelManager;
	using ::gGameChannelManager;
	using ::GameChannel;
	using ::EntityFactory;
	using ::GameChannelStateMap;
	using ::GameChannelUpdater;
	using ::GameChannelUserTable;
}

