// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;

#include "DbSocketUtil.h"


SocketPtr<SocketDbToMain> DbSocketUtil::GetMainSocketPtr()
{
	auto [appInfo, lock] = gAppListManager->GetMainServerAppInfo();
	if (!appInfo)
		return {};

	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};

	auto* ptr = static_cast<SocketDbToMain*>(*socket);
	const wchar_t* reason = socket.GetReason();
	return SocketPtr{ ptr, reason };
}

SocketPtr<SocketDbFromGame> DbSocketUtil::GetGameSocket(AppId gameAppId)
{
	auto [appInfo, lock] = gAppListManager->FindAppInfo(gameAppId);
	if (!appInfo)
		return {};

	if (appInfo->GetData().Get_appType() != AppType::GAME_SERVER)
		return {};

	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};

	auto* ptr = static_cast<SocketDbFromGame*>(*socket);
	const wchar_t* reason = socket.GetReason();
	return { ptr, reason };
}

