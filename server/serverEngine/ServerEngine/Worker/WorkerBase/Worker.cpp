// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Worker.h"


Worker::Worker()
{
}
Worker::~Worker()
{	
}
void Worker::PushTask(WorkerTaskNode* task)
{
	if (++mTaskCount == 1)
	{
		mTaskQueue.Push(task);

		TryWork();
	}
	else
	{
		mTaskQueue.Push(task);
	}
}

void Worker::PushTaskAsyncForced(WorkerTaskNode* task)
{
	if (++mTaskCount == 1)
	{
		mTaskQueue.Push(task);

		gCoWorker->PushWorker(shared_from_this());
	}
	else
	{
		mTaskQueue.Push(task);
	}
}

void Worker::TryWork()
{
	// 이 Thread 가 처리하고 있는 Worker 가 있으면, 빠져나간다
	if (nullptr != tWorkingWorker)
	{
		gCoWorker->PushWorker(shared_from_this());

		return;
	}

	// 이 Thread 가 점유중인 Lock 이 있으면 빠져나간다
	if (0 != tLockedCount)
	{
		gCoWorker->PushWorker(shared_from_this());

		return;
	}

	// 이 Thread 가 맡아서 처리한다
	tWorkingWorker = shared_from_this();

	size_t popCount = 0;
	size_t doneCount = 0;
	WorkerTaskNode* next = mTaskQueue.Pop(OUT popCount);
	while (next)
	{
		next->Execute();

		WorkerTaskNode* temp = next->GetNext();

		next->Dispose();
		next = temp;

		++doneCount;
	}
	_ASSERT_CRASH(popCount == doneCount);

	if (mTaskCount.fetch_sub(doneCount) != doneCount)
	{
		gCoWorker->PushWorker(shared_from_this());
	}

	tWorkingWorker = nullptr;
}
