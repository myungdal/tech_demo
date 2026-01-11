// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


struct RewardDoc;
class UserDbSession;
class UserCacheAccessor;

namespace RewardUtil
{
	Result RewardToUser(UserDbSession& userDbSession, UserCacheAccessor& userCache, const RewardDoc* rewardDoc, size_t rewardGroupIdx = INVALID_REWARD_GROUP_IDX);
}

