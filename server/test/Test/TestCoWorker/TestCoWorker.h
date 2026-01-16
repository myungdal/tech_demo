// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Test/App/TestAppBase.h"
#include "ServerEngine/Worker/WorkerImpl/CoWorker.h"
#include "ServerEngine/Worker/WorkerBase/Worker.h"
#include "ServerEngine/Worker/WorkerTask/WorkerTaskNode.h"
#include <functional>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CoWorker 테스트
// PASS 기준: 크래시/데드락 없음
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TestCoWorker final : public TestAppBase
{
public:
	explicit TestCoWorker(AppArg appArg) : TestAppBase(appArg) {}

public:
	void OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher) override;
	void OnStarted() override;
	void OnUpdate() override;

private:
	// 기본 동작 테스트
	void TestCase_BasicPushPop();
	void TestCase_EmptyPop();
	
	// 만료된 워커 테스트
	void TestCase_ExpiredWorker();
	
	// 멀티스레드 동시 Push 테스트
	void TestCase_ConcurrentPush();
	
	// 멀티스레드 동시 Pop 테스트
	void TestCase_ConcurrentPop();
	
	// 멀티스레드 Push+Pop 혼합 테스트 (스트레스 테스트)
	void TestCase_ConcurrentPushPop();
	
	// Race condition 테스트
	void TestCase_RaceConditionStress();
	
	// 슬롯 재사용 테스트
	void TestCase_SlotReuse();
	
	// 성능 벤치마크
	void TestCase_Performance();

private:
	// 테스트용 간단한 WorkerTaskNode
	class TestTask : public WorkerTaskNode
	{
	public:
		std::function<void()> mFunc;
		
		TestTask(std::function<void()> func) : mFunc(func) {}
		
		virtual void Execute() override { if (mFunc) mFunc(); }
		virtual void ExecuteOnThread() override { Execute(); }
		virtual void Dispose() override { delete this; }
		virtual void Run() override { Execute(); }
		
		WorkerTaskNode* GetNext() { return static_cast<WorkerTaskNode*>(Next); }
	};

	// 테스트용 헬퍼 클래스: 작업 실행 추적용 Worker
	class TestWorker : public Worker
	{
	public:
		std::atomic<int> mExecutionCount{0};
		std::atomic<int> mValue{0};
		
		TestWorker() = default;
		
		void IncrementValue(int delta)
		{
			// memory_order(relaxed): 테스트/통계용 카운터. 다른 공유 데이터의 동기화 용도가 아니므로
			// 원자성만 보장하면 충분(순서/가시성 보장 불필요).
			mValue.fetch_add(delta, std::memory_order_relaxed);
			mExecutionCount.fetch_add(1, std::memory_order_relaxed);
		}

		// 편의 함수: 람다를 task로 추가
		void PushTestTask(std::function<void()> func)
		{
			PushTask(new TestTask(func));
		}
	};
};

inline std::shared_ptr<TestCoWorker> gTestCoWorker = nullptr;

