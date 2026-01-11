// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketPtr/SocketPtr.h"


class SocketFrontToMain;
class SocketFrontToDb;
class SocketFrontToGame;
class SocketFrontToBridge;

// 프론트 서버 소켓 유틸리티
// 각 서버 타입별 소켓을 조회하는 헬퍼 함수 제공
namespace FrontSocketUtil
{	
	// Main 서버 소켓 조회
	SocketPtr<SocketFrontToMain> GetMainSocketPtr();

	// DB 서버 소켓 조회
	SocketPtr<SocketFrontToDb> GetDbSocket(AppId appId);

	// Game 서버 소켓 조회
	SocketPtr<SocketFrontToGame> GetGameSocket(AppId appId);

	// Bridge 서버 소켓 조회 (라운드로빈)
	SocketPtr<SocketFrontToBridge> PickBridgeSocketPtr();
}
