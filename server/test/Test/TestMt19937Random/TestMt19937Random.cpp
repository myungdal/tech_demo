// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;
import ServerEngineCommon;

#include "TestMt19937Random.h"


void TestMt19937Random::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestMt19937Random::OnStarted()
{
	std::wcout << L"=== MT19937 랜덤 테스트 시작 ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	// 테스트 1: Mt19937Random32 범위 테스트
	{
		std::wcout << L"테스트 1: Mt19937Random32 범위 테스트" << std::endl;
		Mt19937Random32 rng32;

		// 범위 [10, 20) 테스트
		std::wcout << L"  범위 [10, 20) 테스트:" << std::endl;
		bool rangeTestPass = true;
		for (int i = 0; i < 10; ++i)
		{
			uint32_t value = rng32(10, 20);
			std::wcout << std::format(L"    Random32[{}]: {}", i, value) << std::endl;
			if (value < 10 || value >= 20)
			{
				rangeTestPass = false;
			}
		}
		testCount++;
		if (rangeTestPass)
		{
			passCount++;
			std::wcout << L"  범위 [10, 20) 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  범위 [10, 20) 테스트: 실패" << std::endl;
		}

		// 범위 [0, 100) 테스트
		std::wcout << L"  범위 [0, 100) 테스트:" << std::endl;
		rangeTestPass = true;
		for (int i = 0; i < 10; ++i)
		{
			uint32_t value = rng32(100);
			std::wcout << std::format(L"    Range[{}]: {}", i, value) << std::endl;
			if (value >= 100)
			{
				rangeTestPass = false;
			}
		}
		testCount++;
		if (rangeTestPass)
		{
			passCount++;
			std::wcout << L"  범위 [0, 100) 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  범위 [0, 100) 테스트: 실패" << std::endl;
		}
	}

	// 테스트 2: Mt19937Random64 범위 테스트
	{
		std::wcout << L"테스트 2: Mt19937Random64 범위 테스트" << std::endl;
		Mt19937Random64 rng64;

		// 범위 [10, 20) 테스트
		std::wcout << L"  범위 [10, 20) 테스트:" << std::endl;
		bool rangeTestPass = true;
		for (int i = 0; i < 10; ++i)
		{
			uint64_t value = rng64(10, 20);
			std::wcout << std::format(L"    Random64[{}]: {}", i, value) << std::endl;
			if (value < 10 || value >= 20)
			{
				rangeTestPass = false;
			}
		}
		testCount++;
		if (rangeTestPass)
		{
			passCount++;
			std::wcout << L"  범위 [10, 20) 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  범위 [10, 20) 테스트: 실패" << std::endl;
		}

		// 범위 [0, 1000) 테스트 (출력 문구와 실제 호출 일치)
		std::wcout << L"  범위 [0, 1000) 테스트:" << std::endl;
		rangeTestPass = true;
		for (int i = 0; i < 10; ++i)
		{
			uint64_t value = rng64(1000); // 실제로 1000을 사용
			std::wcout << std::format(L"    Range[{}]: {}", i, value) << std::endl;
			if (value >= 1000)
			{
				rangeTestPass = false;
			}
		}
		testCount++;
		if (rangeTestPass)
		{
			passCount++;
			std::wcout << L"  범위 [0, 1000) 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  범위 [0, 1000) 테스트: 실패" << std::endl;
		}
	}

	// 테스트 3: 분포 균등성 테스트
	{
		std::wcout << L"테스트 3: 분포 균등성 테스트" << std::endl;
		
		Mt19937Random32 rng;
		
		// 0-9 범위에서 1000번 샘플링
		int counts[10] = {0};
		for (int i = 0; i < 1000; ++i)
		{
			uint32_t value = rng(10);
			if (value < 10)
				counts[value]++;
		}
		
		// 각 값이 50-150 범위에 있어야 함 (100 ± 50)
		bool uniformTestPass = true;
		for (int i = 0; i < 10; ++i)
		{
			std::wcout << std::format(L"  Count[{}]: {}", i, counts[i]) << std::endl;
			if (counts[i] < 50 || counts[i] > 150)
			{
				uniformTestPass = false;
			}
		}
		
		testCount++;
		if (uniformTestPass)
		{
			passCount++;
			std::wcout << L"  균등성 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  균등성 테스트: 실패" << std::endl;
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
	std::wcout << L"=== MT19937 랜덤 테스트 완료 ===" << std::endl;
}

void TestMt19937Random::OnUpdate()
{
}
