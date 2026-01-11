// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Thread.h"

#include "Common/Random/Mt19937Random/Mt19937Random32.h"
#include "Common/Random/Mt19937Random/Mt19937Random64.h"


Thread::Thread(ThreadId threadId)
	:
	mThreadId(threadId)
{
	// jthread 생성 - 즉시 ThreadEntry 진입하지만 내부에서 condition_variable로 대기
	mThread = std::jthread([this](std::stop_token stopToken) {
		ThreadEntry(stopToken);
	});
}

Thread::~Thread()
{
	// jthread 소멸자가 자동으로 stop 요청 및 join 수행
	// 명시적으로 RequestStop() 호출하지 않아도 됨
}

void Thread::Start()
{
	{
		std::lock_guard lock(mStartMutex);
		mStarted = true;
	}
	mStartCv.notify_one();
}

void Thread::RequestStop()
{
	// 먼저 시작 신호를 보내 대기 중인 스레드가 깨어나게 함
	Start();

	// jthread의 stop 토큰에 종료 요청
	mThread.request_stop();
}

void Thread::Join()
{
	if (mThread.joinable())
	{
		mThread.join();
	}
}

void Thread::ThreadEntry(std::stop_token stopToken)
{
	// TLS 초기화 (기존 로직 유지)
	tRandomEngine32 = std::make_shared<Mt19937Random32>();
	tRandomEngine64 = std::make_shared<Mt19937Random64>();

	// 시작 신호 대기 (기존 CREATE_SUSPENDED + Resume 패턴 대체)
	{
		std::unique_lock lock(mStartMutex);
		mStartCv.wait(lock, [this, &stopToken] {
			return mStarted || stopToken.stop_requested();
		});
	}

	// 종료 요청이 있으면 Run 실행하지 않고 종료
	if (stopToken.stop_requested())
	{
		return;
	}

	// 파생 클래스의 Run 실행
	Run(stopToken);
}
