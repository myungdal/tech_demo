// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerImpl/CronWorker.h"
#include "ServerEngine/Worker/WorkerImpl/ThreadWorker.h"
#include "ServerEngine/Worker/WorkerImpl/TimerWorker.h"
#include "ServerEngine/Worker/WorkerBase/Worker.h"
#include "ServerEngine/Worker/WorkerTask/WorkerTask.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WorkerManager - 비동기 작업 스케줄링 통합 관리자
//
// 설계 목표:
// - 게임 서버의 비동기 작업을 단일 인터페이스로 통합 관리
// - 함수 호출 큐잉 기반의 Lock-free 동시성 구현
// - 타이머/Cron 기반 지연 실행 및 반복 스케줄링 지원
//
// 핵심 개념:
// - Worker: 함수 호출 정보 큐를 소유한 논리적 실행 단위
// - Task: Worker에 전달되는 개별 작업 (멤버 함수 포인터 + 인자 캡처)
// - 동기 실행 최적화: 같은 Worker 내에서는 직접 호출로 컨텍스트 스위칭 최소화
//
// 사용 예시:
// - WorkerManager::Run(workerPtr, owner, &Owner::Method, args...)
// - WorkerManager::RunOnTimer(1000ms, workerPtr, owner, &Owner::DelayedMethod)
// - WorkerManager::RunOnCron(now, cronExpr, workerPtr, owner, &Owner::ScheduledMethod)
//
// [WHY] 왜 이 구조인가?
// - 전통적인 Lock 기반 동기화 대신 함수 호출 큐잉 방식 채택 (데드락 방지, 성능 향상)
// - 템플릿 기반 설계로 타입 안전성 보장 및 런타임 오버헤드 최소화
// - 멤버 함수 포인터 + 인자 캡처로 다양한 호출 패턴 지원
//
// [PERF] 성능 특성:
// - Lock-free 큐(MWSR)로 작업 추가 시 경합 최소화
// - 메모리 풀링(DynamicPool)으로 Task 할당/해제 비용 절감
// - 타이머는 슬롯 기반(16ms 단위) + 우선순위 큐 하이브리드 구조
//
// 스레드 모델:
// - Worker 작업은 반드시 한 번에 하나의 스레드에서만 실행됨
// - tWorkingWorker(TLS)로 현재 스레드가 처리 중인 Worker 추적
// - 중첩 실행 방지: 이미 작업 중인 Worker에 새 작업 도착 시 CoWorker가 대리 실행
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WorkerManager
{
public:
	// Run - Worker에 작업 전달 (동기 최적화 포함)
	// 현재 스레드가 이미 해당 Worker를 처리 중이면 동기 실행, 아니면 비동기 큐잉
	// [WHY] 같은 Worker 내 연속 호출 시 컨텍스트 스위칭 오버헤드 제거
	template<typename _Worker, typename _Owner, typename _Function, typename... _Args>
	static void Run(std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args);

	// RunAsyncForced - 항상 비동기로 실행 (동기 최적화 스킵)
	// [WHY] 재귀 호출 방지가 필요하거나, 호출 순서 보장이 필요할 때 사용
	template<typename _Worker, typename _Owner, typename _Function, typename... _Args>
	static void RunAsyncForced(std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args);

	// RunOnTimer - 지정된 시간 후 실행
	// delay: 지연 시간 (밀리초)
	// [PERF] 16ms 이하는 슬롯 배열, 이상은 우선순위 큐 사용
	template<typename _Worker, typename _Owner, typename _Function, typename... _Args>
	static void RunOnTimer(ClockMs delay, std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args);

	// RunOnEachAllThread - 모든 Worker 스레드에 작업 브로드캐스트
	// [WHY] 전역 상태 변경, 캐시 무효화 등 모든 스레드 동기화 필요 시
	template<typename _Owner, typename _Function, typename... _Args>
	static void RunOnEachAllThread(_Owner* owner, _Function&& function, _Args&&... args);

	// RunOnThread - 특정 스레드에서 실행
	// threadId: 대상 스레드 ID (모듈러 연산으로 유효 범위 보장)
	template<typename _Owner, typename _Function, typename... _Args>
	static void RunOnThread(ThreadId threadId, _Owner* owner, _Function&& function, _Args&&... args);

	// RunOnAnyThread - 임의의 Worker 스레드에서 실행 (라운드 로빈)
	// [WHY] 부하 분산이 필요한 독립적 작업 처리
	template<typename _Owner, typename _Function, typename... _Args>
	static void RunOnAnyThread(_Owner* owner, _Function&& function, _Args&&... args);

	// RunOnCron - Cron 표현식 기반 스케줄 실행
	// cex: Cron 표현식 (예: "0 0 * * *" = 매일 자정)
	// 반환값: 취소용 핸들 (CancelCron에 전달)
	template<typename _Worker, typename _Owner, typename _Function, typename... _Args>
	static CronHandle RunOnCron(ClockPoint now, const cron::cronexpr& cex, std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args);

	// CancelCron - 예약된 Cron 작업 취소
	static void CancelCron(CronHandle cronHandle);
};

