// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ClientSocket.h"

#include "SubSystem/NetworkManager/NetworkManager.h"


FClientSocket::FClientSocket()
{
}

FClientSocket::~FClientSocket()
{
	DisconnectSession();
}

void FClientSocket::ConnectSession(const wchar_t* ip, uint16_t port)
{
	mSessionState = SessionState::CONNECTING;

	mIp = ip;
	mPort = port;

	mReconnectCount = 0;
	mTimeConnect = CLOCK_NOW;

	FClientSocketBase::ConnectSocket(ip, port);
}

void FClientSocket::DisconnectSession()
{
	if (SessionState::CONNECTED != mSessionState)
		return;

	mSessionState = SessionState::DISCONNECTING;

	FClientSocketBase::DisconnectSocket();
}

void FClientSocket::Update()
{
	switch (mSessionState)
	{
	case SessionState::RECONNECT_PENDING:
	{
		// Exponential backoff: 0초 -> 1초 -> 2초 -> 4초 -> 8초 (최대)
		// mReconnectCount: 0(즉시), 1(1초), 2(2초), 3(4초), 4+(8초)
		ClockMs reconnectDelay = 0ms;
		if (mReconnectCount > 0)
		{
			// 2^(count-1) * 1000ms, 최대 8초
			int64_t delayMs = (1LL << (mReconnectCount - 1)) * 1000;
			reconnectDelay = ClockMs(std::min(delayMs, static_cast<int64_t>(MAX_RECONNECT_DELAY.count())));
		}

		ClockPoint now = CLOCK_NOW;
		ClockMs elapsed = CLOCK_MS_FROM_DR(now - mTimeReconnect);
		if (elapsed < reconnectDelay)
			break; // 아직 대기 중

		mSessionState = SessionState::CONNECTING;
		mTimeConnect = CLOCK_NOW;
		FClientSocketBase::ConnectSocket(mIp.c_str(), mPort);
		break;
	}
	case SessionState::CONNECTING:
	{
		ClockPoint now = CLOCK_NOW;
		ClockDuration delta = now - mTimeConnect;
		if (CONNECTION_TIME_OUT < delta)
		{
			FClientSocketBase::DisconnectSocket();
		}
		else
		{
			FClientSocketBase::Update();
		}
		break;
	}
	case SessionState::CONNECTED:
	{
		FClientSocketBase::Update();
		break;
	}
	default:
	{
		break;
	}
	}
}

void FClientSocket::OnConnected()
{
	mSessionState = SessionState::CONNECTED;
	mReconnectCount = 0; // 성공 시 재연결 카운터 리셋

	mOnConnected.ExecuteIfBound();
}

void FClientSocket::OnDisconnected()
{
	switch (mSessionState)
	{
	case SessionState::CONNECTING:
	{
		++mReconnectCount;
		if (MAX_RECONNECT_COUNT <= mReconnectCount)
		{
			mSessionState = SessionState::DISCONNECTED;
			mOnDisconnected.ExecuteIfBound();
		}
		else
		{
			mSessionState = SessionState::RECONNECT_PENDING;
			mTimeReconnect = CLOCK_NOW; // 재연결 대기 시작 시간 기록
		}
		break;
	}	
	case SessionState::CONNECTED:
	{
		mReconnectCount = 0; // 정상 연결 중 끊김 시 카운터 리셋
		mSessionState = SessionState::RECONNECT_PENDING;
		mTimeReconnect = CLOCK_NOW; // 재연결 대기 시작 시간 기록
		mOnDisconnected.ExecuteIfBound();
		break;
	}
	case SessionState::DISCONNECTED:
	case SessionState::DISCONNECTING:
	{
		mSessionState = SessionState::DISCONNECTED;
		mOnDisconnected.ExecuteIfBound();
		break;
	}
	default:
	{
		break;
	}
	}
}

bool FClientSocket::OnDispatchPacket(NetworkPacket& rp)
{
	mOnDispatchPacket.ExecuteIfBound(rp);
	
	return true;
}
