// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;
import ServerEngineCommon;

#include "TestDeterministicRandom.h"


void TestDeterministicRandom::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestDeterministicRandom::OnStarted()
{
	std::wcout << L"=== 결정적 랜덤 테스트 시작 ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	// DeterministicRandom32 테스트
	std::wcout << L"DeterministicRandom32 테스트:" << std::endl;
	DeterministicRandom32 rng32(12345);

	// 범위 [10, 20) 테스트
	std::wcout << L"  범위 [10, 20) 테스트:" << std::endl;
	bool rangeTestPass = true;
	for (int i = 0; i < 10; ++i)
	{
		int32_t value = rng32(10, 20);
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
		int32_t value = rng32(100);
		std::wcout << std::format(L"    Range[{}]: {}", i, value) << std::endl;
		if (value < 0 || value >= 100)
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

	// DeterministicRandom64 테스트
	std::wcout << L"DeterministicRandom64 테스트:" << std::endl;
	DeterministicRandom64 rng64(12345);

	// 범위 [10, 20) 테스트
	std::wcout << L"  범위 [10, 20) 테스트:" << std::endl;
	rangeTestPass = true;
	for (int i = 0; i < 10; ++i)
	{
		int64_t value = rng64(10, 20);
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

	// 범위 [0, 1000) 테스트
	std::wcout << L"  범위 [0, 1000) 테스트:" << std::endl;
	rangeTestPass = true;
	for (int i = 0; i < 10; ++i)
	{
		int64_t value = rng64(1000);
		std::wcout << std::format(L"    Range[{}]: {}", i, value) << std::endl;
		if (value < 0 || value >= 1000)
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

	// 시드 일관성 테스트
	std::wcout << L"시드 일관성 테스트:" << std::endl;
	DeterministicRandom32 rng1(999);
	DeterministicRandom32 rng2(999);
	
	bool seedTestPass = true;
	for (int i = 0; i < 5; ++i)
	{
		int32_t v1 = rng1(100);
		int32_t v2 = rng2(100);
		std::wcout << std::format(L"  시드 테스트[{}]: {} == {}", i, v1, v2) << std::endl;
		if (v1 != v2)
		{
			seedTestPass = false;
		}
	}
	testCount++;
	if (seedTestPass)
	{
		passCount++;
		std::wcout << L"  시드 일관성 테스트: 통과" << std::endl;
	}
	else
	{
		std::wcout << L"  시드 일관성 테스트: 실패" << std::endl;
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
	std::wcout << L"=== 결정적 랜덤 테스트 완료 ===" << std::endl;
}

void TestDeterministicRandom::OnUpdate()
{
}
