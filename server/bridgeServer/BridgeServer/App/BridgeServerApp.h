// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"


// BridgeServer 메인 어플리케이션
// [WHY] 외부 서비스 연동(인증, 마켓 결제, 푸시 알림 등)을 담당하는 서버 앱
class BridgeServerApp final : public AppBase
{
private:
	// FrontServer로부터의 연결을 수신하는 리스너 소켓
	std::shared_ptr<class SocketBridgeFromFront> mSocketBridgeFromFrontListener = nullptr;

public:
	explicit BridgeServerApp(AppArg appArg);
};

inline std::shared_ptr<BridgeServerApp> gBridgeServerApp = nullptr;
