// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ThreadWorker.h"


void ThreadWorker::Run()
{
	size_t popCount = 0;
	size_t doneCount = 0;
	WorkerTaskNode* next = mTimerTaskTable.Pop(OUT popCount);
	while (next)
	{
		next->ExecuteOnThread();

		WorkerTaskNode* temp = next->GetNext();

		next->Dispose();
		next = temp;

		++doneCount;
	}
	_ASSERT_CRASH(popCount == doneCount);
}

void ThreadWorker::PushTask(WorkerTaskNode* task)
{
	mTimerTaskTable.Push(task);
}
