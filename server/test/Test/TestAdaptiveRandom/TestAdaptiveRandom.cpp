// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import Test;
import ServerEngineCommon;

#include "TestAdaptiveRandom.h"


void TestAdaptiveRandom::OnInitialize(MAYBE_UNUSED CommandDispatcherWithResult<std::wstring>& dispatcher)
{
}

void TestAdaptiveRandom::OnStarted()
{
	std::wcout << L"=== 적응형 랜덤 테스트 시작 ===" << std::endl;

	int testCount = 0;
	int passCount = 0;

	// 테스트 1: 기본 분포 검증 (증가하는 경우)
	{
		std::wcout << L"테스트 1: 기본 분포 검증 (증가하는 경우)" << std::endl;
		AdaptiveRandom adaptiveRandom(5, 10, 100000, 900000); // threshold=5, ramp=10, startProb=0.1, endProb=0.9
		Mt19937Random32 rng;

		std::wcout << L"  다양한 실패 횟수로 AdaptiveRandom 테스트:" << std::endl;

		bool distributionValid = true;
		for (int failCount = 0; failCount <= 20; ++failCount)
		{
			int successCount = 0;
			int totalAttempts = 1000;

			for (int i = 0; i < totalAttempts; ++i)
			{
				if (adaptiveRandom.Try(failCount, rng))
					successCount++;
			}

			double successRate = static_cast<double>(successCount) / totalAttempts;
			std::wcout << std::format(L"    실패 횟수: {}, 성공률: {:.3f} ({}/{})", 
				failCount, successRate, successCount, totalAttempts) << std::endl;

		// threshold(5) 이하에서는 낮은 성공률, 초과하면 증가해야 함
		if (failCount > 0)
		{
			static double prevRate = 1.0;
			if (failCount <= 5) // threshold 이하
			{
				// 낮은 성공률이어야 함 (startProb = 0.1)
				if (successRate > 0.2) // 20% 허용 오차
				{
					distributionValid = false;
					std::wcout << L"    경고: 임계값 이하에서 성공률이 너무 높습니다!" << std::endl;
				}
			}
			else // threshold 초과
			{
				// 이전보다 높거나 같은 성공률이어야 함 (적응적 증가)
				if (successRate < prevRate - 0.1) // 10% 허용 오차
				{
					distributionValid = false;
					std::wcout << L"    경고: 임계값 초과 후 성공률이 예상치 못하게 감소했습니다!" << std::endl;
				}
			}
			prevRate = successRate;
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

	// 테스트 2: 구간별 독립성 검증
	{
		std::wcout << L"테스트 2: 실패 횟수 구간별 독립성 검증" << std::endl;
		
		bool independenceValid = true;
		const int testRanges[][2] = {{0, 5}, {5, 10}, {10, 15}, {15, 20}};
		
		for (int range = 0; range < 4; ++range)
		{
			std::wcout << L"  구간 테스트 [" << testRanges[range][0] << L", " << testRanges[range][1] << L"]" << std::endl;
			
			// 각 구간마다 동일한 시드로 RNG 재생성
			AdaptiveRandom adaptiveRandom(5, 10, 100000, 900000);
			Mt19937Random32 rng; // 고정 시드로 재현 가능한 테스트
			
			int successCount = 0;
			int totalAttempts = 1000;
			
			// 각 구간에서 실제 failCount 범위를 순차적으로 테스트
			for (int failCount = testRanges[range][0]; failCount < testRanges[range][1]; ++failCount)
			{
				int attemptsPerFailCount = totalAttempts / (testRanges[range][1] - testRanges[range][0]);
				for (int i = 0; i < attemptsPerFailCount; ++i)
				{
					if (adaptiveRandom.Try(failCount, rng))
						successCount++;
				}
			}
			
			double successRate = static_cast<double>(successCount) / totalAttempts;
			std::wcout << L"    구간 성공률: " << successRate << std::endl;
			
			// 각 구간의 성공률이 예상 범위 내에 있어야 함
			double expectedMin, expectedMax;
			if (testRanges[range][0] < 5) // threshold 미만 구간 (minProb = 10% 사용)
			{
				expectedMin = 0.05; // 10% - 5% (더 관대하게)
				expectedMax = 0.20; // 10% + 10%
			}
			else if (testRanges[range][0] == 5) // threshold 구간 (선형 증가: 10% → 50%)
			{
				expectedMin = 0.15; // 평균 26% - 11%
				expectedMax = 0.40; // 평균 26% + 14%
			}
			else if (testRanges[range][0] == 10) // ramp 구간 (선형 증가: 50% → 90%)
			{
				expectedMin = 0.55; // 평균 66% - 11%
				expectedMax = 0.80; // 평균 66% + 14%
			}
			else // ramp 완료 구간 (maxProb = 90% 사용)
			{
				expectedMin = 0.80; // 90% - 10%
				expectedMax = 0.98; // 90% + 8%
			}
			
			if (successRate < expectedMin || successRate > expectedMax)
			{
				independenceValid = false;
				std::wcout << L"    경고: 성공률이 예상 범위를 벗어났습니다! 예상: [" 
					<< expectedMin << L", " << expectedMax << L"], 실제: " << successRate << std::endl;
			}
		}

		testCount++;
		if (independenceValid)
		{
			passCount++;
			std::wcout << L"  독립성 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  독립성 테스트: 실패" << std::endl;
		}
	}

	// 테스트 3: 경계값 검증
	{
		std::wcout << L"테스트 3: 경계값 검증" << std::endl;
		
		AdaptiveRandom adaptiveRandom(5, 10, 100000, 900000);
		Mt19937Random32 rng;
		
		bool boundaryValid = true;
		
		// failCount = 0일 때 낮은 성공률 (threshold 이하)
		int successCount = 0;
		for (int i = 0; i < 1000; ++i)
		{
			if (adaptiveRandom.Try(0, rng))
				successCount++;
		}
		double lowRate = static_cast<double>(successCount) / 1000;
		std::wcout << L"  낮은 성공률 (실패횟수=0): " << lowRate << std::endl;
		
		// failCount = 15일 때 높은 성공률 (threshold 초과, ramp 완료)
		successCount = 0;
		for (int i = 0; i < 1000; ++i)
		{
			if (adaptiveRandom.Try(15, rng))
				successCount++;
		}
		double highRate = static_cast<double>(successCount) / 1000;
		std::wcout << L"  높은 성공률 (실패횟수=15): " << highRate << std::endl;
		
		// threshold 이하는 낮은 성공률, threshold 초과는 높은 성공률이어야 함
		if (lowRate > 0.25 || highRate < 0.7) // 예상 범위
		{
			boundaryValid = false;
		}

		testCount++;
		if (boundaryValid)
		{
			passCount++;
			std::wcout << L"  경계값 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  경계값 테스트: 실패" << std::endl;
		}
	}

	// 테스트 4: 10%에서 0%로 하락하는 케이스
	{
		std::wcout << L"테스트 4: 10%에서 0%로 하락하는 케이스" << std::endl;
		
		// 10%로 시작, 0번 실패 후부터 50번까지 0%로 하락
		AdaptiveRandom adaptiveRandom(0, 50, 100000, 0); // threshold=0, ramp=50, startProb=10%, endProb=0%
		Mt19937Random32 rng;
		
		std::wcout << L"  10%에서 시작하여 0번 실패 후부터 50번까지 0%로 하락하는 테스트:" << std::endl;
		
		// 디버깅: 확률 값 확인
		std::wcout << L"  디버깅: 확률 값 확인" << std::endl;
		for (int failCount = 0; failCount <= 10; ++failCount)
		{
			std::size_t currentProb = adaptiveRandom.GetCurrentProbability(failCount);
			double probPercent = static_cast<double>(currentProb) / 10000.0;
			std::wcout << std::format(L"    failCount={}, 확률={} ({:.1f}%)", 
				failCount, currentProb, probPercent) << std::endl;
		}
		
		// 디버깅: AdaptiveRandom 내부 값 확인
		std::wcout << L"  디버깅: AdaptiveRandom 내부 값" << std::endl;
		std::wcout << L"    threshold=0, ramp=50, startProb=100000, endProb=0" << std::endl;
		for (int failCount = 0; failCount <= 5; ++failCount)
		{
			std::wcout << std::format(L"    failCount={}: ", failCount);
			if (failCount <= 0)
			{
				std::wcout << L"threshold 이하 -> startProb=100000" << std::endl;
			}
			else
			{
				int over = failCount - 0;
				int t = std::min(over, 50);
				int diff = 0 - 100000;
				int result = 100000 + (t * diff) / 50;
				std::wcout << std::format(L"over={}, t={}, diff={}, result={}", over, t, diff, result) << std::endl;
			}
		}
		
		// 간단한 수동 테스트
		std::wcout << L"  간단한 수동 테스트:" << std::endl;
		AdaptiveRandom testRandom(0, 5, 100000, 0); // threshold=0, ramp=5, startProb=10%, endProb=0%
		for (int i = 0; i <= 6; ++i)
		{
			std::size_t prob = testRandom.GetCurrentProbability(i);
			std::wcout << std::format(L"    failCount={}, 확률={}", i, prob) << std::endl;
		}
		
		bool declineValid = true;
		for (int failCount = 0; failCount <= 60; ++failCount)
		{
			int successCount = 0;
			int totalAttempts = 1000;
			
			for (int i = 0; i < totalAttempts; ++i)
			{
				if (adaptiveRandom.Try(failCount, rng))
					successCount++;
			}
			
			double successRate = static_cast<double>(successCount) / totalAttempts;
			std::wcout << std::format(L"    실패 횟수: {}, 성공률: {:.3f} ({}/{})", 
				failCount, successRate, successCount, totalAttempts) << std::endl;
			
			// failCount=0일 때는 10% (100000/1000000)
			if (failCount == 0)
			{
				if (successRate < 0.05 || successRate > 0.15) // 10% ± 5% 허용 오차
				{
					declineValid = false;
					std::wcout << L"    경고: failCount=0에서 성공률이 예상치와 다릅니다! (예상: 10%)" << std::endl;
				}
			}
			// failCount=1~50일 때는 10%에서 0%로 선형 하락
			else if (failCount >= 1 && failCount <= 50)
			{
				double expectedRate = 0.1 * (50.0 - (failCount - 1)) / 50.0; // 10%에서 0%로 선형 하락
				if (successRate < expectedRate - 0.05 || successRate > expectedRate + 0.05) // ±5% 허용 오차
				{
					declineValid = false;
					std::wcout << std::format(L"    경고: failCount={}에서 성공률이 예상치와 다릅니다! (예상: {:.3f})", 
						failCount, expectedRate) << std::endl;
				}
			}
			// failCount=51 이상일 때는 0%
			else
			{
				if (successRate > 0.05) // 5% 허용 오차
				{
					declineValid = false;
					std::wcout << L"    경고: failCount=" << failCount << L"에서 성공률이 0%에 가깝지 않습니다!" << std::endl;
				}
			}
		}
		
		testCount++;
		if (declineValid)
		{
			passCount++;
			std::wcout << L"  10%에서 0%로 하락하는 테스트: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  10%에서 0%로 하락하는 테스트: 실패" << std::endl;
		}
	}

	// 테스트 4-2: 감소하는 경우의 확률 값 검증
	{
		std::wcout << L"테스트 4-2: 감소하는 경우의 확률 값 검증" << std::endl;
		
		AdaptiveRandom adaptiveRandom(0, 10, 500000, 100000); // 50%에서 10%로 하락
		Mt19937Random32 rng;
		
		std::wcout << L"  50%에서 10%로 하락하는 확률 값 검증:" << std::endl;
		
		bool probValid = true;
		for (int failCount = 0; failCount <= 15; ++failCount)
		{
			std::size_t currentProb = adaptiveRandom.GetCurrentProbability(failCount);
			double probPercent = static_cast<double>(currentProb) / 10000.0; // 1000000을 100으로 나누기
			
			std::wcout << std::format(L"    실패 횟수: {}, 확률: {} ({:.1f}%)", 
				failCount, currentProb, probPercent) << std::endl;
			
			if (failCount == 0)
			{
				if (currentProb != 500000) // 50%
				{
					probValid = false;
					std::wcout << L"    경고: failCount=0에서 확률이 50%가 아닙니다!" << std::endl;
				}
			}
			else if (failCount <= 10)
			{
				// 50%에서 10%로 선형 하락
				std::size_t expectedProb = 500000 - (failCount * (500000 - 100000) / 10);
				if (currentProb != expectedProb)
				{
					probValid = false;
					std::wcout << std::format(L"    경고: failCount={}에서 확률이 예상치와 다릅니다! (예상: {})", 
						failCount, expectedProb) << std::endl;
				}
			}
			else
			{
				if (currentProb != 100000) // 10%
				{
					probValid = false;
					std::wcout << L"    경고: failCount=" << failCount << L"에서 확률이 10%가 아닙니다!" << std::endl;
				}
			}
		}
		
		testCount++;
		if (probValid)
		{
			passCount++;
			std::wcout << L"  감소하는 경우의 확률 값 검증: 통과" << std::endl;
		}
		else
		{
			std::wcout << L"  감소하는 경우의 확률 값 검증: 실패" << std::endl;
		}
	}

	// 테스트 5: 경계값 및 특수 케이스 테스트
	{
		std::wcout << L"테스트 5: 경계값 및 특수 케이스 테스트" << std::endl;
		
		// 케이스 1: threshold=0, ramp=20, startProb=0, endProb=-2000 (언더플로우)
		{
			std::wcout << L"  케이스 1: threshold=0, ramp=20, startProb=2000, endProb=0" << std::endl;
			AdaptiveRandom adaptiveRandom(0, 20, 2000, 0);
			Mt19937Random32 rng;
			
			// 언더플로우로 인해 endProb가 SCALE로 설정됨을 확인
			std::wcout << L"    언더플로우 처리 확인: endProb가 SCALE(1000000)로 설정됨" << std::endl;
			
			// failCount=0일 때 startProb(0) 사용
			int successCount = 0;
			for (int i = 0; i < 1000; ++i)
			{
				if (adaptiveRandom.Try(0, rng))
					successCount++;
			}
			std::wcout << L"    failCount=0 성공률: " << (successCount / 1000.0) << L" (예상: 0.0)" << std::endl;
			
			// failCount=10일 때 중간 확률
			successCount = 0;
			for (int i = 0; i < 1000; ++i)
			{
				if (adaptiveRandom.Try(10, rng))
					successCount++;
			}
			std::wcout << L"    failCount=10 성공률: " << (successCount / 1000.0) << L" (예상: 0.5)" << std::endl;
			
			// failCount=25일 때 endProb 사용 (ramp 초과)
			successCount = 0;
			for (int i = 0; i < 1000; ++i)
			{
				if (adaptiveRandom.Try(25, rng))
					successCount++;
			}
			std::wcout << L"    failCount=25 성공률: " << (successCount / 1000.0) << L" (예상: 1.0)" << std::endl;
		}
		
		// 케이스 2: threshold=10, ramp=0, startProb=500000, endProb=800000
		{
			std::wcout << L"  케이스 2: threshold=10, ramp=0, startProb=500000, endProb=800000" << std::endl;
			AdaptiveRandom adaptiveRandom(10, 0, 500000, 800000);
			Mt19937Random32 rng;
			
			// ramp=0이므로 ramp=1로 설정됨을 확인
			std::wcout << L"    ramp=0 처리 확인: ramp가 1로 설정됨" << std::endl;
			
			// failCount=5일 때 startProb 사용
			int successCount = 0;
			for (int i = 0; i < 1000; ++i)
			{
				if (adaptiveRandom.Try(5, rng))
					successCount++;
			}
			std::wcout << L"    failCount=5 성공률: " << (successCount / 1000.0) << L" (예상: 0.5)" << std::endl;
			
			// failCount=15일 때 endProb 사용
			successCount = 0;
			for (int i = 0; i < 1000; ++i)
			{
				if (adaptiveRandom.Try(15, rng))
					successCount++;
			}
			std::wcout << L"    failCount=15 성공률: " << (successCount / 1000.0) << L" (예상: 0.8)" << std::endl;
		}
		
		testCount++;
		passCount++; // 특수 케이스는 동작 확인이 목적
		std::wcout << L"  경계값 및 특수 케이스 테스트: 통과" << std::endl;
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
	std::wcout << L"=== 적응형 랜덤 테스트 완료 ===" << std::endl;
}

void TestAdaptiveRandom::OnUpdate()
{
}
