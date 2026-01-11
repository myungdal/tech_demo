// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <cstddef>
#include <algorithm>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 적응형 랜덤 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 실패 횟수에 따라 확률이 변하는 적응형 랜덤 생성기.
// - 책임: 실패 횟수 기반 동적 확률 조정
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class COMMON_API AdaptiveRandom final
{
private:
	std::size_t mThreshold = 0;        // 임계값 (이 값 이하는 시작 확률 사용)
	std::size_t mRamp = 1;             // 램프 단계 수 (임계값 초과 후 확률 변화 단계)
	std::size_t mStartProb = 0;        // 시작 확률 [0, SCALE]
	std::size_t mEndProb = 1000000;    // 최종 확률 [0, SCALE]

	static constexpr std::size_t SCALE = 1000000;  // 확률 해상도

public:
	AdaptiveRandom() = default;
	AdaptiveRandom(std::size_t threshold, std::size_t ramp, std::size_t startProb = 0, std::size_t endProb = SCALE);

	// 실패 횟수에 따라 확률을 계산하고 성공 여부를 반환한다.
	// - failCount: 실패 횟수
	// - rng: 랜덤 생성기 (size_t operator()(size_t bound) -> [0, bound) 형태)
	// - return: 성공 시 true
	template<class _Urng>
	bool Try(std::size_t failCount, _Urng& rng) const;
	
	// 현재 확률 값을 반환한다 (디버깅용).
	// - failCount: 실패 횟수
	// - return: 현재 확률 [0, SCALE]
	std::size_t GetCurrentProbability(std::size_t failCount) const;
};


template<class _Urng>
bool AdaptiveRandom::Try(std::size_t failCount, _Urng& rng) const
{
	std::size_t p = GetCurrentProbability(failCount);
	p = std::clamp(p, static_cast<std::size_t>(0), SCALE);

	const std::size_t bound = SCALE;
	const std::size_t hit = rng(bound); // 0..bound-1
	return hit < p;
}

inline std::size_t AdaptiveRandom::GetCurrentProbability(std::size_t failCount) const
{
	// [WHY] 임계값 이하는 시작 확률 사용
	if (failCount <= mThreshold)
	{
		return mStartProb;
	}
	else
	{
		const std::size_t over = failCount - mThreshold;
		// [WHY] 선형 보간: start에서 end로 mRamp 단계에 걸쳐 변화
		const std::size_t t = std::min(over, mRamp);
		
		// [WHY] 오버플로우 방지를 위해 부호 있는 정수로 계산
		const std::ptrdiff_t diff = static_cast<std::ptrdiff_t>(mEndProb) - static_cast<std::ptrdiff_t>(mStartProb);
		const std::ptrdiff_t result = static_cast<std::ptrdiff_t>(mStartProb) + (static_cast<std::ptrdiff_t>(t) * diff) / static_cast<std::ptrdiff_t>(mRamp);
		
		// [WHY] 결과를 0과 SCALE 사이로 클램프
		return static_cast<std::size_t>(std::clamp(result, static_cast<std::ptrdiff_t>(0), static_cast<std::ptrdiff_t>(SCALE)));
	}
}
