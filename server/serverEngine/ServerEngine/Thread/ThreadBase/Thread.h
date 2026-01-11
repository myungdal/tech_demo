// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// Thread.h - std::jthread 기반 스레드 기본 클래스
#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <stop_token>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread
// 
// [WHY] std::jthread 기반 리팩토링
// - C++20 std::jthread로 자동 join 및 cooperative cancellation 지원
// - std::stop_token으로 안전한 스레드 종료 패턴
// - condition_variable로 효율적인 시작/대기 제어
//
// 사용 패턴:
// 1. 생성자에서 스레드 생성 (suspended 상태로 대기)
// 2. Start()로 스레드 시작 신호
// 3. 파생 클래스의 Run(std::stop_token)에서 실제 로직 수행
// 4. 소멸 시 자동으로 stop_token 통해 종료 요청 + join
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Thread
{
protected:
	// 스레드 ID
	ThreadId mThreadId = INVALID_THREAD_ID;

private:
	// jthread 핸들 - 자동 join/stop_token 지원
	std::jthread mThread;

	// 시작 동기화용 mutex/cv
	std::mutex mStartMutex;
	std::condition_variable mStartCv;
	bool mStarted = false;

public:
	// 생성자 - 스레드를 suspended 상태로 생성
	explicit Thread(ThreadId threadId);

	// 가상 소멸자 - jthread가 자동으로 stop 요청 및 join
	virtual ~Thread();

	// 스레드 시작 (Resume 대신 Start로 명명 변경 - 의미가 더 명확)
	void Start();

	// 스레드 종료 요청 (cooperative)
	void RequestStop();

	// 스레드 종료 대기
	void Join();

	// 스레드 ID 반환
	[[nodiscard]] ThreadId GetThreadId() const { return mThreadId; }

protected:
	// 스레드 실행 함수 (파생 클래스에서 구현)
	// [WHY] stop_token을 받아 cooperative cancellation 지원
	virtual void Run(std::stop_token stopToken) = 0;

private:
	// 스레드 진입점 - TLS 초기화 후 Run 호출
	void ThreadEntry(std::stop_token stopToken);
};
