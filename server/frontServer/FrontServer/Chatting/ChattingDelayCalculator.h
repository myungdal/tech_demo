// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Time/ValuePerTime.h"


// 채팅 지연 시간을 계산하는 클래스
// 채팅 빈도에 따라 동적으로 지연 시간을 조절하여 서버 부하를 관리
class ChattingDelayCalculator
{
private:
	static constexpr ClockMs MIN_DELAY_MS = 1000ms;		// 최소 지연 시간
	static constexpr ClockMs MAX_DELAY_MS = 15000ms;	// 최대 지연 시간
	static constexpr float MIN_CHAT_RATE = 10.f;		// 최소 채팅 빈도
	static constexpr float MAX_CHAT_RATE = 1024.f;		// 최대 채팅 빈도
	static constexpr int MAX_CALC_COUNT = 1000;			// 최대 계산 카운트
	static constexpr float RATE_RANGE = (MAX_CHAT_RATE - MIN_CHAT_RATE);
	static constexpr ClockMs DELAY_RANGE_MS = (MAX_DELAY_MS - MIN_DELAY_MS);

	std::array<ValuePerTime<1024>, MAX_THREAD_COUNT> mChattingRatePerSec;
	std::array<std::atomic<float>, MAX_THREAD_COUNT> mChattingRatePerSecResult;
	
public:
	ChattingDelayCalculator();

	// 채팅 발생 시 호출하여 빈도 업데이트
	void Update();

	// 현재 채팅 빈도에 따른 지연 시간 계산
	ClockMs CalculateChattingDelay() const;
};

inline std::shared_ptr<ChattingDelayCalculator> gChattingDelayCalculator = nullptr;
