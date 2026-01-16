// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;

#include "TestCoWorker.h"

// [WHY] 표준 라이브러리 헤더들은 pch_common.h에 이미 포함됨


void TestCoWorker::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestCoWorker::OnStarted()
{
	std::wcout << L"================================================================================\n";
	std::wcout << L"=== CoWorker �׽�Ʈ ���� ===\n";
	std::wcout << L"=== PASS ����: ũ����/����� ���� ===\n";
	std::wcout << L"================================================================================\n\n";

	TestCase_BasicPushPop();
	TestCase_EmptyPop();
	TestCase_ExpiredWorker();
	TestCase_ConcurrentPush();
	TestCase_ConcurrentPop();
	TestCase_ConcurrentPushPop();
	TestCase_RaceConditionStress();
	TestCase_SlotReuse();
	TestCase_Performance();

	std::wcout << L"\n================================================================================\n";
	std::wcout << L"=== CoWorker �׽�Ʈ �Ϸ� ===\n";
	std::wcout << L"================================================================================\n";
}

void TestCoWorker::OnUpdate()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 1: 기본 Push/Pop 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_BasicPushPop()
{
	std::wcout << L"[�׽�Ʈ 1] �⺻ Push/Pop ����\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	auto startTime = std::chrono::high_resolution_clock::now();

	auto coWorker = std::make_shared<CoWorker>();
	auto worker = std::make_shared<TestWorker>();

	// 워커에 테스트 작업 추가
	int initialValue = worker->mValue.load();
	worker->PushTestTask([worker]() {
		worker->IncrementValue(1);
	});

	// Push
	coWorker->PushWorker(worker);

	// Run
	coWorker->Run();

	int afterValue = worker->mValue.load();

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

	std::wcout << std::format(L"  �ҿ� �ð�: {}us\n", duration.count());
	std::wcout << std::format(L"  ��Ŀ Value: {} �� {}\n", initialValue, afterValue);

	if (afterValue > initialValue)
	{
		std::wcout << L"  [PASS] ��Ŀ ���� Ȯ��\n";
	}
	else
	{
		std::wcout << L"  [WARN] ��Ŀ ���� ��Ȯ��\n";
	}

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 2: 빈 큐에서 Pop/Run
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_EmptyPop()
{
	std::wcout << L"[�׽�Ʈ 2] �� ť���� Pop\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	auto startTime = std::chrono::high_resolution_clock::now();

	auto coWorker = std::make_shared<CoWorker>();

	// 빈 큐에서 Run (크래시 없이 종료되어야 함)
	coWorker->Run();

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

	std::wcout << std::format(L"  �ҿ� �ð�: {}us\n", duration.count());
	std::wcout << L"  [PASS] �� ť Run ũ���� ����\n";

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 3: 만료(Expired) 워커 처리
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_ExpiredWorker()
{
	std::wcout << L"[�׽�Ʈ 3] ����� ��Ŀ ó��\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	constexpr int MAX_RUN_ATTEMPTS = 100;

	auto startTime = std::chrono::high_resolution_clock::now();

	auto coWorker = std::make_shared<CoWorker>();

	{
		auto worker = std::make_shared<TestWorker>();
		coWorker->PushWorker(worker);
	}

	// 만료된 워커가 남아있을 수 있는 상태에서 새 워커 Push 시도
	auto newWorker = std::make_shared<TestWorker>();
	newWorker->PushTestTask([newWorker]() {
		newWorker->IncrementValue(1);
	});
	coWorker->PushWorker(newWorker);

	// Run 시도
	int initialValue = newWorker->mValue.load();
	for (int i = 0; i < MAX_RUN_ATTEMPTS; ++i)
	{
		coWorker->Run();
	}
	int afterValue = newWorker->mValue.load();

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

	std::wcout << std::format(L"  �ҿ� �ð�: {}us\n", duration.count());
	std::wcout << std::format(L"  �� ��Ŀ Value: {} �� {}\n", initialValue, afterValue);

	std::wcout << L"  [PASS] Expired ��Ŀ ó�� ũ���� ����\n";

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 4: 멀티스레드 동시 Push
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_ConcurrentPush()
{
	std::wcout << L"[�׽�Ʈ 4] ��Ƽ������ ���� Push (50 ������ �� 50 ��Ŀ)\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	constexpr int THREAD_COUNT = 50;
	constexpr int PUSH_PER_THREAD = 50;
	constexpr int TOTAL_WORKERS = THREAD_COUNT * PUSH_PER_THREAD;

	auto coWorker = std::make_shared<CoWorker>();
	std::vector<std::shared_ptr<TestWorker>> allWorkers;
	std::mutex workersMutex;
	std::vector<std::thread> threads;

	auto startTime = std::chrono::high_resolution_clock::now();

	for (int t = 0; t < THREAD_COUNT; ++t)
	{
		threads.emplace_back([&]() {
			for (int i = 0; i < PUSH_PER_THREAD; ++i)
			{
				auto worker = std::make_shared<TestWorker>();
				{
					std::lock_guard<std::mutex> lock(workersMutex);
					allWorkers.push_back(worker);
				}
				coWorker->PushWorker(worker);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	std::wcout << std::format(L"  Push ��: {}, �ҿ� �ð�: {}ms\n", TOTAL_WORKERS, duration.count());
	std::wcout << L"  [PASS] ���� Push ũ����/����� ����\n";

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 5: 멀티스레드 동시 Pop
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_ConcurrentPop()
{
	std::wcout << L"[�׽�Ʈ 5] ��Ƽ������ ���� Pop (1000 ��Ŀ �� 50 ������)\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	constexpr int WORKER_COUNT = 1000;
	constexpr int POP_THREAD_COUNT = 50;
	constexpr int MAX_DURATION_MS = 3000;

	auto coWorker = std::make_shared<CoWorker>();
	std::vector<std::shared_ptr<TestWorker>> allWorkers;
	allWorkers.reserve(WORKER_COUNT);

	// 테스트 워커들을 큐에 Push
	for (int i = 0; i < WORKER_COUNT; ++i)
	{
		auto worker = std::make_shared<TestWorker>();
		allWorkers.push_back(worker);
		coWorker->PushWorker(worker);
	}

	std::atomic<bool> stopFlag{false};
	std::atomic<int> runCount{0};
	std::vector<std::thread> threads;

	auto startTime = std::chrono::high_resolution_clock::now();

	for (int t = 0; t < POP_THREAD_COUNT; ++t)
	{
		threads.emplace_back([&]() {
			while (!stopFlag.load())
			{
				coWorker->Run();
				runCount.fetch_add(1);
				std::this_thread::yield();
			}
		});
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(MAX_DURATION_MS));
	stopFlag.store(true);

	for (auto& thread : threads)
	{
		thread.join();
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	std::wcout << std::format(L"  Run ȣ�� ��: {}, �ҿ� �ð�: {}ms\n", runCount.load(), duration.count());
	std::wcout << L"  [PASS] ���� Pop ũ����/����� ����\n";

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 6: 멀티스레드 Push+Pop 혼합(스트레스 테스트)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_ConcurrentPushPop()
{
	std::wcout << L"[�׽�Ʈ 6] ��Ƽ������ Push+Pop ȥ�� ��Ʈ���� �׽�Ʈ\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	constexpr int PUSH_THREAD_COUNT = 20;
	constexpr int POP_THREAD_COUNT = 20;
	constexpr int PUSH_PER_THREAD = 50;
	constexpr int DURATION_SECONDS = 2;

	auto coWorker = std::make_shared<CoWorker>();
	std::atomic<bool> stopFlag{false};
	std::atomic<int> pushCount{0};
	std::atomic<int> runCount{0};

	std::vector<std::thread> threads;
	std::vector<std::shared_ptr<TestWorker>> allWorkers;
	std::mutex workersMutex;

	auto startTime = std::chrono::high_resolution_clock::now();

	// Push 스레드
	for (int t = 0; t < PUSH_THREAD_COUNT; ++t)
	{
		threads.emplace_back([&]() {
			for (int i = 0; i < PUSH_PER_THREAD; ++i)
			{
				auto worker = std::make_shared<TestWorker>();
				{
					std::lock_guard<std::mutex> lock(workersMutex);
					allWorkers.push_back(worker);
				}
				coWorker->PushWorker(worker);
				pushCount.fetch_add(1);
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
		});
	}

	// Pop 스레드
	for (int t = 0; t < POP_THREAD_COUNT; ++t)
	{
		threads.emplace_back([&]() {
			while (!stopFlag.load())
			{
				coWorker->Run();
				runCount.fetch_add(1);
				std::this_thread::yield();
			}
		});
	}

	std::this_thread::sleep_for(std::chrono::seconds(DURATION_SECONDS));
	stopFlag.store(true);

	for (auto& thread : threads)
	{
		thread.join();
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	std::wcout << std::format(L"  Push ��: {}, Run ȣ�� ��: {}, �ҿ� �ð�: {}ms\n", 
		pushCount.load(), runCount.load(), duration.count());
	std::wcout << L"  [PASS] Push+Pop ȥ�� ũ����/����� ����\n";

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 7: Race Condition 스트레스 테스트
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_RaceConditionStress()
{
	std::wcout << L"[�׽�Ʈ 7] Race Condition ��Ʈ���� �׽�Ʈ\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	constexpr int ITERATIONS = 3;
	constexpr int THREAD_COUNT = 40;
	constexpr int OPS_PER_THREAD = 50;

	for (int iter = 0; iter < ITERATIONS; ++iter)
	{
		auto coWorker = std::make_shared<CoWorker>();
		std::vector<std::shared_ptr<TestWorker>> testWorkers;
		std::mutex workersMutex;
		std::vector<std::thread> threads;

		for (int t = 0; t < THREAD_COUNT; ++t)
		{
			threads.emplace_back([&, t]() {
				for (int i = 0; i < OPS_PER_THREAD; ++i)
				{
					if (t % 2 == 0)
					{
						auto worker = std::make_shared<TestWorker>();
						{
							std::lock_guard<std::mutex> lock(workersMutex);
							testWorkers.push_back(worker);
						}
						coWorker->PushWorker(worker);
					}
					else
					{
						coWorker->Run();
					}
				}
			});
		}

		for (auto& thread : threads)
		{
			thread.join();
		}

		// 정리: 잔여 워커 소진
		constexpr int MAX_CLEANUP_ATTEMPTS = 10000;
		for (int i = 0; i < MAX_CLEANUP_ATTEMPTS; ++i)
		{
			coWorker->Run();
		}

		std::wcout << std::format(L"  Iteration {} �Ϸ�\n", iter + 1);
	}

	std::wcout << std::format(L"  [PASS] {} iterations �Ϸ�\n", ITERATIONS);

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 8: 슬롯 재사용 검증
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_SlotReuse()
{
	std::wcout << L"[�׽�Ʈ 8] ���� ���� �׽�Ʈ\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	constexpr int CYCLES = 5;
	constexpr int WORKERS_PER_CYCLE = 500;
	constexpr int MAX_POP_ATTEMPTS = 2000;
	constexpr int TOTAL_PUSH = CYCLES * WORKERS_PER_CYCLE;

	auto startTime = std::chrono::high_resolution_clock::now();

	auto coWorker = std::make_shared<CoWorker>();

	for (int cycle = 0; cycle < CYCLES; ++cycle)
	{
		std::vector<std::shared_ptr<TestWorker>> cycleWorkers;
		cycleWorkers.reserve(WORKERS_PER_CYCLE);

		for (int i = 0; i < WORKERS_PER_CYCLE; ++i)
		{
			auto worker = std::make_shared<TestWorker>();
			cycleWorkers.push_back(worker);
			coWorker->PushWorker(worker);
		}

		for (int i = 0; i < MAX_POP_ATTEMPTS; ++i)
		{
			coWorker->Run();
		}
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	std::wcout << std::format(L"  Push ��: {}, �ҿ� �ð�: {}ms\n", TOTAL_PUSH, duration.count());
	std::wcout << L"  [PASS] ���� ���� �׽�Ʈ �Ϸ�\n";

	std::wcout << L"\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Case 9: 성능 측정
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestCoWorker::TestCase_Performance()
{
	std::wcout << L"[�׽�Ʈ 9] ���� ��ġ��ũ\n";
	std::wcout << L"--------------------------------------------------------------------------------\n";

	constexpr int BATCH_SIZE = 1000;
	constexpr int BATCH_COUNT = 10;
	constexpr int TOTAL_OPERATIONS = BATCH_SIZE * BATCH_COUNT;
	constexpr int MAX_POP_PER_BATCH = 3000;

	auto coWorker = std::make_shared<CoWorker>();

	long long totalPushDuration = 0;
	long long totalPopDuration = 0;

	for (int batch = 0; batch < BATCH_COUNT; ++batch)
	{
		std::vector<std::shared_ptr<TestWorker>> batchWorkers;
		batchWorkers.reserve(BATCH_SIZE);

		// Push 측정
		auto pushStart = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < BATCH_SIZE; ++i)
		{
			auto worker = std::make_shared<TestWorker>();
			batchWorkers.push_back(worker);
			coWorker->PushWorker(worker);
		}
		auto pushEnd = std::chrono::high_resolution_clock::now();
		totalPushDuration += std::chrono::duration_cast<std::chrono::microseconds>(pushEnd - pushStart).count();

		// Pop 측정
		auto popStart = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < MAX_POP_PER_BATCH; ++i)
		{
			coWorker->Run();
		}
		auto popEnd = std::chrono::high_resolution_clock::now();
		totalPopDuration += std::chrono::duration_cast<std::chrono::microseconds>(popEnd - popStart).count();
	}

	double pushOpsPerSec = totalPushDuration > 0 ? (TOTAL_OPERATIONS * 1000000.0) / totalPushDuration : 0;
	double popOpsPerSec = totalPopDuration > 0 ? (MAX_POP_PER_BATCH * BATCH_COUNT * 1000000.0) / totalPopDuration : 0;

	std::wcout << std::format(L"  �� Push: {}\n", TOTAL_OPERATIONS);
	std::wcout << std::format(L"  Push: {} us ({:.0f} ops/sec)\n", totalPushDuration, pushOpsPerSec);
	std::wcout << std::format(L"  Pop:  {} us ({:.0f} ops/sec)\n", totalPopDuration, popOpsPerSec);
	std::wcout << L"  [PASS] ��ġ��ũ �Ϸ�\n\n";
}

