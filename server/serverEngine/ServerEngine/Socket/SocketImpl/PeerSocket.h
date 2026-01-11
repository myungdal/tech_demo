// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketBase/SocketBase.h"


class PeerSocket : public SocketBase
{
private:
	static constexpr ClockMs MIN_RECONNECT_DELAY = 100s;
	static constexpr ClockMs MAX_RECONNECT_DELAY = 10 * 1000ms;

	ClockMs mReconnectDelay = 0ms;

	std::wstring mHostIp = L"";
	uint16_t mHostPort = 0;
	AppId mMyAppId = INVALID_APP_ID;
	AppType mMyAppType = AppType::MAX;
	AppId mHostAppId = INVALID_APP_ID;
	AppType mHostAppType = AppType::MAX;

	EncryptionKey mMyEncryptionKey = 0;
	EncryptionKey mHostEncryptionKey = 0;

	bool mEnableReconnect = true;

public:
	bool Connect(const wchar_t* hostIp, uint16_t hostPort, AppId myAppId, AppType myAppType, AppId hostAppId, AppType hostAppType);

private:
	void TryConnect();
	void Reconnect_timer();

protected:
	void OnConnected(bool succeed) override;
	void OnDisconnected() override;
	bool OnDispose() final;
	Result OnRecv(uint8_t* buf, size_t len, OUT size_t& recved) final;
	bool HasEncryptionKey() const final;
	void OnTryEncrypt(uint8_t* buf, int32_t len) final;

protected:
	virtual void OnEncrypt(MAYBE_UNUSED uint8_t* buf, MAYBE_UNUSED int32_t len, MAYBE_UNUSED EncryptionKey encryptionKey) {}

protected:
	virtual void OnHandshakeCompleted(MAYBE_UNUSED bool reconnected) {}
	virtual void OnLostSocket() {}
	virtual bool OnDispatchPacket(MAYBE_UNUSED class NetworkPacket& rp) { return false; }

protected:
	void UpdateAppListSocketInfo();

public:
	void SetEnableReconnect(bool enable) noexcept { mEnableReconnect = enable; }
	AppId GetHostAppId() const noexcept { return mHostAppId; }
};
