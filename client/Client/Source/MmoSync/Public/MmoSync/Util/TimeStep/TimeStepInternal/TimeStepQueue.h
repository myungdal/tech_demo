// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeStepQueue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MMOSYNC_API TimeStepQueue final
{
private:
	const int64_t mInterval = 0;
	ClockPoint mStartTime;

	TimeStep mPastStepCount = 0;
	TimeStep mQueuedStepCount = 0;

public:
	DISABLE_COPY(TimeStepQueue);
	explicit TimeStepQueue(int64_t interval, ClockPoint now);

public:
	void Reset(ClockPoint now);
	void PushStep(TimeStep count);
	TimeStep PopStep(ClockPoint now, double speed);

public:
	TimeStep GetQueuedStepCount() const { return mQueuedStepCount; }
};
