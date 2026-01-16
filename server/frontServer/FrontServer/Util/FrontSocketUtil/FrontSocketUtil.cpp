// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;

// [WHY] FrontSocketUtil은 import FrontServer를 통해 FrontServerSocket 모듈에서 이미 export됨


SocketPtr<SocketFrontToMain> FrontSocketUtil::GetMainSocketPtr()
{
	auto [appInfo, lock]  = gAppListManager->GetMainServerAppInfo();
	if(!appInfo)
		return {};

	auto socket = appInfo->GetSocket();
	if(!socket)
		return {};

	auto* ptr = static_cast<SocketFrontToMain*>(*socket);
	const wchar_t* reason = socket.GetReason();
	return SocketPtr{ ptr, reason };
}

SocketPtr<SocketFrontToDb> FrontSocketUtil::GetDbSocket(AppId appId)
{
	auto [appInfo, lock] = gAppListManager->FindAppInfo(appId);
	if (!appInfo)
		return {};
	
	if (AppType::DB_SERVER != appInfo->GetData().Get_appType())
		return {};
	
	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};
	
	auto* ptr = static_cast<SocketFrontToDb*>(*socket);
	const wchar_t* reason = socket.GetReason();
	return SocketPtr{ ptr, reason };
}

SocketPtr<SocketFrontToGame> FrontSocketUtil::GetGameSocket(AppId appId)
{
	if (INVALID_APP_ID == appId)
		return {};

	auto [appInfo, lock] = gAppListManager->FindAppInfo(appId);
	if (!appInfo)
		return {};

	if (AppType::GAME_SERVER != appInfo->GetData().Get_appType())
		return {};

	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};

	auto* ptr = static_cast<SocketFrontToGame*>(*socket);
	const wchar_t* reason = socket.GetReason();
	return { ptr, reason };
}

SocketPtr<SocketFrontToBridge> FrontSocketUtil::PickBridgeSocketPtr()
{
	auto [appInfo, lock] = gAppListManager->PickBridgeServerAppInfo();
	if (!appInfo)
		return {};

	auto socket = appInfo->GetSocket();
	if (!socket)
		return {};

	auto* ptr = static_cast<SocketFrontToBridge*>(*socket);
	const wchar_t* reason = socket.GetReason();
	return { ptr, reason };
}

