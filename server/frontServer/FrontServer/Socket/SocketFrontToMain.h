// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/MainPeerSocket.h"
#include "ServerEngine/Socket/BypassSocket.h"
#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"


class SocketFrontFromClient;

// Main 서버로 연결되는 소켓
// 서버 목록 관리 및 셸 명령 처리를 담당
class SocketFrontToMain final : public BypassSocket<SocketFrontFromClient, SocketFrontToMain, MAX_POOL_SIZE_FACTOR_SOCKET_FRONT_TO_MAIN, MainPeerSocket>
{
private:
	CommandDispatcherWithResult<std::wstring> mCommandDispatcher;

public:
	SocketFrontToMain();

protected:
	// 새 서버 앱 추가 시 해당 서버로 연결 생성
	void OnAppAdded(IN OUT AppInfo& appInfo) final;

	// 셸 명령 처리
	std::wstring OnShellCommand(const wchar_t* command) override;
	std::wstring OnCommand_help(ArgList& argList);
	std::wstring OnCommand_disconnect(ArgList& argList);

	// 패킷 디스패치
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};

inline std::shared_ptr<SocketFrontToMain> gSocketFrontToMain = nullptr;
