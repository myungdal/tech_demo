// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// Iocp.h - IOCP (I/O Completion Port) 래퍼
#pragma once

#include "ServerEngine/Network/NetworkManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Iocp
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// IOCP 래퍼 클래스 (비동기 I/O 완료 처리)
class Iocp final
{
private:
	// IOCP 핸들
	HANDLE mIocpHandle = INVALID_HANDLE_VALUE;

public:
	// 생성자 (IOCP 생성)
	Iocp();
	
	// 소멸자 (IOCP 해제)
	virtual ~Iocp();

	// 완료 이벤트 디스패치 (타임아웃 지정)
	void DispatchCompletion(DWORD timeout) const;

private:
	friend class SocketBase;
	
	// 소켓을 IOCP에 바인딩
	void BindCompletionHandle(SOCKET socket, class CompletionHandler* completionHandler) const;
};
