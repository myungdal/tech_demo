// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerTask/WorkerTask.h"
#include "ServerEngine/Util/MwsrQueue/MwsrQueue.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ThreadWorker - 특정 스레드 전용 작업 실행기
//
// 설계 목표:
// - 특정 스레드 ID에 바인딩된 작업 실행
// - Worker와 독립적으로 스레드 단위 작업 관리
//
// 사용 시나리오:
// 1. RunOnThread(threadId, ...): 지정된 스레드에서 실행
// 2. RunOnAnyThread(...): 라운드 로빈으로 스레드 선택
// 3. RunOnEachAllThread(...): 모든 스레드에 브로드캐스트
//
// [WHY] Worker와 별도로 ThreadWorker가 필요한 이유:
// - Worker: 논리적 실행 단위 (세션, 채널 등)
// - ThreadWorker: 물리적 스레드 바인딩 (스레드 로컬 작업)
// - 예: 스레드 로컬 캐시 갱신, 전역 상태 동기화
//
// 구현 구조:
// - 각 Worker 스레드마다 하나의 ThreadWorker 인스턴스
// - gThreadWorkerPtrList[threadId]로 접근
// - gAnyThreadId로 라운드 로빈 스레드 선택
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WorkerTaskNode;

class ThreadWorker
{
	friend class WorkerManager;

private:
	MwsrQueue<WorkerTaskNode> mTimerTaskTable;
	
public:
	ThreadWorker() = default;
	void Run();

private:
	template<typename _Owner, typename _Function, typename... _Args>
	void Push(_Owner* owner, _Function&& function, _Args&&... args)
	{
		using TaskType = WorkerTask<Worker, _Owner, _Function, _Args...>;

		TaskType* task = TaskType::Acquire();
		task->Initialize(nullptr, std::forward<_Owner*>(owner), std::forward<_Function>(function), std::forward<_Args>(args)...);
		PushTask(task);
	}

	void PushTask(WorkerTaskNode* task);
};

inline std::atomic<ThreadId> gAnyThreadId = 0;

#if 0
#include "ServerEngine/Worker/WorkerImpl/ThreadWorker.h"

class Test
{
public:
	Lock mLock;

	int64_t mValue = 0;
	int64_t mValue0 = 0;
	int64_t mValue1 = 0;

	std::tuple<int, WriteLock> GetWriteLock()
	{
		WriteLock lock(mLock);

		return std::tuple{ 1, std::move(lock) };
	}

	void Run0()
	{
		auto [value, _] = GetWriteLock();

		mValue += 100;
		++mValue0;

		WorkerManager::RunOnThread(tThreadId, this, &Test::Run0);
	}

	void Run1()
	{
		auto [value, _] = GetWriteLock();

		mValue -= 100;
		++mValue1;

		WorkerManager::RunOnThread(tThreadId, this, &Test::Run1);
	}
};
Test gTest;

WorkerManager::RunOnThread(0, &gTest, &Test::Run0);
WorkerManager::RunOnThread(1, &gTest, &Test::Run1);
#endif
