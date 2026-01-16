// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;
import ServerEngineCommon;

#include "TestRandom.h"


void TestRandom::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestRandom::OnStarted()
{
	std::wcout << L"=== 랜덤 테스트 시작 ===" << std::endl;

	// EaseOutSine 테스트
	std::wcout << L"Testing EaseOutSine function:" << std::endl;
	for (int i = 0; i <= 1024; i += 128)
	{
		std::wcout << std::format(L"  EaseOutSine({}): {}", i, MathUtil::EaseOutSine(i)) << std::endl;
	}

	// DeterministicRandom32 테스트
	std::wcout << L"Testing DeterministicRandom32:" << std::endl;
	DeterministicRandom32 random(123);

	for (int i = 0; i < 10; ++i)
	{
		int32_t r = random(-32'768, 32'767);
		std::wcout << std::format(L"  Random[{}]: {}", i, r) << std::endl;
	}

	// ArcTan 테이블 생성 테스트
	std::wcout << L"Testing ArcTan table generation:" << std::endl;
	int curr = 0;
	int count = 0;
	MathUtil::MakeArcTanTable(
		[&curr, &count](Angle v)
		{
			if ((v / 1024) != (curr / 1024))
			{
				curr = v;
				if (count > 0) std::wcout << std::endl;
			}
			if (count < 20) // 처음 20개만 출력
			{
				std::wcout << std::format(L"{}, ", v);
				count++;
			}
		}
	);
	std::wcout << L"..." << std::endl;

	// 각도 변환 테스트
	std::wcout << L"Testing angle conversion functions:" << std::endl;
	for (int i = -32768; i <= 32767; i = i + 8192)
	{
		auto v = MathUtil::GetVec1024FromAngle(Angle(i));
		auto s = MathUtil::Sin(Angle(i));
		auto a = MathUtil::ArcTan(v.first, v.second);
		std::wcout << std::format(L"  Angle {}: x:{}, y:{}, sin:{}, atan:{}", 
			i, v.first, v.second, s, a) << std::endl;
	}

	std::wcout << L"=== 랜덤 테스트 완료 ===" << std::endl;
}

void TestRandom::OnUpdate()
{
}
