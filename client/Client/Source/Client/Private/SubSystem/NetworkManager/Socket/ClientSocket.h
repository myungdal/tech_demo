// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "SubSystem/NetworkManager/Socket/ClientSocketBase.h"


class FClientSocket : public FClientSocketBase
{
public:
	DECLARE_DELEGATE(NetworkEventDelegate);
	NetworkEventDelegate mOnConnected;
	NetworkEventDelegate mOnDisconnected;
	NetworkEventDelegate mOnFatalError;

	DECLARE_DELEGATE_OneParam(DispatchPacketDelegate, NetworkPacket&);
	DispatchPacketDelegate mOnDispatchPacket;

private:
	static constexpr ClockDuration CONNECTION_TIME_OUT = 5s;
	static constexpr uint32_t MAX_RECONNECT_COUNT = 3;
	static constexpr ClockMs MAX_RECONNECT_DELAY = 8000ms;

	enum class SessionState : size_t
	{
		DISCONNECTED,
		CONNECTING,
		CONNECTED,
		DISCONNECTING,
		RECONNECT_PENDING,
		MAX
	};

private:	
	SessionState mSessionState = SessionState::DISCONNECTED;
	uint32_t mReconnectCount = 0;

	std::wstring mIp = L"";
	uint16_t mPort = 0;

	ClockPoint mTimeConnect = CLOCK_NOW;
	ClockPoint mTimeReconnect = CLOCK_NOW;

public:
	DISABLE_COPY(FClientSocket);
	explicit FClientSocket();
	virtual ~FClientSocket();

public:
	void ConnectSession(const wchar_t* ip, uint16_t port);
	bool IsConnectedSession() const { return (SessionState::CONNECTED == mSessionState) ? true : false;  }
	void DisconnectSession();

public:
	void Update() override;

protected:
	void OnConnected() override;
	void OnDisconnected() override;
	bool OnDispatchPacket(NetworkPacket& rp) override;
};
