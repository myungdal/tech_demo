// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/Worker/WorkerTask/WorkerTaskImpl.h"
#include "ServerEngine/Worker/WorkerTask/WorkerTaskNode.h"
#include "Common/MemPool/DynamicPool.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WorkerTask - 메모리 풀링을 지원하는 최종 작업 노드
//
// 설계 목표:
// - WorkerTaskImpl에 메모리 풀링 기능 추가
// - 빈번한 작업 생성/소멸의 할당 비용 제거
//
// 메모리 풀 구조:
// - DynamicPool<WorkerTask, 32>: 초기 32개 슬롯, 동적 확장
// - 스레드 안전: Lock으로 풀 접근 보호
// - 타입별 독립 풀: 각 템플릿 인스턴스마다 별도 풀
//
// 사용 흐름:
// 1. Acquire(): 풀에서 노드 획득 (없으면 new)
// 2. Initialize(): 캡처 데이터 설정
// 3. Execute()/ExecuteOnThread(): 작업 실행
// 4. Dispose(): 풀에 반환
//
// [WHY] 왜 정적 풀인가?
// - 게임 서버는 작업 생성/소멸이 매우 빈번
// - 힙 할당 비용 및 단편화 방지
// - 타입별 풀로 크기가 다른 작업도 효율적 관리
//
// [PERF] 성능 특성:
// - O(1) 할당/해제 (풀에 여유 있을 때)
// - Lock 경합은 풀 접근 시에만 발생 (실행 시 Lock-free)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename _Worker, typename _Owner, typename _Function, typename... _Args>
class WorkerTask : public WorkerTaskImpl<WorkerTaskNode, _Worker, _Owner, _Function, _Args...>
{
public:
	using PoolPtr = DynamicPool<WorkerTask, 32>;
	static inline PoolPtr mPool{};
	static inline Lock mLock;

	static WorkerTask* Acquire() 
	{ 
		WriteLock lock(mLock);

		return mPool.Pop(); 
	}

	void Dispose() override
	{
		__super::Release();

		WriteLock lock(mLock);

		mPool.Push(this);
	}

public:
	void Run() override
	{
		if (auto workerPtr = __super::mWorkerWeakPtr.lock())
		{
			workerPtr->PushTask(this);
		}
	}
};
