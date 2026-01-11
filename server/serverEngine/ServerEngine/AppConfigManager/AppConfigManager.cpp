// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "AppConfigManager.h"

#include "json.hpp"

import ServerEngineApp;
import ServerEngineUtil;


static nlohmann::json* gJson = nullptr;

AppConfigManager::AppConfigManager()
{
	Load();
}

void AppConfigManager::Load()
{		
	WriteLock lock(mLock);

	gJson = _NEW<nlohmann::json>();
	{
		const char* filename = MAKE_SERVER_DATA_FILE_PATH("config.json");

		std::ifstream jsonFile(filename);
		jsonFile >> *gJson;
	}

	LoadVariable();
	LoadAppIdTable();
	LoadMyAppConfig();
	LoadBotTestConfig();
	LoadDbConfig();
	LoadStaticDataIndex();

	_DELETE(gJson);
	gJson = nullptr;
}

void AppConfigManager::LoadVariable()
{
	mVariableMap.clear();

	auto variableJson = (*gJson)["Variable"];

	for (auto& row : variableJson)
	{
		Buf_wchar_t key = StringUtil::a_to_w(row["Key"].get<std::string>().c_str());
		if (true == row["Value"].is_string())
		{
			Buf_wchar_t value = StringUtil::a_to_w(row["Value"].get<std::string>().c_str());
			_ASSERT_CRASH(nullptr != *value);
			mVariableMap.emplace(*key, *value);
		}
		else if (true == row["Value"].is_number_integer())
		{
			auto [valueStr, valueStrLen] = StringUtil::FormatStr(L"{}", static_cast<int>(row["Value"]));
			_ASSERT_CRASH(nullptr != *valueStr);
			mVariableMap.emplace(*key, *valueStr);
		}
	}
}

void AppConfigManager::LoadAppIdTable()
{
	mAppIdTable.fill({});

	auto appJson = (*gJson)["App"];
	for (auto& row : appJson)
	{
		if (0 == row["Enable"])
			continue;

		Buf_wchar_t appTypeStr = StringUtil::GetJsonStr(row["AppType"]);
		const AppType appType = GetAppTypeEnum(*appTypeStr);
		if (AppType::NONE == appType)
			continue;

		const AppId appId = row["AppId"];
		mAppIdTable[static_cast<size_t>(appType)].emplace_back(appId);
	}
}

void AppConfigManager::LoadMyAppConfig()
{
	mMyAppConfigArray.fill({});

	auto appJson = (*gJson)["App"];
	for (auto& row : appJson)
	{
		Buf_wchar_t ip = StringUtil::GetJsonStr(row["Ip"]);
		if (false == NetworkUtil::IsMyIp(*ip))
			continue;

		mMyIp = *ip;

		if (0 == row["Enable"])
			continue;

		const bool enable = false;
		const AppId appId = row["AppId"];

		Buf_wchar_t appTypeStr = StringUtil::GetJsonStr(row["AppType"]);
		const AppType appType = GetAppTypeEnum(*appTypeStr);

		const ThreadCount workerThreadCount = row["WorkerThreadCount"];
		const int32_t mainDbConnPoolCount = row["MainDbConnPoolCount"];
		const int32_t userDbConnPoolCount = row["UserDbConnPoolCount"];
		const GameChannelCount gameChannelCount = row["GameChannelCount"];
		const int32_t backlog = row["Backlog"];
		const int32_t maxConnectionCount = row["MaxConnectionCount"];
		Buf_wchar_t botDestIpStr = StringUtil::GetJsonStr(row["BotDestIp"]);
		const BotScenarioId botScenarioId = row["BotScenarioId"];
		const int32_t botCount = row["BotCount"];
		const ClockMs botCreationDelay = ClockMs(row["BotCreationDelay"]);
		const ClockMs botTaskDelay = ClockMs(row["BotTaskDelay"]);
		const int32_t botRepeatCount = row["BotRepeatCount"];

		AppConfigDataList& appConfigDataList = mMyAppConfigArray[_IDX(appType)];
		appConfigDataList.emplace_back(
			AppConfigData
			{
				enable,
				appId,
				appType,
				workerThreadCount,
				mainDbConnPoolCount,
				userDbConnPoolCount,
				gameChannelCount,
				backlog,
				maxConnectionCount,
				*botDestIpStr,
				botScenarioId,
				botCount,
				botCreationDelay,
				botTaskDelay,
				botRepeatCount
			}
		);
	}
}

