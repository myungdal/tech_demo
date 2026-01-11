// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CoWorker - 협력적 작업 분배자 (Cooperative Worker Dispatcher)
//
// 설계 목표:
// - 대기 중인 Worker 작업을 유휴 스레드에 분배
// - Lock-free 구조로 스레드 간 효율적 작업 전달
//
// 문제 상황:
// - Worker A 처리 중인 스레드에 Worker B 작업이 도착
// - Worker B를 즉시 처리하면 Worker A의 단일 스레드 보장 위반
// - 해결: CoWorker에 Worker B를 위임, 다른 유휴 스레드가 처리
//
// 구현 구조:
// - 순환 배열 (16384개 슬롯)
// - Lock-free Push/Pop (InterlockedExchangePointer)
// - weak_ptr로 Worker 참조 (수명 안전성)
//
// 실행 흐름:
// 1. Worker::TryWork() 실패 시 CoWorker::PushWorker() 호출
// 2. Worker 스레드가 CoWorker::Run() 호출로 대기 Worker 처리
// 3. Pop한 Worker의 TryWork() 재시도
//
// [WHY] 왜 CoWorker가 필요한가?
// - Worker의 단일 스레드 보장 유지하면서 작업 지연 방지
// - 바쁜 스레드가 아닌 유휴 스레드가 처리하도록 부하 분산
//
// [PERF] 성능 특성:
// - Lock-free 순환 배열로 경합 최소화
// - weak_ptr로 이미 파괴된 Worker 안전하게 스킵
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Worker;

class CoWorker
{
	friend class WorkerManager;
	friend class TestCoWorker;

private:
	static constexpr size_t POOL_SIZE = (1 << 14);
	static constexpr size_t POOL_SIZE_MASK = (POOL_SIZE - 1);

private:
	std::array<std::weak_ptr<Worker>*, POOL_SIZE> mWorkerArray;

	std::atomic<size_t> mPopPos{ 0 };
	std::atomic<size_t> mPushPos{ 0 };
	std::atomic<size_t> mWorkerCnt{ 0 };

public:
	CoWorker();
	virtual ~CoWorker();

private:
	friend class Worker;
	void PushWorker(std::shared_ptr<Worker> worker);

private:
	friend class WorkerThread;
	void Run();
};

inline std::shared_ptr<CoWorker> gCoWorker = nullptr;
