// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/TimeStep/TimeStepInternal/TimeStepQueue.h"


TimeStepQueue::TimeStepQueue(int64_t interval, ClockPoint now)
	:
	mInterval(interval),
	mStartTime(now)
{
}
void TimeStepQueue::Reset(ClockPoint now)
{
	mStartTime = now;
}

void TimeStepQueue::PushStep(TimeStep count)
{
	mQueuedStepCount += count;
}

TimeStep TimeStepQueue::PopStep(ClockPoint now, double speed)
{
	// 시작을 안했음.
	if (now <= mStartTime)
		return INVALID_TIME_STEP;

	const ClockMs pastTimeStep = ClockMs(static_cast<int64_t>(mPastStepCount * mInterval));
	const ClockPoint timeStepNow = (mStartTime + pastTimeStep);

	// 시간이 흐르지 않았음.
	if (now <= timeStepNow)
		return INVALID_TIME_STEP;

	const ClockMs deltaMs = CLOCK_MS_FROM_DR((now - timeStepNow) * speed);
	
	// 흐른 스텝 계산.
	TimeStep stepCount = static_cast<TimeStep>(deltaMs.count() / mInterval);

	if (stepCount > mQueuedStepCount)
		stepCount = mQueuedStepCount;

	mPastStepCount += stepCount;

	// 스텝 큐에서 차감.
	mQueuedStepCount -= stepCount;

	return stepCount;
}
