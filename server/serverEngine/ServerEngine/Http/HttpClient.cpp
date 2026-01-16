// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "HttpClient.h"


HttpClient::HttpClient(const wchar_t* agent, DWORD timeout)
{
	mOpen = WinHttpOpen(
		agent, 
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, 
		WINHTTP_NO_PROXY_NAME, 
		WINHTTP_NO_PROXY_BYPASS, 
		0
	);
	if (nullptr == mOpen)
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return;
	}
	DWORD connTimeout = timeout;
	DWORD sendTimeout = timeout;
	DWORD recvTimeout = timeout;
	if ((FALSE == WinHttpSetOption(mOpen, WINHTTP_OPTION_CONNECT_TIMEOUT, &connTimeout, sizeof(connTimeout))) ||
		(FALSE == WinHttpSetOption(mOpen, WINHTTP_OPTION_SEND_TIMEOUT, &sendTimeout, sizeof(sendTimeout))) ||
		(FALSE == WinHttpSetOption(mOpen, WINHTTP_OPTION_RECEIVE_TIMEOUT, &recvTimeout, sizeof(recvTimeout))))
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		WinHttpCloseHandle(mOpen);
		mOpen = nullptr;
		return;
	}
}
HttpClient::~HttpClient()
{
	Stop();
	if (nullptr != mOpen)
	{
		WinHttpCloseHandle(mOpen);
		mOpen = nullptr;
	}
}
bool HttpClient::Get(bool secure, const wchar_t* host, const wchar_t* path)
{
	if (false == IsOpen())
		return false;

	Stop();
	
	INTERNET_PORT port;
	if (secure)
		port = INTERNET_DEFAULT_HTTPS_PORT;
	else
		port = INTERNET_DEFAULT_HTTP_PORT;

	mConn = WinHttpConnect(mOpen, host, port, 0);
	if(nullptr == mConn)
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return false;
	}

	const DWORD flag = (true == secure) ? WINHTTP_FLAG_SECURE : 0;
	mReq = WinHttpOpenRequest(mConn, L"GET", path, L"HTTP/1.1", nullptr, nullptr, flag);
	if(nullptr == mReq)
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return false;
	}

	if(FALSE == WinHttpSendRequest(mReq, nullptr, 0, nullptr, 0, 0, 0))
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return false;
	}

	return true;
}

bool HttpClient::PostStart(bool secure, const wchar_t* host, const wchar_t* path)
{
	if (false == IsOpen())
		return false;
	
	Stop();

	const INTERNET_PORT port = (true == secure) ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
	mConn = WinHttpConnect(mOpen, host, port, 0);
	if(nullptr == mConn)
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return false;
	}

	DWORD flag = (true == secure) ? WINHTTP_FLAG_SECURE : 0;
	mReq = WinHttpOpenRequest(mConn, L"POST", path, L"HTTP/1.1", nullptr, nullptr, flag);
	if(nullptr == mReq)
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return false;
	}

	return true;
}

bool HttpClient::PostPushHeader(const wchar_t* header)
{
	if (FALSE == WinHttpAddRequestHeaders(mReq, header, DWORD(-1), WINHTTP_ADDREQ_FLAG_ADD))
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return false;
	}

	return true;
}

bool HttpClient::PostEnd(const uint8_t* body, size_t bodySize)
{
	const DWORD size = static_cast<DWORD>(bodySize);
	if (FALSE == WinHttpSendRequest(mReq, WINHTTP_NO_ADDITIONAL_HEADERS, DWORD(-1), (LPVOID)body, size, size, 0))
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return false;
	}

	return true;
}

size_t HttpClient::GetResponse(uint8_t* buf, size_t bufSize)
{
	if (false == IsOpen())
		return 0;

	if (FALSE == WinHttpReceiveResponse(mReq, NULL))
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return false;
	}

	DWORD statusCode;
	DWORD statusCodeLength = sizeof(statusCode);
	if(FALSE == WinHttpQueryHeaders(
		mReq, (WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER), nullptr, &statusCode, &statusCodeLength, nullptr))
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return 0;
	}

	if (HTTP_STATUS_OK != statusCode)
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return 0;
	}

	DWORD availableSize = 0;
	if (FALSE == WinHttpQueryDataAvailable(mReq, &availableSize))
	{
		_DEBUG_LOG(RED, L"{}", GetLastError());
		return 0;
	}

	DWORD offset = 0;	
	while (0 < availableSize)
	{
		if (bufSize <= (size_t(offset) + size_t(availableSize)))
		{
			_DEBUG_LOG(RED, L"buffer overflow");
			return 0;
		}

		DWORD readSize = 0;
		if (FALSE == WinHttpReadData(mReq, buf + offset, availableSize, &readSize))
		{
			_DEBUG_LOG(RED, L"{}", GetLastError());
			return 0;
		}

		offset += readSize;

		if (FALSE == WinHttpQueryDataAvailable(mReq, &availableSize))
		{
			_DEBUG_LOG(RED, L"{}", GetLastError());
			return 0;
		}
	}

	buf[offset] = 0;

	return offset;
}

void HttpClient::Stop()
{
	if(nullptr != mReq)
	{
		WinHttpCloseHandle(mReq);
		mReq = nullptr;
	}
	if(nullptr != mConn)
	{
		WinHttpCloseHandle(mConn);
		mConn = nullptr;
	}
}
