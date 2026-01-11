// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;
import ServerEngineCommon;

#include "TestNonReplacementSampler.h"


void TestNonReplacementSampler::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestNonReplacementSampler::OnStarted()
{
	std::wcout << L"=== 비복원 샘플러 테스트 시작 ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	// 테스트 1: 기본 분포 검증
	{
		std::wcout << L"테스트 1: 기본 분포 검증" << std::endl;
		
		NonReplaceSampler<uint32_t> sampler(100);
		sampler.Add(1, 333);
		sampler.Add(2, 333);
		sampler.Add(3, 333);
		sampler.Add(4, 1);

		std::wcout << L"  NonReplaceSampler 구성: 1(333), 2(333), 3(333), 4(1)" << std::endl;

		Mt19937Random64 random;
		std::map<uint32_t, uint32_t> resultList;

		std::wcout << L"  100회 샘플링 반복 시작..." << std::endl;

		for (int i = 0; i < 100; ++i)
		{
			uint32_t result = 0;

			if (sampler.Pick(OUT result, random))
			{
				++resultList[result];
			}
			else
			{
				std::wcout << L"  오류: 반복 " << i << L"에서 Pick 실패" << std::endl;
				break;
			}

			// 진행 상황 표시 (1000번마다)
			if ((i + 1) % 1000 == 0)
			{
				std::wcout << L"    " << (i + 1) << L"회 반복 완료..." << std::endl;
			}
		}

		// 결과 통계 출력 및 검증
		std::wcout << L"  샘플링 결과:" << std::endl;
		bool distributionValid = true;
		double totalWeight = 1000.0; // 333+333+333+1
		
		for (const auto& [value, count] : resultList)
		{
			double percentage = static_cast<double>(count) / 10000.0 * 100.0;
			double expectedWeight = (value == 4) ? 1.0 : 333.0;
			double expectedPercentage = expectedWeight / totalWeight * 100.0;
			double tolerance = 5.0; // 5% 허용 오차
			
			std::wcout << L"    값 " << value << L": " << count << L"회 (" 
				<< std::format(L"{:.2f}%", percentage) << L", 예상: " 
				<< std::format(L"{:.2f}%", expectedPercentage) << L")" << std::endl;
			
			if (std::abs(percentage - expectedPercentage) > tolerance)
			{
				distributionValid = false;
				std::wcout << L"    경고: 분포가 예상과 다릅니다!" << std::endl;
			}
		}

		testCount++;
		if (distributionValid)
		{
			passCount++;
			std::wcout << L"  기본 분포 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  기본 분포 테스트: 실패" << std::endl;
		}
	}

	// 테스트 2: 직렬화/역직렬화 안정성 검증
	{
		std::wcout << L"테스트 2: 직렬화/역직렬화 안정성 검증" << std::endl;
		
		NonReplaceSampler<uint32_t> sampler(100);
		sampler.Add(1, 100);
		sampler.Add(2, 200);
		sampler.Add(3, 300);
		
		// 초기화를 위해 Clear() 호출
		sampler.Clear();

		Mt19937Random64 random;
		
		bool serializationValid = true;
		const int testIterations = 1000;
		
		for (int i = 0; i < testIterations; ++i)
		{
			// 직렬화/역직렬화 테스트 (Pick 전에 수행)
			std::wstring serialized = sampler.Serialize();
			sampler.Deserialize(serialized);
			
			// 상태가 일치하는지 확인
			std::wstring afterDeserialize = sampler.Serialize();
			if (serialized != afterDeserialize)
			{
				serializationValid = false;
				std::wcout << L"    오류: 반복 " << i << L"에서 역직렬화 후 상태 불일치" << std::endl;
				std::wcout << L"    직렬화 전: " << serialized << std::endl;
				std::wcout << L"    역직렬화 후: " << afterDeserialize << std::endl;
				break;
			}
			
			// 샘플링 수행
			uint32_t result = 0;
			if (!sampler.Pick(OUT result, random))
			{
				serializationValid = false;
				std::wcout << L"    오류: 반복 " << i << L"에서 Pick 실패" << std::endl;
				break;
			}
		}

		testCount++;
		if (serializationValid)
		{
			passCount++;
			std::wcout << L"  직렬화 안정성 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  직렬화 안정성 테스트: 실패" << std::endl;
		}
	}

	// 테스트 3: 분포 일관성 검증
	{
		std::wcout << L"테스트 3: 분포 일관성 검증" << std::endl;
		
		Mt19937Random64 random;
		
		NonReplaceSampler<uint32_t> sampler(100);
		sampler.Add(1, 50);
		sampler.Add(2, 50);
		
		bool consistencyValid = true;
		const int testIterations = 1000;
		int count1 = 0, count2 = 0;
		
		for (int i = 0; i < testIterations; ++i)
		{
			uint32_t result = 0;
			
			if (!sampler.Pick(OUT result, random))
			{
				consistencyValid = false;
				break;
			}
			
			if (result == 1) count1++;
			else if (result == 2) count2++;
		}
		
		// 50:50 비율이므로 각각 400-600 범위에 있어야 함 (1000 ± 100)
		if (count1 < 400 || count1 > 600 || count2 < 400 || count2 > 600)
		{
			consistencyValid = false;
			std::wcout << L"    오류: 분포 불일치 - Count1: " << count1 
				<< L", Count2: " << count2 << std::endl;
		}
		
		testCount++;
		if (consistencyValid)
		{
			passCount++;
			std::wcout << L"  분포 일관성 테스트: 통과 (Count1: " << count1 
				<< L", Count2: " << count2 << L")" << std::endl;
		}
		else
		{
			std::wcout << L"  분포 일관성 테스트: 실패" << std::endl;
		}
	}

	// 테스트 4: 빈 샘플러 처리
	{
		std::wcout << L"테스트 4: 빈 샘플러 처리" << std::endl;
		
		NonReplaceSampler<uint32_t> emptySampler(100);
		Mt19937Random64 random;
		
		uint32_t result = 0;
		bool pickResult = emptySampler.Pick(OUT result, random);
		
		testCount++;
		if (!pickResult)
		{
			passCount++;
			std::wcout << L"  빈 샘플러 테스트: 통과 (올바르게 선택 실패)" << std::endl;
		}
		else
		{
			std::wcout << L"  빈 샘플러 테스트: 실패 (빈 샘플러에서 선택하면 안됨)" << std::endl;
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
	std::wcout << L"=== 비복원 샘플러 테스트 완료 ===" << std::endl;
}

void TestNonReplacementSampler::OnUpdate()
{
}
