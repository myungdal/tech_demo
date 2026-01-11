// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;

#include "TestSmooth.h"

#include "MmoSync/Util/Smooth/SmoothScalar.h"


void TestSmooth::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestSmooth::OnStarted()
{
	std::wcout << L"=== 스무스 테스트 시작 ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	// 테스트 1: 기본 수렴 테스트 (적절한 값 범위 사용)
	{
		std::wcout << L"테스트 1: 기본 수렴 테스트" << std::endl;
		SmoothScalar ss;
		float x = 100.f;
		const float target = 200.f;
		const float factor = 0.2f;
		const float threshold = 10.f; // 허용 오차

		std::wcout << L"  초기값: " << x << std::endl;
		std::wcout << L"  목표값: " << target << std::endl;
		std::wcout << L"  업데이트 팩터: " << factor << std::endl;
		std::wcout << L"  임계값: " << threshold << std::endl;
		std::wcout << L"  SmoothScalar 업데이트 진행상황:" << std::endl;

		bool converged = false;
		for (int i = 0; i < 50; ++i) // 충분한 반복
		{
			x = ss.Update(x, target, factor);
			std::wcout << std::format(L"    단계 {}: {} (차이: {})", i + 1, x, std::abs(x - target)) << std::endl;
			
			if (std::abs(x - target) < threshold)
			{
				converged = true;
				std::wcout << L"    단계 " << (i + 1) << L"에서 수렴" << std::endl;
				break;
			}
		}

		testCount++;
		if (converged)
		{
			passCount++;
			std::wcout << L"  기본 수렴 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  기본 수렴 테스트: 실패 (50단계 내 수렴하지 않음)" << std::endl;
		}
	}

	// 테스트 2: 다른 초기값과 목표값으로 테스트
	{
		std::wcout << L"테스트 2: 다른 초기값과 목표값 테스트" << std::endl;
		SmoothScalar ss;
		float x = 0.f;
		const float target = 1000.f;
		const float factor = 0.1f;
		const float threshold = 10.f;

		std::wcout << L"  초기값: " << x << std::endl;
		std::wcout << L"  목표값: " << target << std::endl;
		std::wcout << L"  업데이트 팩터: " << factor << std::endl;

		bool converged = false;
		for (int i = 0; i < 100; ++i)
		{
			x = ss.Update(x, target, factor);
			if (std::abs(x - target) < threshold)
			{
				converged = true;
				std::wcout << L"  단계 " << (i + 1) << L"에서 값 " << x << L"로 수렴" << std::endl;
				break;
			}
		}

		testCount++;
		if (converged)
		{
			passCount++;
			std::wcout << L"  다른 값 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  다른 값 테스트: 실패" << std::endl;
		}
	}

	// 테스트 3: 이미 목표값에 가까운 경우
	{
		std::wcout << L"테스트 3: 이미 목표값에 가까운 경우" << std::endl;
		SmoothScalar ss;
		float x = 99.5f;
		const float target = 100.f;
		const float factor = 0.5f;
		const float threshold = 0.1f;

		std::wcout << L"  초기값: " << x << std::endl;
		std::wcout << L"  목표값: " << target << std::endl;

		x = ss.Update(x, target, factor);
		std::wcout << L"  한 번 업데이트 후: " << x << L" (차이: " << std::abs(x - target) << L")" << std::endl;

		testCount++;
		if (std::abs(x - target) < threshold)
		{
			passCount++;
			std::wcout << L"  목표값 근처 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  목표값 근처 테스트: 실패" << std::endl;
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
	std::wcout << L"=== 스무스 테스트 완료 ===" << std::endl;
}

void TestSmooth::OnUpdate()
{
}
