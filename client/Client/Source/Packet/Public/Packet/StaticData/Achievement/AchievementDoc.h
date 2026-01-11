// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

	#pragma once


class STATIC_ACHIEVEMENT;
class STATIC_ACHIEVEMENT_STEP;
struct RewardDoc;

struct AchievementDoc
{
	const STATIC_ACHIEVEMENT* mStaticAchievement = nullptr;
	std::vector<const STATIC_ACHIEVEMENT_STEP*> mStaticAchievementStepList;
	std::vector<const RewardDoc*> mRewardDocList;
};
