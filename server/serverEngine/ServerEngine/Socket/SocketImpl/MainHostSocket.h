// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketImpl/HostSocket.h"


class MainHostSocket : public HostSocket
{
private:
	std::atomic<AppId> mAppId = INVALID_APP_ID;

public:
	MainHostSocket();

	AppId GetAppId() const { return mAppId.load(); }

protected:
	void OnConnected(bool succeed) override;
	void OnDisconnected() override;

protected:
	virtual void OnAppAdded(MAYBE_UNUSED IN OUT AppInfo& appInfo) {}
	virtual void OnAppRemoved(MAYBE_UNUSED AppId appId) {}

public:
	HandleResult OnPacket(MAYBE_UNUSED NetworkPacket& rp);
	HandleResult OnPacket(class REQ_GLOBAL_NOW& rp);
	HandleResult OnPacket(class REQ_APP_JOIN& rp);
};
