// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// HttpClient.h - HTTP 클라이언트 (WinHTTP 기반)
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HttpClient
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// HTTP 클라이언트 (WinHTTP API 래퍼)
class HttpClient
{
private:
	// WinHTTP 세션 핸들
	HINTERNET mOpen = nullptr;
	
	// WinHTTP 연결 핸들
	HINTERNET mConn = nullptr;
	
	// WinHTTP 요청 핸들
	HINTERNET mReq = nullptr;

public:
	// 생성자 (User-Agent 및 타임아웃 설정)
	explicit HttpClient(const wchar_t* agent = L"agent", DWORD timeout = 5000);
	
	// 소멸자
	virtual ~HttpClient();

	// HTTP GET 요청
	bool Get(bool secure, const wchar_t* host, const wchar_t* path);

	// HTTP POST 요청 시작
	bool PostStart(bool secure, const wchar_t* host, const wchar_t* path);
	
	// POST 헤더 추가
	bool PostPushHeader(const wchar_t* header);
	
	// POST 요청 완료 (본문 전송)
	bool PostEnd(const uint8_t* body, size_t bodySize);

	// 응답 데이터 읽기
	size_t GetResponse(uint8_t* buf, size_t bufSize);
	
	// 연결 종료
	void Stop();

	// 연결 상태 확인
	bool IsOpen() const noexcept { return (nullptr == mOpen) ? false : true; }
};

#if 0
#include <fstream>
#include <iostream>

HttpClient test;
{
	char* buf = new char[1024 * 1024];
	test.Get(true, L"postman-echo.com", L"get?foo1=bar1&foo2=bar2");
	test.GetResponse((uint8_t*)buf, sizeof(char) * 1024 * 1024);
	{
		std::ofstream file(L"get.txt");
		file << buf;
	}
	delete[] buf;
}
{
	char* buf = new char[1024 * 1024];
	test.PostStart(true, L"postman-echo.com", L"post");
	test.PostPushHeader(L"Content-Type:application/json");
	//std::string body("{\"email\":\"a@b.c\"}");
	std::string body(R"({"email":"a@b.c"})"); // LR"()" R"()"
	test.PostEnd((uint8_t*)body.c_str(), body.length());
	test.GetResponse((uint8_t*)buf, sizeof(char) * 1024 * 1024);
	{
		std::ofstream file(L"post.txt");
		file << buf;
	}
	delete[] buf;
}
#endif
