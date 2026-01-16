// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import GameServer;


SocketPtr<SocketGameToDb> GameSocketUtil::GetDbSocket(AppId appId)
{
	auto [appInfo, lock] = gAppListManager->FindAppInfo(appId);

	if (!appInfo)
		return {};

	if (AppType::DB_SERVER != appInfo->GetData().Get_appType())
		return {};

	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};

	auto* ptr = static_cast<SocketGameToDb*>(*socket);
	const wchar_t* reason = socket.GetReason();

	return SocketPtr{ ptr, reason };
}

SocketPtr<SocketGameFromFront> GameSocketUtil::GetFrontSocket(AppId appId)
{
	auto [appInfo, lock] = gAppListManager->FindAppInfo(appId);

	if (!appInfo)
		return {};

	if (AppType::FRONT_SERVER != appInfo->GetData().Get_appType())
		return {};
	
	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};
	
	auto* ptr = static_cast<SocketGameFromFront*>(*socket);
	const wchar_t* reason = socket.GetReason();

	return SocketPtr{ ptr, reason };
}

