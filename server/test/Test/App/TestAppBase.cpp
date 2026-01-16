// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;

#include "TestAppBase.h"


TestAppBase::TestAppBase(AppArg appArg)
	:
	AppBase(AppType::TEST, appArg)
{
	Initialize();
	// Test 역할 준비
	{
		mCommandDispatcher.Register(
			L"help", L"도움말",
			std::bind(&TestAppBase::OnCommand_help, this, std::placeholders::_1)
		);
	}

	// std::wcout을 로그 파일에도 기록되도록 래핑
	mOriginalWcoutBuf = std::wcout.rdbuf();
	mTeeStreambuf = std::make_unique<TeeWstreambuf>(mOriginalWcoutBuf);
	std::wcout.rdbuf(mTeeStreambuf.get());
}

TestAppBase::~TestAppBase()
{
	// 원래 streambuf 복원
	if (mOriginalWcoutBuf)
	{
		std::wcout.rdbuf(mOriginalWcoutBuf);
		mOriginalWcoutBuf = nullptr;
	}
}

void TestAppBase::Run()
{
	OnInitialize(mCommandDispatcher);

	TlsCommonBuffer::ResetTlsCommonBufferIdx();
	tClock.Update();

	OnStarted();

	// OnStarted()에서 출력된 내용을 로그에 기록
	std::wcout.flush();
	UpdateLog();

	while (true)
	{
		TlsCommonBuffer::ResetTlsCommonBufferIdx();
		tClock.Update();

		std::wstring command;
		std::getline(std::wcin, command);

		std::wstring result = Dispatch(command);
		if (result == L"")
			break;

		std::wcout << result;

		OnUpdate();

		std::wcout.flush();
		UpdateLog();

		_YIELD_APP;
	}

	// 종료 시 남은 로그 Flush
	std::wcout.flush();
	UpdateLog();
}

std::wstring TestAppBase::Dispatch(const std::wstring& command)
{
	std::wstring result = mCommandDispatcher.Dispatch(command);

	return result;
}

std::wstring TestAppBase::OnCommand_help(MAYBE_UNUSED ArgList& argList)
{
	std::wstring result;
	result += L"\n";
	result += L"* Test 명령:\n";
	for (const auto& [key, handler] : mCommandDispatcher.GetHandlerMap())
	{
		result += L"\t";
		result += std::get<0>(handler);
		result += L"\t";
		result += std::get<1>(handler);
		result += L"\n";
	}
	result += L"\n";
	return result;
}
