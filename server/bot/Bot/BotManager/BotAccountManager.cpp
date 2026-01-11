// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Bot;
import ServerEngineCommon;

#include "json.hpp"


BotAccountManager::BotAccountManager()
{
	std::list<std::wstring> ipList = NetworkUtil::GetMyIpList();
	_ASSERT_CRASH(false == ipList.empty());
	mIpAddress = *ipList.begin();
}
void BotAccountManager::Load()
{
	WriteLock lock(mLock);

	//WriteLock jsonLock(gJsonLock);
	nlohmann::json json;

	std::ifstream jsonFile(MAKE_BOT_DATA_FILE_PATH("botUserIdList.json"));

	if (true == jsonFile.is_open())
	{
		try
		{
			jsonFile >> json;
		}		
		catch(...)
		{
			json["list"] = nlohmann::json::array();
		}
	}
	else
	{
		json["list"] = nlohmann::json::array();
	}

	mBotAccountMap.clear();

	for (auto& obj : json["list"])
	{
		BotId botId = obj["botId"];
		Buf_wchar_t accountId = StringUtil::GetJsonStr(obj["accountId"]);
		Buf_wchar_t deviceToken = StringUtil::GetJsonStr(obj["deviceToken"]);
		Buf_wchar_t googleToken = StringUtil::GetJsonStr(obj["googleToken"]);
		Buf_wchar_t appleToken = StringUtil::GetJsonStr(obj["appleToken"]);

		mBotAccountMap.emplace(
			botId,
			ACCOUNT::Writer(PARAM, TEMP_BUF,
				AccountId(StringUtil::w_to_int64(*accountId)),
				*deviceToken,
				*googleToken,
				*appleToken,
				INVALID_DB_SHARD_IDX,
				AccountState::NONE,
				time_t{ 0 },
				time_t{ 0 },
				time_t{ 0 }
			).GetPacketBufPtr()
		);
	}
}

void BotAccountManager::Save()
{
	ReadLock lock(mLock);

	//WriteLock jsonLock(gJsonLock);
	nlohmann::json json;

	json["list"] = nlohmann::json::array();

	for (const auto& it : mBotAccountMap)
	{
		auto [accountIdStr, accountIdLen] = StringUtil::FormatStr(L"{}", it.second->Get_c_account_id().GetData());

		const BotId botId = it.first;
		Buf_char accountId = StringUtil::w_to_a(*accountIdStr);
		Buf_char deviceToken = StringUtil::w_to_a(it.second->Get_c_device_token());
		Buf_char googleToken = StringUtil::w_to_a(it.second->Get_c_google_token());
		Buf_char appleToken = StringUtil::w_to_a(it.second->Get_c_apple_token());
	
		json["list"].push_back(
			{
				{ "botId", botId },
				{ "accountId", *accountId },
				{ "deviceToken", *deviceToken },
				{ "googleToken", *googleToken },
				{ "appleToken", *appleToken }
			}
		);
	}

	std::ofstream jsonFile = CreateOutputFileStream(MAKE_BOT_DATA_FILE_PATH("botUserIdList.json"));
	jsonFile << std::setw(4) << json << std::endl;
}

void BotAccountManager::Add(BotId botId, const ACCOUNT& account)
{
	WriteLock lock(mLock);

	mBotAccountMap.emplace(botId, account);
}

void BotAccountManager::Remove(BotId botId)
{
	WriteLock lock(mLock);

	mBotAccountMap.erase(botId);
}

std::pair<const ACCOUNT*, WriteLock> BotAccountManager::Find(BotId botId)
{
	WriteLock lock(mLock);

	auto it = mBotAccountMap.find(botId);

	if (mBotAccountMap.end() == it)
	{
		auto [deviceToken, deviceTokenLen] = StringUtil::FormatStr(L"{}_{}", mIpAddress, botId);

		// 인증 정보를 파일에 저장
		Add(
			botId,
			ACCOUNT::Writer(PARAM, TEMP_BUF,
				INVALID_UUID,
				*deviceToken,
				L"",
				L"",
				INVALID_DB_SHARD_IDX,
				AccountState::NONE,
				time_t{ 0 },
				time_t{ 0 },
				time_t{ 0 }
			).GetPacket()
		);

		gBotAccountManager->Save();
	}

	it = mBotAccountMap.find(botId);

	_ASSERT_CRASH(mBotAccountMap.end() != it);

	const ACCOUNT* result = *it->second;

	return std::make_pair(result, std::move(lock));
}
