// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "WorkerThreadManager.h"

import ServerEngineWorker;
import ServerEngineNetwork;


WorkerThreadManager::WorkerThreadManager(ThreadId workerThreadCount, std::shared_ptr<class Iocp> iocp)
	:
	mIocp(iocp)
{
	gWorkerThreadCount = workerThreadCount;

	// Thread 별 ThreadWorker 생성
	gThreadWorkerPtrList.reserve(workerThreadCount);
	for (ThreadId i = 0; i < workerThreadCount; ++i)
	{
		std::shared_ptr<ThreadWorker> worker = std::make_shared<ThreadWorker>();
		gThreadWorkerPtrList.push_back(worker);
	}

	// WorkerThread 생성 (jthread 기반, 생성 시 suspended 상태)
	mWorkerThreadPtrList.reserve(workerThreadCount);
	for (ThreadId i = 0; i < workerThreadCount; ++i)
	{
		std::shared_ptr<WorkerThread> workerThreadPtr = std::make_shared<WorkerThread>(i, iocp);
		mWorkerThreadPtrList.push_back(workerThreadPtr);
	}
}

WorkerThreadManager::~WorkerThreadManager()
{
	// 소멸자에서 Stop 호출하여 모든 스레드 정리
	Stop();
}

void WorkerThreadManager::CreateIocpThread()
{
	if (mIocpThreadPtr)
		return;

	// 모든 WorkerThread가 네트워크 패킷을 대기할 경우를 대비하여 IocpThread 하나 생성.
	mIocpThreadPtr = std::make_shared<IocpThread>(mIocp);
}

void WorkerThreadManager::Start()
{
	for (const auto& workerThreadPtr : mWorkerThreadPtrList)
	{
		if (workerThreadPtr)
			workerThreadPtr->Start();
	}

	if (mIocpThreadPtr)
		mIocpThreadPtr->Start();
}

void WorkerThreadManager::Stop()
{
	// 모든 스레드에 종료 요청
	for (auto& workerThreadPtr : mWorkerThreadPtrList)
	{
		if (workerThreadPtr)
			workerThreadPtr->RequestStop();
	}

	if (mIocpThreadPtr)
		mIocpThreadPtr->RequestStop();

	// 모든 스레드 종료 대기
	for (auto& workerThreadPtr : mWorkerThreadPtrList)
	{
		if (workerThreadPtr)
			workerThreadPtr->Join();
	}

	if (mIocpThreadPtr)
		mIocpThreadPtr->Join();
}
