// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"


class SocketGameFromFront;

class GameServerApp final : public AppBase
{
private:
	std::shared_ptr<SocketGameFromFront> mSocketGameFromFrontListener = nullptr;

public:
	explicit GameServerApp(AppArg appArg);
};

inline std::shared_ptr<GameServerApp> gGameServerApp = nullptr;
