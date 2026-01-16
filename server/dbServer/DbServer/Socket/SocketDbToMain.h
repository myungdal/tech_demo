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
#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"


class SocketDbFromFront;

class SocketDbToMain final : public RequestSocket<SocketDbToMain, MAX_POOL_SIZE_FACTOR_SOCKET_DB_TO_MAIN, MainPeerSocket>
{
private:
	CommandDispatcherWithResult<std::wstring> mShellCommandDispatcher;

public:
	SocketDbToMain();

protected:
	void OnAppAdded(IN OUT AppInfo& appInfo) final;	

protected:
	std::wstring OnShellCommand(const wchar_t* command) override;
	std::wstring OnCommand_help(ArgList& argList);
	std::wstring OnCommand_reload(ArgList& argList);

protected:
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};

inline std::shared_ptr<SocketDbToMain> gSocketDbToMain = nullptr;
