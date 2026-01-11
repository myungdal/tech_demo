// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Time/Cron.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CronWorker - Unix Cron 표현식 기반 스케줄 실행기
//
// 설계 목표:
// - Cron 표현식으로 복잡한 반복 스케줄 정의
// - 취소 가능한 스케줄 작업 관리
//
// Cron 표현식 예시:
// - "0 0 * * *"     : 매일 자정
// - "*/5 * * * *"   : 5분마다
// - "0 9 * * 1-5"   : 평일 오전 9시
// - "0 0 1 * *"     : 매월 1일 자정
//
// 실행 흐름:
// 1. RunOnCron() 호출 시 다음 실행 시간 계산
// 2. TimerWorker에 지연 실행 등록
// 3. 실행 시점에 작업 수행 후 다음 스케줄 재등록
// 4. CancelCron() 호출 시 취소 플래그 설정
//
// [WHY] 왜 별도 CronWorker인가?
// - Cron 스케줄은 반복 실행이므로 취소 메커니즘 필요
// - 취소된 핸들 집합(mCancelSet)으로 효율적 취소 처리
//
// 사용 예시:
// auto handle = WorkerManager::RunOnCron(now, cronExpr, worker, owner, &Owner::DailyReset);
// WorkerManager::CancelCron(handle);  // 취소
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WorkerTaskNode;

class CronWorker
{
	friend class WorkerManager;

private:
	using CronHandleSet = std::unordered_set<CronHandle>;

	Lock mLock;
	CronHandleSet mCancelSet;

public:
	void CancelCron(CronHandle cronHandle);

private:	
	bool CheckCancelSet(CronHandle cronHandle);

private:
	void OnCron_timer(std::weak_ptr<CronWorker> cronWorkerWeakPtr, time_t prev, const cron::cronexpr& cex, WorkerTaskNode* task, CronHandle cronHandle);
};

inline std::atomic<CronHandle> gCurrentCronHandle = 0;
inline std::shared_ptr<CronWorker> gCronWorker = nullptr;
