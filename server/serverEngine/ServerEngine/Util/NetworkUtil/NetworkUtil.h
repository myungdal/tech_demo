// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NetworkUtil
{
	bool IsMyIp(const wchar_t* myIp);
	std::list<std::wstring> GetMyIpList();
	SOCKADDR_IN GetAddr(const wchar_t* ip, uint16_t port);
	uint16_t GetPort(const SOCKADDR_IN& addr);

	SOCKET CreateSocket();
	bool SetSendBufSize(SOCKET socket, int size);
	bool SetRecvBufSize(SOCKET socket, int size);
	bool SetNoDelay(SOCKET socket, bool noDelay);
	bool SetKeepAlive(SOCKET socket, ULONG keepAliveTime, ULONG keepAliveInterval);
	bool SetReuseAddr(SOCKET socket, bool optVal);
	bool SetLinger(SOCKET socket, int onOff, int linger);
	bool BindAnyIpAddress(SOCKET socket);
	bool UpdateAcceptContext(SOCKET listenSocket, SOCKET acceptSocket);
	void Encrypt(uint8_t* buf, int32_t len, EncryptionKey encryptionKey);
}