void AppConfigManager::LoadBotTestConfig()
{
	mBotTestTable.clear();

	auto appJson = (*gJson)["BotTest"];

	for (auto& row : appJson)
	{
		BotScenarioId botScenarioId = row["BotScenarioId"];
		Buf_wchar_t botSceneNameStr = StringUtil::GetJsonStr(row["BotSceneName"]);

		mBotTestTable[botScenarioId].emplace_back(*botSceneNameStr);
	}
}

void AppConfigManager::LoadDbConfig()
{
	mDbConfigList.clear();

	auto dbJson = (*gJson)["Db"];

	for (auto& row : dbJson)
	{
		if (0 == row["Enable"])
			continue;

		Buf_wchar_t dbTypeStr = StringUtil::GetJsonStr(row["DbType"]);
		Buf_wchar_t dbNameStr = StringUtil::GetJsonStr(row["DbName"]);
		Buf_wchar_t dbIpStr = StringUtil::GetJsonStr(row["DbIp"]);
		Buf_wchar_t dbUserStr = StringUtil::GetJsonStr(row["DbUser"]);
		Buf_wchar_t dbPwdStr = StringUtil::GetJsonStr(row["DbPwd"]);

		DbConfigData dbConfigData{
			GetDbTypeEnum(*dbTypeStr),
			*dbNameStr,
			row["DbShardIdx"],
			*dbIpStr,
			row["DbPort"],
			*dbUserStr,
			*dbPwdStr
		};

		mDbConfigList.emplace_back(
			std::move(dbConfigData)
		);
	}
}

void AppConfigManager::LoadStaticDataIndex()
{	
	mStaticDataIndexListForGameServer.clear();
	mStaticDataIndexListForClient.clear();

	auto dbJson = (*gJson)["DbTablesStatic"];

	StaticDataIndex index = 0;
	for (auto& row : dbJson)
	{
		if (0 != row["GameServer"])
			mStaticDataIndexListForGameServer.emplace_back(index);

		if (0 != row["Client"])
			mStaticDataIndexListForClient.emplace_back(index);

		++index;
	}
}

AppId AppConfigManager::GetFirstAppId(AppType appType) const
{
	const AppIdList& appIdList = mAppIdTable.at(_IDX(appType));
	const auto it = appIdList.begin();
	return (it == appIdList.end()) ? INVALID_APP_ID : *it;
}

const AppConfigData& AppConfigManager::GetMyAppConfig(AppType appType, AppArg appArg) const
{ 
	if (appArg < 0)
		return INVALID_APP_CONFIG_DATA;

	const AppConfigDataList& appConfigDataList = mMyAppConfigArray[_IDX(appType)];

	if (appConfigDataList.size() <= appArg)
		return INVALID_APP_CONFIG_DATA;

	return appConfigDataList[_IDX(appArg)];
}

const BotTestList* AppConfigManager::GetBotTestList(BotScenarioId botScenarioId) const
{
	const auto it = mBotTestTable.find(botScenarioId);

	if (mBotTestTable.end() == it)
		return nullptr;

	return &it->second;
}

const wchar_t* AppConfigManager::GetVariableStr(const wchar_t* key)
{
	ReadLock lock(mLock);

	const auto it = mVariableMap.find(key);
	_ASSERT_CRASH(mVariableMap.end() != it);
	const std::wstring& value = it->second;
	return value.c_str();
}
