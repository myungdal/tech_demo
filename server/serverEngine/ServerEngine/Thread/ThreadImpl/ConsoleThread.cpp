// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ConsoleThread.h"

import ServerEngineWorker;
import ServerEngineNetwork;


ConsoleThread::ConsoleThread(const OnShellCommand& onCommand)
	:
	Thread(INVALID_THREAD_ID),
	mOnCommand(onCommand)
{
	AllocConsole();
	mInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	mOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

ConsoleThread::~ConsoleThread()
{
}

void ConsoleThread::Run(std::stop_token stopToken)
{
	std::this_thread::sleep_for(1000ms);

	std::wcout << L">";

	wchar_t buffer[256];
	DWORD read;

	// [WHY] stop_requested() 체크로 graceful shutdown 지원
	while (!stopToken.stop_requested())
	{
		TlsCommonBuffer::ResetTlsCommonBufferIdx();

		tClock.Update();

		if (TRUE == ReadConsole(mInputHandle, buffer, 200, &read, nullptr))
		{
			// 종료 요청 확인 (ReadConsole 블로킹 후)
			if (stopToken.stop_requested())
				break;

			if (read > 2)
				buffer[read - 2] = 0;
			else
				buffer[read] = 0;

			std::promise<std::wstring> promise;
			std::future<std::wstring> future = promise.get_future();

			mOnCommand(promise, buffer);

			Print(future.get().c_str());
		}

		_YIELD_CONSOLE_THREAD;
	}
}

void ConsoleThread::Print(const wchar_t* text) const
{
	const DWORD length = static_cast<DWORD>(std::wcslen(text));
	DWORD written;
	WriteConsole(mOutputHandle, text, length, &written, nullptr);
}
