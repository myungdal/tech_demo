// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimerWorker - 시간 기반 지연 실행 스케줄러
//
// 설계 목표:
// - 밀리초 단위의 정밀한 지연 실행 지원
// - 대량의 타이머 작업을 효율적으로 관리
//
// 구현 전략 (하이브리드 구조):
// 1. 슬롯 테이블 (단기 타이머, ~65초):
//    - 4096개 슬롯, 각 슬롯은 16ms 간격
//    - O(1) 삽입/조회, 메모리 효율적
//    - 해시 기반 순환 버퍼 구조
//
// 2. 우선순위 큐 (장기 타이머, 65초 초과):
//    - std::priority_queue 사용
//    - O(log N) 삽입, O(1) 최소값 조회
//
// [WHY] 왜 하이브리드인가?
// - 게임 서버에서 대부분의 타이머는 단기 (스킬 쿨다운, 버프 지속시간 등)
// - 단기 타이머에 O(1) 성능 제공, 장기 타이머도 지원
//
// [PERF] 성능 특성:
// - 슬롯 기반 처리로 대량의 단기 타이머 효율적 관리
// - Flush_async가 16ms마다 호출되어 슬롯 진행
// - Worker 기반 실행으로 스레드 안전성 보장
//
// 사용 예시:
// WorkerManager::RunOnTimer(1000ms, workerPtr, owner, &Owner::Method);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Worker;
class WorkerTaskNode;

class TimerWorker
{
	friend class WorkerManager;

private:
	std::shared_ptr<Worker> mTimerWorker = nullptr;

private:
	using TimerTask = std::pair<ClockMs, WorkerTaskNode*>;
	using TimerTaskList = std::vector<TimerTask>;

	struct Comparator
	{
		bool operator()(const TimerTask& l, const TimerTask& r) const
		{
			return (std::get<0>(l) > std::get<0>(r)) ? true : false;
		}
	};

	using TimerTaskTable = std::priority_queue<TimerTask, TimerTaskList, Comparator>;

	TimerTaskTable mTimerTaskTable;

private:
	static constexpr size_t SLOT_SIZE = 4096;
	static constexpr size_t SLOT_SIZE_MASK = (SLOT_SIZE - 1);
	static constexpr size_t SLOT_TIME_DELTA_EXOINENT = 4;
	static constexpr ClockMs SLOT_TIME_DELTA = ClockMs(1 << SLOT_TIME_DELTA_EXOINENT);
	static constexpr size_t SLOT_TIME_DELTA_MASK = 0b1111;
	static constexpr ClockMs MAX_SLOT_TIME_DELTA = (SLOT_SIZE_MASK * SLOT_TIME_DELTA);

	using SlotTaskList = std::vector<WorkerTaskNode*>;
	using SlotTaskTable = std::array<SlotTaskList, SLOT_SIZE>;

	SlotTaskTable mSlotTaskTable;
	ClockMs mCurrClockMs = 0ms;
	size_t mCurrSlotIndex = 0;

public:
	TimerWorker();

private:
	friend class AppBase;

	void Resume();

private:
	void Push_async(ClockMs delay, WorkerTaskNode* task);
	void Flush_async();


public:
	std::shared_ptr<Worker> GetTimerWorker() { return mTimerWorker; }
};

inline std::shared_ptr<TimerWorker> gTimerWorker = nullptr;
