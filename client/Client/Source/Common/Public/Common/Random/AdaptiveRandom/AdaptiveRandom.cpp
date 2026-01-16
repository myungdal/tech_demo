// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "AdaptiveRandom.h"


AdaptiveRandom::AdaptiveRandom(std::size_t threshold, std::size_t ramp, std::size_t startProb, std::size_t endProb)
{
	mThreshold = threshold;
	mRamp = (ramp == 0 ? 1 : ramp);
	mStartProb = std::clamp(startProb, static_cast<std::size_t>(0), SCALE);
	mEndProb = std::clamp(endProb, static_cast<std::size_t>(0), SCALE);
	if (mStartProb > mEndProb) std::swap(mStartProb, mEndProb);
}
