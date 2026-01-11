// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"


class MainServerApp final : public AppBase
{
private:
	std::shared_ptr<class SocketMainFromBridge> mSocketMainFromBridgeListener = nullptr;
	std::shared_ptr<class SocketMainFromDb> mSocketMainFromDbListener = nullptr;
	std::shared_ptr<class SocketMainFromFront> mSocketMainFromFrontListener = nullptr;
	std::shared_ptr<class SocketMainFromGame> mSocketMainFromGameListener = nullptr;
	std::shared_ptr<class SocketMainFromLog> mSocketMainFromLogListener = nullptr;
	std::shared_ptr<class SocketMainFromShell> mSocketMainFromShellListener = nullptr;

public:
	explicit MainServerApp(AppArg appArg);
};

inline std::shared_ptr<MainServerApp> gMainServerApp = nullptr;
