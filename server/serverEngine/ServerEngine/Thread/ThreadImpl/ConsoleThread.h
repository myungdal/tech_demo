// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Thread/ThreadBase/Thread.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ConsoleThread - 콘솔 입력 처리 스레드
//
// [WHY] stop_token 기반 graceful shutdown
// - ReadConsole이 블로킹되지만 stop 요청 시 안전하게 종료
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ConsoleThread : public Thread
{
public:
	using OnShellCommand = std::function<void(std::promise<std::wstring>&, const wchar_t*)>;

private:
	HANDLE mInputHandle = INVALID_HANDLE_VALUE;
	HANDLE mOutputHandle = INVALID_HANDLE_VALUE;

	OnShellCommand mOnCommand = nullptr;

public:
	explicit ConsoleThread(const OnShellCommand& onCommand);
	virtual ~ConsoleThread();

protected:
	void Run(std::stop_token stopToken) final;

public:
	void Print(const wchar_t* text) const;
};
