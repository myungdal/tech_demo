// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mersenne Twister 랜덤 생성기 (32비트)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 표준 라이브러리 std::mt19937 기반 랜덤 생성기.
// - 책임: 고품질 랜덤 값 생성
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class COMMON_API Mt19937Random32 final
{
private:
	std::mt19937 mRandomEngine;  // Mersenne Twister 엔진

public:
	Mt19937Random32();

public:
	// 랜덤 값을 생성한다.
	// - return: 랜덤 값 (uint32_t 범위)
	uint32_t operator()();

	// 범위 내 랜덤 값을 생성한다.
	// - min: 최소값
	// - max: 최대값
	// - return: 랜덤 값 (min ~ max)
	int32_t operator()(int32_t min, int32_t max);

	// 범위 내 랜덤 값을 생성한다.
	// - range: 범위 (0 ~ range-1)
	// - return: 랜덤 값
	uint32_t operator()(uint32_t range);
};
