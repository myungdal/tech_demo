// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ServerEngineApp.ixx - App Module
// AppBase, Http, PacketTransactor 제공

module;

// GMF - AppBase
#include "ServerEngine/AppBase/AppBase.h"
#include "ServerEngine/AppConfigManager/AppConfigManager.h"
#include "ServerEngine/AppConfigManager/AppConfigData.h"
#include "ServerEngine/AppConfigManager/DbConfigData.h"
#include "ServerEngine/AppListManager/AppListManager.h"
#include "ServerEngine/AppListManager/AppInfo.h"

// GMF - Http
#include "ServerEngine/Http/HttpClient.h"

// GMF - PacketTransactor
#include "ServerEngine/PacketTransactor/PacketTransactorBase.h"
#include "ServerEngine/PacketTransactor/DbAndPacketTransactor.h"

export module ServerEngineApp;

// ============= AppBase =============
export using ::AppBase;
export using ::AppConfigManager;
export using ::AppConfigData;
export using ::DbConfigData;
export using ::AppListManager;
export using ::AppInfo;
export using ::gAppConfigManager;
export using ::gAppListManager;

// ============= Http =============
export using ::HttpClient;

// ============= PacketTransactor =============
export using ::PacketTransactorBase;
export using ::DbAndPacketTransactor;

