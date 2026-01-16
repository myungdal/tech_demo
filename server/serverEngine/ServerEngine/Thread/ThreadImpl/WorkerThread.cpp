// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "WorkerThread.h"

import ServerEngineWorker;
import ServerEngineNetwork;


WorkerThread::WorkerThread(ThreadId threadId, std::shared_ptr<Iocp> iocp)
	: 
	Thread(threadId),
	mIocp(iocp)
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::Run(std::stop_token stopToken)
{
	// 이 루프는 워커 스레드의 메인 핫패스라서,
	// thread_local(TLS) 접근을 반복하지 않도록 필요한 값/참조를 루프 밖에서 캐싱한다.
	const ThreadId threadId = mThreadId;
	tThreadId = threadId;

	auto& threadWorker = gThreadWorkerPtrList[threadId];
	TlsClock& clock = tClock;

	// [WHY] stop_requested() 체크로 graceful shutdown 지원
	// - RequestStop() 호출 시 루프 종료
	// - jthread 소멸 시 자동으로 stop 요청됨
	while (!stopToken.stop_requested())
	{
		TlsCommonBuffer::ResetTlsCommonBufferIdx();

		clock.Update();

		mIocp->DispatchCompletion(1);

		threadWorker->Run();

		gCoWorker->Run();
	}

	TlsCommonBuffer::ReleaseTlsCommonBuffer();
}
