// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Random/DeterministicRandom/DeterministicRandom32.h"


DeterministicRandom32::DeterministicRandom32(uint32_t seed)
	:
	mSeed(seed)
{
}
uint32_t DeterministicRandom32::operator()()
{
	// [WHY] 선형 합동 생성기(LCG) 알고리즘: seed = seed * multiplier + increment
	mSeed = mSeed * MULTIPLIER + INCREMENT;
	return mSeed;
}

uint32_t DeterministicRandom32::operator()(uint32_t range)
{
	// [EDGE-CASE] 범위가 0이면 0 반환
	if (range == 0)
		return 0;

	// [WHY] 모듈로 편향을 피하기 위한 임계값 계산
	const uint32_t threshold = static_cast<uint32_t>(-static_cast<int32_t>(range));

	uint32_t result;
	do
	{
		result = (*this)();
	} while (result >= threshold);

	return result % range;
}

int32_t DeterministicRandom32::operator()(int32_t min, int32_t max)
{
	// [EDGE-CASE] min > max인 경우 교환
	if (min > max)
		std::swap(min, max);

	uint32_t diff = static_cast<uint32_t>(max) - static_cast<uint32_t>(min) + 1;
	// [EDGE-CASE] diff가 0이면 (오버플로우) 시드 값 반환
	if (diff == 0)
		return static_cast<int32_t>((*this)());

	uint64_t r = (*this)(diff);
	return static_cast<int32_t>(static_cast<uint32_t>(min) + r);
}
