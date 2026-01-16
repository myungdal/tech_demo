// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 결정론적 랜덤 생성기 (32비트)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 선형 합동 생성기(LCG) 기반 결정론적 랜덤 생성기.
// - 책임: 시드 기반 재현 가능한 랜덤 값 생성
// - 범위: 0 ~ 0x1FFFC (131068)
class COMMON_API DeterministicRandom32 final
{
private:
	static constexpr uint32_t MULTIPLIER = 1103515245u;  // LCG 곱셈 상수
	static constexpr uint32_t INCREMENT = 12345u;        // LCG 증가 상수

private:
	uint32_t mSeed;  // 현재 시드 값

public:
	explicit DeterministicRandom32(uint32_t seed);

	// 시드를 설정한다.
	// - seed: 새로운 시드 값
	void SetSeed(uint32_t seed) { mSeed = seed; }

	// 현재 시드를 반환한다.
	// - return: 시드 값
	uint32_t GetSeed() const { return mSeed; }

	// 랜덤 값을 생성한다.
	// - return: 랜덤 값 (0 ~ 0x1FFFC)
	uint32_t operator()();

	// 범위 내 랜덤 값을 생성한다.
	// - range: 범위 (0 ~ range-1)
	// - return: 랜덤 값
	uint32_t operator()(uint32_t range);

	// 범위 내 랜덤 값을 생성한다.
	// - min: 최소값
	// - max: 최대값
	// - return: 랜덤 값 (min ~ max)
	int32_t operator()(int32_t min, int32_t max);
};
