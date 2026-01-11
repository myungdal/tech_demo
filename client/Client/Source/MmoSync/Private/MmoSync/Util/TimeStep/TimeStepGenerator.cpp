// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/TimeStep/TimeStepGenerator.h"


TimeStepGenerator::TimeStepGenerator(int64_t interval, ClockPoint now)
	:
	mInterval(interval),
	mStartTime(now)
{
}
void TimeStepGenerator::Reset(ClockPoint now)
{
	mStartTime = now;
}

TimeStep TimeStepGenerator::Generate(ClockPoint now) 
{
	if (now <= mStartTime)
		return 0;

	const ClockPoint timeStepNow = (mStartTime + ClockMs(mGeneratedStepCount * mInterval));
	const ClockMs deltaMs = CLOCK_MS_FROM_DR((now > timeStepNow) ? (now - timeStepNow) : 0ms);

	if (deltaMs.count() <= 0)
		return 0;

	TimeStep stepCount = (deltaMs.count() / mInterval);
	mGeneratedStepCount += stepCount;

	//_DEBUG_LOG(RED, L"stepCount: {}", stepCount);

	if (stepCount > TIME_STEP_GENERATOR_LIMIT)
		stepCount = TIME_STEP_GENERATOR_LIMIT;

	return stepCount;
}
