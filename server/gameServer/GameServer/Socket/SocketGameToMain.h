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
#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"


class SocketGameToDb;

class SocketGameToMain final : public Socket<SocketGameToMain, MAX_POOL_SIZE_FACTOR_SOCKET_GAME_TO_MAIN, MainPeerSocket>
{
private:
	std::shared_ptr<SocketGameToDb> mSocketGameToDb = nullptr;

private:
	CommandDispatcherWithResult<std::wstring> mCommandDispatcher;

public:
	SocketGameToMain();

protected:
	void OnAppAdded(IN OUT AppInfo& appInfo) final;

protected:
	std::wstring OnShellCommand(const wchar_t* command) override;
	std::wstring OnCommand_help(ArgList& argList);
	std::wstring OnCommand_print(ArgList& argList);
	std::wstring OnCommand_pause(ArgList& argList);
	std::wstring OnCommand_step(ArgList& argList);
	std::wstring OnCommand_resume(ArgList& argList);
	std::wstring OnCommand_save(ArgList& argList);
	std::wstring OnCommand_rewind(ArgList& argList);
	std::wstring OnCommand_reloaded(ArgList& argList);

protected:
	bool OnDispatchPacket(NetworkPacket& rp) final;
	void DispatchPacket(NetworkPacket& rp);
	void DispatchPacket_async(MAYBE_UNUSED SocketPtrType& ptr, PacketTemp tp);
	void DispatchPacketToHandler(NetworkPacket& rp);
};

inline std::shared_ptr<SocketGameToMain> gSocketGameToMain = nullptr;
