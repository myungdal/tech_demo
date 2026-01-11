// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import FrontServer;

#include "ChattingDelayCalculator.h"


ChattingDelayCalculator::ChattingDelayCalculator()
{
	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		mChattingRatePerSecResult[i] = 0.f;
	}
}
void ChattingDelayCalculator::Update()
{
	mChattingRatePerSec[tThreadId].Update(1, tClock.GetLocalNowMs());
	mChattingRatePerSecResult[tThreadId] = mChattingRatePerSec[tThreadId].GetUpdatePerTime();
}

ClockMs ChattingDelayCalculator::CalculateChattingDelay() const
{
	float chattingPerSecResult = 0.f;

	for (ThreadId i = 0; i < gWorkerThreadCount; ++i)
	{
		chattingPerSecResult += mChattingRatePerSecResult[i];
	}

	const ClockMs delay = CLOCK_MS_FROM_DR(MIN_DELAY_MS + (chattingPerSecResult - MIN_CHAT_RATE) / RATE_RANGE * DELAY_RANGE_MS);

	return (MIN_DELAY_MS > delay) ? MIN_DELAY_MS : delay;
}
