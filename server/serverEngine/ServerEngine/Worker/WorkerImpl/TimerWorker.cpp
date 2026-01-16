// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ServerEngine/PCH/pch_serverEngine.h"
#include "TimerWorker.h"

// [WHY] 우산 모듈 import는 C1605(obj 4GB 초과)로 빌드 실패 가능.
// 필요한 헤더만 직접 include한다.
#include "ServerEngine/Worker/WorkerManager.h"
#include "ServerEngine/TLS/TlsGlobals.h"


TimerWorker::TimerWorker()
	:
	mTimerWorker(std::make_shared<Worker>())
{
	mCurrClockMs = ClockMs(tClock.GetLocalNowMs().count() & ~SLOT_TIME_DELTA_MASK);
	mCurrSlotIndex = (mCurrClockMs.count() >> SLOT_TIME_DELTA_EXOINENT) & SLOT_SIZE_MASK;
}
void TimerWorker::Resume()
{
	WorkerManager::Run(mTimerWorker, this, &TimerWorker::Flush_async);
}

void TimerWorker::Push_async(ClockMs delay, WorkerTaskNode* task)
{
	_ASSERT_CRASH(tWorkingWorker == mTimerWorker);

	const ClockMs time = (mCurrClockMs + delay);

	if (delay > MAX_SLOT_TIME_DELTA)
	{
		mTimerTaskTable.emplace(time, task);
	}
	else
	{
		size_t slotIndex = (time.count() >> SLOT_TIME_DELTA_EXOINENT) & SLOT_SIZE_MASK;
		if (slotIndex == mCurrSlotIndex)
			slotIndex = ((slotIndex + 1) & SLOT_SIZE_MASK);

		SlotTaskList& slotTaskList = mSlotTaskTable[slotIndex];
		slotTaskList.emplace_back(task);
	}	
}

void TimerWorker::Flush_async()
{
	_ASSERT_CRASH(tWorkingWorker == mTimerWorker);

	const ClockMs& clockMs = tClock.GetLocalNowMs();

	while ((mCurrClockMs + SLOT_TIME_DELTA) <= clockMs)
	{
		mCurrClockMs += SLOT_TIME_DELTA;
		mCurrSlotIndex = (mCurrSlotIndex + 1) & SLOT_SIZE_MASK;

		SlotTaskList& slotTaskList = mSlotTaskTable[mCurrSlotIndex];

		if (false == slotTaskList.empty())
		{
			for (WorkerTaskNode* task : slotTaskList)
			{
				task->Run();
			}

			slotTaskList.clear();
		}
	}

	while (false == mTimerTaskTable.empty())
	{
		auto& [timePoint, task] = mTimerTaskTable.top();

		if (clockMs < timePoint)
			break;

		task->Run();

		mTimerTaskTable.pop();
	}

	WorkerManager::Run(mTimerWorker, this, &TimerWorker::Flush_async);
}
