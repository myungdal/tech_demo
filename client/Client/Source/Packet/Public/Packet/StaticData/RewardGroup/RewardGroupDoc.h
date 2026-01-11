// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Random/RatioTable/RatioTable.h"


class STATIC_REWARD_GROUP;
struct ItemDoc;

struct RewardGroupDoc
{
	std::vector<const STATIC_REWARD_GROUP*> mStaticRewardGroupList;
	std::vector<const ItemDoc*> mItemDocList;
	RatioTable<size_t> mRewardGroupTable;
};
