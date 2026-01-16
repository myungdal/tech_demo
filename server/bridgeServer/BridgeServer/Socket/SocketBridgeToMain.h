// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/MainPeerSocket.h"
#include "ServerEngine/Socket/Socket.h"


// MainServer로 연결하는 소켓
// [WHY] BridgeServer는 MainServer와 연결하여 앱 정보를 관리하고 패킷을 교환함
class SocketBridgeToMain final : public Socket<SocketBridgeToMain, MAX_POOL_SIZE_FACTOR_SOCKET_BRIDGE_TO_MAIN, MainPeerSocket>
{
protected:
	// MainServer로부터 앱 정보를 수신했을 때 호출
	void OnAppAdded(IN OUT AppInfo& appInfo) final;

	// 패킷 디스패치 (메인 진입점)
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};

inline std::shared_ptr<SocketBridgeToMain> gSocketBridgeToMain = nullptr;
