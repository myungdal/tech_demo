// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// BotScene.ixx - Bot Scene Module Interface
module;

#pragma warning(disable: 5028)

#include "Bot/PCH/pch.h"

// Bot core
#include "Bot/Bot/Bot.h"
#include "Bot/Bot/BotContext.h"
#include "Bot/Bot/BotStatistics.h"
#include "Bot/Bot/BotMonitor.h"
#include "Bot/Bot/BotStatistics.h"

// Bot manager
#include "Bot/BotManager/BotAccountManager.h"
#include "Bot/BotManager/BotScenarioManager.h"

// Bot scenes
#include "Bot/BotScene/Base/BotScene.h"
#include "Bot/BotScene/Base/BotSceneFactory.h"
#include "Bot/BotScene/BotSceneDisconnect/BotSceneDisconnect.h"
#include "Bot/BotScene/BotSceneGame/BotSceneGame.h"
#include "Bot/BotScene/BotSceneItem/BotSceneItem.h"
#include "Bot/BotScene/BotSceneLogin/BotSceneLogin.h"
#include "Bot/BotScene/BotSceneMail/BotSceneMail.h"
#include "Bot/BotScene/BotSceneReconnect/BotSceneReconnect.h"

export module BotScene;

export import ServerEngineCore;
export
{
	// Common types
	using ::ValuePerTime;

	using ::Bot;
	using ::BotContext;
	using ::BotStatistics;
	using ::BotMonitor;
	using ::gBotMonitor;
	using ::BotStatistics;
	using ::BotAccountManager;
	using ::gBotAccountManager;
	using ::BotScenarioManager;
	// gBotScenarioManager is not global - created in BotApp
	using ::BotScene;
	using ::BotSceneFactory;
	using ::BotSceneDisconnect;
	using ::BotSceneGame;
	using ::BotSceneItem;
	using ::BotSceneLogin;
	using ::BotSceneMail;
	using ::BotSceneReconnect;
}
