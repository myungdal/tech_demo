// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 결정론적 랜덤 생성기 (64비트)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 선형 합동 생성기(LCG) 기반 결정론적 랜덤 생성기 (64비트).
// - 책임: 시드 기반 재현 가능한 랜덤 값 생성
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class COMMON_API DeterministicRandom64 final
{
private:
	static constexpr uint64_t MULTIPLIER = 6364136223846793005ull;  // LCG 곱셈 상수
	static constexpr uint64_t INCREMENT = 1442695040888963407ull;   // LCG 증가 상수

private:
	uint64_t mSeed;  // 현재 시드 값

public:
	explicit DeterministicRandom64(uint64_t seed);

	// 시드를 설정한다.
	// - seed: 새로운 시드 값
	void SetSeed(uint64_t seed) { mSeed = seed; }

	// 현재 시드를 반환한다.
	// - return: 시드 값
	uint64_t GetSeed() const { return mSeed; }
  
	// 랜덤 값을 생성한다.
	// - return: 랜덤 값 (uint64_t 범위)
	uint64_t operator()();

	// 범위 내 랜덤 값을 생성한다.
	// - range: 범위 (0 ~ range-1)
	// - return: 랜덤 값
	uint64_t operator()(uint64_t range);

	// 범위 내 랜덤 값을 생성한다.
	// - min: 최소값
	// - max: 최대값
	// - return: 랜덤 값 (min ~ max)
	int64_t operator()(int64_t min, int64_t max);
};
