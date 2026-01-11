// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"


class SocketFrontFromClient;

// 프론트 서버 애플리케이션
// 클라이언트 접점 서버로서 클라이언트 연결 및 패킷 중계를 담당
class FrontServerApp final : public AppBase
{
private:
	std::shared_ptr<SocketFrontFromClient> mSocketFrontFromClientListener = nullptr;	

public:
	explicit FrontServerApp(AppArg appArg);
};

inline std::shared_ptr<FrontServerApp> gFrontServerApp = nullptr;
