// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/PeerSocket.h"
#include "ServerEngine/AppListManager/AppListManager.h"
#include "ServerEngine/Socket/SocketImpl/CommonShellCommandHandler.h"


class MainPeerSocket : public PeerSocket
{
private:
	static constexpr ClockMs MAX_TIME_SYNC_DELTA = 200ms;

private:
	CommonShellCommandHandler mCommonShellCommandHandler;

public:
	MainPeerSocket();

protected:
	void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) final;

protected:
	virtual void OnAppAdded(MAYBE_UNUSED IN OUT AppInfo& appInfo) {}
	virtual void OnAppRemoved(MAYBE_UNUSED AppId appId) {}
	virtual std::wstring OnShellCommand(MAYBE_UNUSED const wchar_t* command) { return L""; }

public:
	HandleResult OnPacket(MAYBE_UNUSED NetworkPacket& rp);
	HandleResult OnPacket(class SYNC_TIME& rp);
	HandleResult OnPacket(class ACK_GLOBAL_NOW& rp);
	HandleResult OnPacket(class ACK_APP_JOIN& rp);
	HandleResult OnPacket(class APP_ADD& rp);
	HandleResult OnPacket(class APP_REMOVE& rp);
	HandleResult OnPacket(class REQ_SHELL_COMMAND& rp);
	HandleResult OnPacket(class SHELL_NOTIFY& rp);
};
