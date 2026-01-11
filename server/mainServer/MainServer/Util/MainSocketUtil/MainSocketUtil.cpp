// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import MainServer;


AppId MainSocketUtil::GetDbAppId(const AccountId accountId)
{
	auto [appInfo, lock] = gAppListManager->GetDbServerAppInfo(accountId);
	if (!appInfo)
		return INVALID_APP_ID; 
	
	return appInfo->GetData().Get_appId();
}

SocketPtr<SocketMainFromDb> MainSocketUtil::GetDbSocket(AppId appId)
{
	auto [appInfo, lock] = gAppListManager->FindAppInfo(appId);
	if (!appInfo)
		return {};

	if (AppType::DB_SERVER != appInfo->GetData().Get_appType())
		return {};

	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};

	auto* ptr = static_cast<SocketMainFromDb*>(*socket);
	const wchar_t* reason = socket.GetReason();
	return SocketPtr{ ptr, reason };
}

SocketPtr<SocketMainFromGame> MainSocketUtil::GetGameSocket(AppId appId)
{
	auto [appInfo, lock] = gAppListManager->FindAppInfo(appId);
	if (!appInfo)
		return {};

	if (AppType::GAME_SERVER != appInfo->GetData().Get_appType())
		return {};

	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};

	auto* ptr = static_cast<SocketMainFromGame*>(*socket);
	const wchar_t* reason = socket.GetReason();
	return SocketPtr{ ptr, reason };
}

AppId MainSocketUtil::PickGameAppId(size_t hash)
{
	auto [appInfo, lock] = gAppListManager->PickGameServerAppInfo(hash);
	if (!appInfo)
		return INVALID_APP_ID;

	const APP_DATA& appData = appInfo->GetData();
	return appData.Get_appId();
}

SocketPtr<SocketMainFromGame> MainSocketUtil::PickGameSocket(size_t hash)
{
	const AppId gameAppId = PickGameAppId(hash);
	return GetGameSocket(gameAppId);
}

