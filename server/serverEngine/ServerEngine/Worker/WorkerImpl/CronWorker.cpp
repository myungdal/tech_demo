// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ServerEngine/PCH/pch_serverEngine.h"
#include "CronWorker.h"

// [WHY] 우산 모듈 import는 C1605(obj 4GB 초과)로 빌드 실패 가능.
// 필요한 헤더만 직접 include한다.
#include "ServerEngine/Worker/WorkerManager.h"
#include "ServerEngine/Lock/WriteLock.h"


void CronWorker::CancelCron(CronHandle cronHandle)
{
	WriteLock lock(mLock);

	mCancelSet.emplace(cronHandle);
}

bool CronWorker::CheckCancelSet(CronHandle cronHandle)
{
	WriteLock lock(mLock);

	if (true == mCancelSet.empty())
		return false;

	auto it = mCancelSet.find(cronHandle);
	if (mCancelSet.end() == it)
		return false;

	mCancelSet.erase(it);

	return true;
}

void CronWorker::OnCron_timer(std::weak_ptr<CronWorker> cronWorkerWeakPtr, time_t prev, const cron::cronexpr& cex, WorkerTaskNode* task, CronHandle cronHandle)
{
	auto cronWorkerPtr = cronWorkerWeakPtr.lock();
	if (!cronWorkerPtr)
		return;

	if (true == CheckCancelSet(cronHandle))
		return;

	task->Run();

	std::time_t now = cron::cron_next(cex, prev);
	std::time_t next = cron::cron_next(cex, now);
	ClockMs after = (next > now) ? ((next - now) * 1000ms) : 0ms;

	WorkerManager::RunOnTimer(after, gTimerWorker->GetTimerWorker(), this, &CronWorker::OnCron_timer, cronWorkerWeakPtr, tClock.GetGlobalNowTt(), cex, task, cronHandle);
}
