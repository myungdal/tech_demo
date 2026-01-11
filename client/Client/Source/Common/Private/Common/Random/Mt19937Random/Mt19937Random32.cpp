// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Random/Mt19937Random/Mt19937Random32.h"


Mt19937Random32::Mt19937Random32()
{
	// [WHY] random_device로 시드 초기화 (하드웨어 엔트로피 사용)
	std::random_device rd;
	mRandomEngine.seed(rd());
}

uint32_t Mt19937Random32::operator()()
{
	return mRandomEngine();
}

int32_t Mt19937Random32::operator()(int32_t min, int32_t max)
{
	// [EDGE-CASE] min >= max인 경우 min 반환
	if (min >= max)
		return min;
	
	std::uniform_int_distribution<int32_t> dist(min, max - 1);

	return dist(mRandomEngine);
}

uint32_t Mt19937Random32::operator()(uint32_t range)
{
	std::uniform_int_distribution<int32_t> dist(0, range - 1);

	return dist(mRandomEngine);
}
