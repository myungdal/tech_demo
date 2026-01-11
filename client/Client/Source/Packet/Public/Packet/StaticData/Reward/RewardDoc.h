// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Random/RatioTable/RatioTable.h"


class STATIC_REWARD;
struct RewardGroupDoc;

struct RewardDoc
{
	const STATIC_REWARD* mStaticReward = nullptr;
	const RewardGroupDoc* mRewardGroupDoc = nullptr;
};
