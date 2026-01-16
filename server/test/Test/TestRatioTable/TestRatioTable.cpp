// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;
import ServerEngineCommon;

#include "TestRatioTable.h"


void TestRatioTable::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestRatioTable::OnStarted()
{
	std::wcout << L"=== 비율 테이블 테스트 시작 ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	// 테스트 1: 기본 가중치 비례 검증
	{
		std::wcout << L"테스트 1: 기본 가중치 비례 검증" << std::endl;
		
		DeterministicRandom64 rng(12345);
		RatioTable<int> ratioTable;

		// 비율 설정
		ratioTable.Add(1, 10);  // 10% 확률
		ratioTable.Add(2, 20);  // 20% 확률
		ratioTable.Add(3, 30);  // 30% 확률
		ratioTable.Add(4, 40);  // 40% 확률

		std::wcout << L"  Testing RatioTable with ratios: 1(10%), 2(20%), 3(30%), 4(40%)" << std::endl;

		// 통계 수집
		std::map<int, int> counts;
		int totalSamples = 10000;

		for (int i = 0; i < totalSamples; ++i)
		{
			int selected = 0;
			ratioTable.Pick(OUT selected, rng);
			counts[selected]++;
		}

		// 결과 출력 및 검증
		std::wcout << L"  Sampling results:" << std::endl;
		bool proportionValid = true;
		double expectedRatios[] = {0.1, 0.2, 0.3, 0.4}; // 예상 비율
		int values[] = {1, 2, 3, 4};
		double tolerance = 0.05; // 5% 허용 오차
		
		for (int i = 0; i < 4; ++i)
		{
			int value = values[i];
			int count = counts[value];
			double percentage = static_cast<double>(count) / totalSamples;
			double expected = expectedRatios[i];
			
			std::wcout << std::format(L"    Value {}: {} times ({:.2f}%, expected: {:.2f}%)", 
				value, count, percentage * 100.0, expected * 100.0) << std::endl;
			
			if (std::abs(percentage - expected) > tolerance)
			{
				proportionValid = false;
				std::wcout << L"    WARNING: Value " << value << L" deviates from expected ratio!" << std::endl;
			}
		}

		testCount++;
		if (proportionValid)
		{
			passCount++;
			std::wcout << L"  Basic proportion test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Basic proportion test: FAIL" << std::endl;
		}
	}

	// 테스트 2: 빈 테이블 처리 검증
	{
		std::wcout << L"Test 2: Empty table handling" << std::endl;
		
		DeterministicRandom64 rng(54321);
		RatioTable<int> emptyTable;
		int emptyResult = 0;
		bool pickResult = emptyTable.Pick(OUT emptyResult, rng);
		
		std::wcout << L"  Empty table pick result: " << (pickResult ? L"true" : L"false") << std::endl;
		std::wcout << L"  Empty table result value: " << emptyResult << std::endl;

		testCount++;
		if (!pickResult)
		{
			passCount++;
			std::wcout << L"  Empty table test: OK (correctly failed to pick)" << std::endl;
		}
		else
		{
			std::wcout << L"  Empty table test: FAIL (should not pick from empty table)" << std::endl;
		}
	}

	// 테스트 3: 단일 항목 테스트
	{
		std::wcout << L"Test 3: Single item verification" << std::endl;
		
		DeterministicRandom64 rng(99999);
		RatioTable<int> singleTable;
		singleTable.Add(99, 100);
		
		bool singleItemValid = true;
		for (int i = 0; i < 10; ++i)
		{
			int result = 0;
			bool pickResult = singleTable.Pick(OUT result, rng);
			
			if (!pickResult || result != 99)
			{
				singleItemValid = false;
				std::wcout << L"    ERROR: Single item test failed at iteration " << i << std::endl;
				break;
			}
		}

		testCount++;
		if (singleItemValid)
		{
			passCount++;
			std::wcout << L"  Single item test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Single item test: FAIL" << std::endl;
		}
	}

	// 테스트 4: 재현성 검증
	{
		std::wcout << L"Test 4: Reproducibility verification" << std::endl;
		
		RatioTable<int> table1, table2;
		table1.Add(1, 50);
		table1.Add(2, 50);
		table2.Add(1, 50);
		table2.Add(2, 50);
		
		DeterministicRandom64 rng1(77777), rng2(77777); // 동일한 시드
		
		bool reproducibilityValid = true;
		const int testIterations = 100;
		
		for (int i = 0; i < testIterations; ++i)
		{
			int result1 = 0, result2 = 0;
			bool pick1 = table1.Pick(OUT result1, rng1);
			bool pick2 = table2.Pick(OUT result2, rng2);
			
			if (!pick1 || !pick2 || result1 != result2)
			{
				reproducibilityValid = false;
				std::wcout << L"    ERROR: Results differ at iteration " << i 
					<< L": " << result1 << L" vs " << result2 << std::endl;
				break;
			}
		}

		testCount++;
		if (reproducibilityValid)
		{
			passCount++;
			std::wcout << L"  Reproducibility test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Reproducibility test: FAIL" << std::endl;
		}
	}

	// 테스트 5: 균등 분포 테스트
	{
		std::wcout << L"Test 5: Uniform distribution test" << std::endl;
		
		DeterministicRandom64 rng(11111);
		RatioTable<int> uniformTable;
		
		// 모든 항목에 동일한 가중치 부여
		uniformTable.Add(1, 1);
		uniformTable.Add(2, 1);
		uniformTable.Add(3, 1);
		uniformTable.Add(4, 1);
		uniformTable.Add(5, 1);
		
		std::map<int, int> counts;
		const int totalSamples = 5000;
		
		for (int i = 0; i < totalSamples; ++i)
		{
			int selected = 0;
			uniformTable.Pick(OUT selected, rng);
			counts[selected]++;
		}
		
		// 각 항목이 20% ± 5% 범위에 있어야 함
		bool uniformValid = true;
		double expectedPercentage = 20.0; // 20%
		double tolerance = 5.0; // 5% 허용 오차
		
		for (int value = 1; value <= 5; ++value)
		{
			int count = counts[value];
			double percentage = static_cast<double>(count) / totalSamples * 100.0;
			
			std::wcout << std::format(L"    Value {}: {} times ({:.2f}%)", 
				value, count, percentage) << std::endl;
			
			if (std::abs(percentage - expectedPercentage) > tolerance)
			{
				uniformValid = false;
				std::wcout << L"    WARNING: Value " << value << L" deviates from uniform distribution!" << std::endl;
			}
		}

		testCount++;
		if (uniformValid)
		{
			passCount++;
			std::wcout << L"  Uniform distribution test: OK" << std::endl;
		}
		else
		{
			std::wcout << L"  Uniform distribution test: FAIL" << std::endl;
		}
	}

	// 결과 요약
	std::wcout << L"=== 테스트 결과 ===" << std::endl;
	std::wcout << std::format(L"통과: {}/{} 테스트", passCount, testCount) << std::endl;
	if (passCount == testCount)
	{
		std::wcout << L"전체: 성공" << std::endl;
	}
	else
	{
		std::wcout << L"전체: 실패" << std::endl;
	}
	std::wcout << L"=== 비율 테이블 테스트 완료 ===" << std::endl;
}

void TestRatioTable::OnUpdate()
{
}
