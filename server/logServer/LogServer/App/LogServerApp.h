// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"


class LogServerApp final : public AppBase
{
private:
	std::shared_ptr<class SocketLogFromDb> mSocketLogFromDbListener = nullptr;

public:
	explicit LogServerApp(AppArg appArg);
};

inline std::shared_ptr<LogServerApp> gLogServerApp = nullptr;