// 비동기 실행
template<typename _Worker, typename _Owner, typename _Function, typename... _Args>
void WorkerManager::Run(std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args)
{
	using TaskType = WorkerTask<_Worker, _Owner, _Function, _Args...>;

	TaskType* task = TaskType::Acquire();
	task->Initialize(workerPtr, std::forward<_Owner*>(owner), std::forward<_Function>(function), std::forward<_Args>(args)...);

	workerPtr->PushTask(task);
}

// 동기 실행이 가능하더라도 비동기 실행
template<typename _Worker, typename _Owner, typename _Function, typename... _Args>
void WorkerManager::RunAsyncForced(std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args)
{
	using TaskType = WorkerTask<_Worker, _Owner, _Function, _Args...>;

	TaskType* task = TaskType::Acquire();
	task->Initialize(workerPtr, std::forward<_Owner*>(owner), std::forward<_Function>(function), std::forward<_Args>(args)...);

	workerPtr->PushTaskAsyncForced(task);
}

// 타이머로 실행
template<typename _Worker, typename _Owner, typename _Function, typename... _Args>
void WorkerManager::RunOnTimer(ClockMs delay, std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args)
{
	using TaskType = WorkerTask<_Worker, _Owner, _Function, _Args...>;

	TaskType* task = TaskType::Acquire();
	task->Initialize(workerPtr, std::forward<_Owner*>(owner), std::forward<_Function>(function), std::forward<_Args>(args)...);

	WorkerManager::Run(gTimerWorker->GetTimerWorker(), gTimerWorker.get(), &TimerWorker::Push_async, delay, task);
}

// 모든 스레드에 실행
template<typename _Owner, typename _Function, typename... _Args>
void WorkerManager::RunOnEachAllThread(_Owner* owner, _Function&& function, _Args&&... args)
{
	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		gThreadWorkerPtrList[i]->Push(owner, function, args...);
	}
}

// 특정 스레드에 실행
template<typename _Owner, typename _Function, typename... _Args>
void WorkerManager::RunOnThread(ThreadId threadId, _Owner* owner, _Function&& function, _Args&&... args)
{
	threadId = (threadId % gWorkerThreadCount);
	gThreadWorkerPtrList[threadId]->Push(owner, function, args...);
}

// 임의의 스레드에 실행
template<typename _Owner, typename _Function, typename... _Args>
void WorkerManager::RunOnAnyThread(_Owner* owner, _Function&& function, _Args&&... args)
{
	const ThreadId anyThreadId = gAnyThreadId.fetch_add(1) % gWorkerThreadCount;
	gThreadWorkerPtrList[anyThreadId]->Push(owner, function, args...);
}

// Cron 스케줄로 실행
template<typename _Worker, typename _Owner, typename _Function, typename... _Args>
CronHandle WorkerManager::RunOnCron(ClockPoint now, const cron::cronexpr& cex, std::shared_ptr<_Worker> workerPtr, _Owner* owner, _Function&& function, _Args&&... args)
{
	using TaskType = WorkerTask<_Worker, _Owner, _Function, _Args...>;

	TaskType* task = TaskType::Acquire();
	task->Initialize(workerPtr, std::forward<_Owner*>(owner), std::forward<_Function>(function), std::forward<_Args>(args)...);

	CronHandle cronHandle = ++gCurrentCronHandle;

	std::time_t next = cron::cron_next(cex, tClock.GetGlobalNowTt());
	ClockMs after = (next - tClock.GetGlobalNowTt()) * 1000ms;

	WorkerManager::RunOnTimer(after, gTimerWorker->GetTimerWorker(), gCronWorker.get(), &CronWorker::OnCron_timer, gCronWorker, now, cex, task, cronHandle);

	return cronHandle;
}
