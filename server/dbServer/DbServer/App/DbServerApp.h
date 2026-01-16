// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"


class DbServerApp final : public AppBase
{
private:
	std::shared_ptr<class SocketDbFromFront> mSocketDbFromFrontListener = nullptr;
	std::shared_ptr<class SocketDbFromGame> mSocketDbFromGameListener = nullptr;

public:
	explicit DbServerApp(AppArg appArg);
};

inline std::shared_ptr<DbServerApp> gDbServerApp = nullptr;

