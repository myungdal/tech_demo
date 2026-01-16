// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Socket/SocketBase/SocketBase.h"


class HostSocket : public SocketBase
{
private:
	EncryptionKey mMyEncryptionKey = 0;
	EncryptionKey mPeerEncryptionKey = 0;
	AppId mPeerAppId = INVALID_APP_ID;
	AppType mPeerAppType = AppType::MAX;

protected:
	void OnConnected(bool succeed) override;
	void OnDisconnected() override;
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
	AppId GetPeerAppId() const { return mPeerAppId; }
};
