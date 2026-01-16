// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/AppBase/AppBase.h"
#include "ServerEngine/Util/CommandDispatcher/CommandDispatcherWithResult.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TeeWstreambuf: std::wcout 출력을 콘솔과 로그 큐에 동시에 기록하기 위한 streambuf
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TeeWstreambuf : public std::wstreambuf
{
private:
	std::wstreambuf* mOriginalBuf = nullptr;
	std::wstring mLineBuffer;
	bool mIsLogging = false;  // 재진입(중복 로그) 방지 플래그

public:
	explicit TeeWstreambuf(std::wstreambuf* originalBuf)
		: mOriginalBuf(originalBuf)
	{
	}

	~TeeWstreambuf() override = default;

protected:
	int_type overflow(int_type ch) override
	{
		if (ch == traits_type::eof())
			return traits_type::eof();

		// 원본 콘솔로 출력
		if (mOriginalBuf)
			mOriginalBuf->sputc(static_cast<wchar_t>(ch));

		// 로그 출력 중에는 재진입을 막는다.
		// (LogQueue::Push 내부에서 다시 출력이 발생하면 무한 재귀가 될 수 있음)
		if (mIsLogging)
			return ch;

		// 개행 시점에 한 줄 단위로 로그 큐에 Push
		if (ch == L'\n')
		{
			if (!mLineBuffer.empty() && gLogQueue)
			{
				mIsLogging = true;
				gLogQueue->Push(WHITE, L"TEST", 0, L"{}", mLineBuffer);
				mIsLogging = false;
			}
			mLineBuffer.clear();
		}
		else
		{
			mLineBuffer += static_cast<wchar_t>(ch);
		}

		return ch;
	}

	int sync() override
	{
		// 로그 출력 중에는 원본 버퍼만 flush
		if (mIsLogging)
		{
			if (mOriginalBuf)
				return mOriginalBuf->pubsync();
			return 0;
		}

		// 남아있는 버퍼를 한 줄 로그로 flush
		if (!mLineBuffer.empty() && gLogQueue)
		{
			mIsLogging = true;
			gLogQueue->Push(WHITE, L"TEST", 0, L"{}", mLineBuffer);
			mIsLogging = false;
			mLineBuffer.clear();
		}

		if (mOriginalBuf)
			return mOriginalBuf->pubsync();

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TestAppBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TestAppBase : public AppBase
{
private:
	CommandDispatcherWithResult<std::wstring> mCommandDispatcher;
	
	// wcout 로깅 구성요소
	std::unique_ptr<TeeWstreambuf> mTeeStreambuf;
	std::wstreambuf* mOriginalWcoutBuf = nullptr;

public:
	explicit TestAppBase(AppArg appArg);
	~TestAppBase() override;

public:
	void Run() override;
	std::wstring Dispatch(const std::wstring& command);

private:
	std::wstring OnCommand_help(ArgList& argList);

public:
	virtual void OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher) = 0;
	virtual void OnStarted() = 0;
	virtual void OnUpdate() = 0;
};
