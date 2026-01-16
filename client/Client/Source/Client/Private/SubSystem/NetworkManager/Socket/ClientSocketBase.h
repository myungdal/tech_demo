// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class NetworkPacket;
class NetworkPacketWriter;

class ISocketSubsystem;
class FSocket;
class FInternetAddr;

class FClientSocketBase
{
protected:
	static constexpr size_t MAX_RECV_BUF_SIZE = 64 * (1 << 20); // 64M

public:
	enum class SocketState : size_t
	{
		CLOSED = 0,
		OPENED,
		CONNECTING,
		CONNECTED,
		HANDSHAKED,
		DISCONNECTED,
		MAX
	};

private:
	SocketState mSocketState = SocketState::CLOSED;
	
private:
	ISocketSubsystem* mSocketSubsystem = nullptr;
	FSocket* mSocket = nullptr;
	TSharedPtr<FInternetAddr> mAddr = nullptr;

private:
	uint8_t mRecvBuf[MAX_RECV_BUF_SIZE] = {};
	size_t mRecvBufHead = 0;
	size_t mRecvBufTail = 0;
		
private:
	using SendBufferInfo = std::pair<uint8_t*, PacketSize>;	
	std::queue<SendBufferInfo> mSendBufferQueue;

private:
	EncryptionKey mMyEncryptionKey = 0;
	EncryptionKey mHostEncryptionKey = 0;

public:
	DISABLE_COPY(FClientSocketBase);
	explicit FClientSocketBase() = default;
	virtual ~FClientSocketBase() = default;

private:
	bool CanChangeSocketState(SocketState socketState);
	void ChangeSocketState(SocketState socketState);

	bool Open();
	bool Close();
	bool HasEncryptionKey() const;
	void Encrypt(uint8_t* buf, int32_t len, EncryptionKey encryptionKey) const;	

public:
	void ConnectSocket(const wchar_t* ip, uint16_t port);
	bool SendNetworkPacket(NetworkPacketWriter& wp, EncryptionOption encryptionOption= EncryptionOption::ENCRYPTION);
	void DisconnectSocket();

	bool IsConnectedSocket() const;
	bool IsHandshaked() const;

private:
	bool TryRecv();
	bool FlushSend();

public:
	virtual void Update();

protected:
	virtual void OnConnected() {}
	virtual void OnDisconnected() {}
	virtual bool OnDispatchPacket(NetworkPacket& /*rp*/) { return false; }
};
