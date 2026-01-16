// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/MainPeerSocket.h"
#include "ServerEngine/Socket/RequestSocket.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Shell → Main 서버 소켓
// Main 서버와 연결하여 명령어를 전송하고 결과를 수신한다.
class SocketShellToMain final : public RequestSocket<SocketShellToMain, MAX_POOL_SIZE_FACTOR_SOCKET_SHELL_TO_MAIN, MainPeerSocket>
{
protected:
	// 새 앱 추가 시 호출
	void OnAppAdded(IN OUT AppInfo& appInfo) final;

	// 패킷 디스패치 진입점
	bool OnDispatchPacket(NetworkPacket& rp) final;

private:
	// 패킷 디스패치 분기
	void DispatchPacket(NetworkPacket& rp);

	// 비동기 패킷 처리
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);

	// 핸들러로 패킷 전달
	void DispatchPacketToHandler(NetworkPacket& rp);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<SocketShellToMain> gSocketShellToMain = nullptr;
