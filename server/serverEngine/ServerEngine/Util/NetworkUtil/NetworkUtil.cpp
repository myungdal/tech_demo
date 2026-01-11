// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "NetworkUtil.h"


bool NetworkUtil::IsMyIp(const wchar_t* myIp)
{
	const std::wstring myIpStr = myIp;
	if (std::wstring(L"127.0.0.1") == myIpStr)
		return true;

	std::list<std::wstring> ipList = NetworkUtil::GetMyIpList();
	
	auto it = std::find(ipList.begin(), ipList.end(), myIpStr);
	return (ipList.end() == it) ? false : true;
}

std::list<std::wstring> NetworkUtil::GetMyIpList()
{
	std::list<std::wstring> ipList;

	// 첫 번째 호출: 필요한 버퍼 크기 조회
	ULONG ulOutBufLen = 0;
	GetAdaptersInfo(nullptr, &ulOutBufLen);

	if (ulOutBufLen == 0)
		return ipList;

	// ulOutBufLen은 바이트 단위이므로, 구조체 개수로 환산해 배열을 할당한다.
	const size_t adapterCount = (ulOutBufLen + sizeof(IP_ADAPTER_INFO) - 1) / sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = new IP_ADAPTER_INFO[adapterCount];

	if (NO_ERROR == GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
	{
		PIP_ADAPTER_INFO p0 = pAdapterInfo;
		while (p0)
		{
			IP_ADDR_STRING* p1 = &p0->IpAddressList;
			while (p1)
			{
				Buf_wchar_t ipAddressStr = StringUtil::a_to_w(&p1->IpAddress.String[0]);

				ipList.push_back(*ipAddressStr);
				p1 = p1->Next;
			}
			p0 = p0->Next;
		}
	}

	delete[] pAdapterInfo;

	return ipList;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SOCKADDR_IN NetworkUtil::GetAddr(const wchar_t* ip, uint16_t port)
{
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));

	Buf_char ipA = StringUtil::w_to_a(ip);

#pragma warning(push)
#pragma warning(disable:4996)
	unsigned long inetAddr = inet_addr(*ipA);
	if (INADDR_NONE == inetAddr)
	{
		HOSTENT* HostEnt = gethostbyname(*ipA);
		if (HostEnt && PF_INET == HostEnt->h_addrtype)
			inetAddr = ((in_addr*)(*HostEnt->h_addr_list))->s_addr;
	}
#pragma warning(pop)

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inetAddr;
	addr.sin_port = htons(port);

	return addr;
}

uint16_t NetworkUtil::GetPort(const SOCKADDR_IN& addr)
{
	return ntohs(addr.sin_port);
}

SOCKET NetworkUtil::CreateSocket()
{
	//const auto socketFlag = (WSA_FLAG_OVERLAPPED | WSA_FLAG_NO_HANDLE_INHERIT);
	//mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, socketFlag);

	SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	::SetHandleInformation((HANDLE)socket, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
	return socket;
}

bool NetworkUtil::SetSendBufSize(SOCKET socket, int size)
{
	int optVal = size;
	constexpr int optLen = sizeof(optVal);

	return (SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&optVal), optLen)) ? true : false;
}

bool NetworkUtil::SetRecvBufSize(SOCKET socket, int size)
{
	int optVal = size;
	const int optLen = sizeof(optVal);

	return (SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char*>(&optVal), optLen)) ? true : false;
}

bool NetworkUtil::SetNoDelay(SOCKET socket, bool noDelay)
{
	const int optLen = sizeof(noDelay);

	return (SOCKET_ERROR != setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&noDelay), optLen)) ? true : false;
}

bool NetworkUtil::SetKeepAlive(SOCKET socket, ULONG keepAliveTime, ULONG keepAliveInterval)
{
	tcp_keepalive keepAlive{1, keepAliveTime, keepAliveInterval };

	DWORD retBytes = 0;
	if (0 != WSAIoctl(socket, SIO_KEEPALIVE_VALS, &keepAlive, sizeof(tcp_keepalive), nullptr, 0, &retBytes, nullptr, nullptr))
	{
		const int errorCode = WSAGetLastError();

		if (WSA_IO_PENDING != errorCode)
			return false;
	}
	return true;
}

bool NetworkUtil::SetReuseAddr(SOCKET socket, bool optVal)
{
	const int optLen = sizeof(optVal);

	return (SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&optVal), optLen)) ? true : false;
}

bool NetworkUtil::SetLinger(SOCKET socket, int onOff, int linger)
{
	struct Option
	{
		int l_onoff;
		int l_linger;
	} optVal{ onOff, linger };
	const int optLen = sizeof(optVal);

	return (SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&optVal), optLen)) ? true : false;
}

bool NetworkUtil::BindAnyIpAddress(SOCKET socket)
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(0);

	return (SOCKET_ERROR != bind(socket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr))) ? true : false;
}

bool NetworkUtil::UpdateAcceptContext(SOCKET listenSocket, SOCKET acceptSocket)
{
	const int optLen = sizeof(SOCKET);

	return (SOCKET_ERROR != setsockopt(acceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&listenSocket), optLen)) ? true : false;
}

void NetworkUtil::Encrypt(uint8_t* buf, int32_t len, EncryptionKey encryptionKey)
{
	uint8_t* key = reinterpret_cast<uint8_t*>(&encryptionKey);

	for (int32_t i = 0; i < len; ++i)
	{
		buf[i] ^= ++key[i & 0b00000011];
	}
}
