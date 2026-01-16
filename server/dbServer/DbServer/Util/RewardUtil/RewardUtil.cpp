// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;
import ServerEngineCommon;

#include "RewardUtil.h"

#include "DbServer/Util/DbUserUtil/DbUserUtil.h"


namespace RewardUtil
{
	Result RewardExp(UserDbSession& userDbSession, UserCacheAccessor& userCache, const RewardDoc* reward)
	{
		const UserExp rewardExp = reward->mStaticReward->Get_c_user_exp();		
		const Result result = DbUserUtil::IncreaseUserExp(userDbSession, userCache, rewardExp);
		return result;
	}

	Result RewardGroupAll(UserDbSession& userDbSession, UserCacheAccessor& userCache, const RewardGroupDoc* rewardGroupDoc, MAYBE_UNUSED size_t rewardGroupIdx)
	{
		Result result = Result::SUCCEEDED;

		for (size_t i = 0; i < rewardGroupDoc->mStaticRewardGroupList.size(); ++i)
		{
			const STATIC_REWARD_GROUP* staticRewardGroup = rewardGroupDoc->mStaticRewardGroupList[i];
			const ItemDoc* itemDoc = rewardGroupDoc->mItemDocList[i];
			const STATIC_ITEM* staticItem = itemDoc->mStaticItem;
			const ItemQuantity itemQuantity = staticRewardGroup->Get_c_item_quantity();

			result = ItemUtil::CreateItem(userDbSession, userCache, staticItem, itemQuantity);
			if (Result::SUCCEEDED != result)
				break;
		}

		return result;
	}

	Result RewardGroupPick(UserDbSession& userDbSession, UserCacheAccessor& userCache, const RewardGroupDoc* rewardGroupDoc, size_t rewardGroupIdx)
	{
		Result result = Result::SUCCEEDED;

		do
		{
			if (rewardGroupDoc->mStaticRewardGroupList.size() <= rewardGroupIdx)
			{
				result = Result::FATAL_ERROR;
				break;
			}

			const STATIC_REWARD_GROUP* staticRewardGroup = rewardGroupDoc->mStaticRewardGroupList[rewardGroupIdx];
			const ItemDoc* itemDoc = rewardGroupDoc->mItemDocList[rewardGroupIdx];
			const STATIC_ITEM* staticItem = itemDoc->mStaticItem;
			const ItemQuantity itemQuantity = staticRewardGroup->Get_c_item_quantity();

			result = ItemUtil::CreateItem(userDbSession, userCache, staticItem, itemQuantity);
			if (Result::SUCCEEDED != result)
				break;

		} while (false);

		return result;
	}

	Result RewardGroupProbability(UserDbSession& userDbSession, UserCacheAccessor& userCache, const RewardGroupDoc* rewardGroupDoc, MAYBE_UNUSED size_t rewardGroupIdx)
	{
		Result result = Result::SUCCEEDED;

		for (size_t i = 0; i < rewardGroupDoc->mStaticRewardGroupList.size(); ++i)
		{
			const STATIC_REWARD_GROUP* staticRewardGroup = rewardGroupDoc->mStaticRewardGroupList[i];

			const Probability rand = (*tRandomEngine64)(0, 10000 - 1);
			if (staticRewardGroup->Get_c_reward_value() <= rand)
				continue;

			const ItemDoc* itemDoc = rewardGroupDoc->mItemDocList[i];
			const STATIC_ITEM* staticItem = itemDoc->mStaticItem;
			const ItemQuantity itemQuantity = staticRewardGroup->Get_c_item_quantity();

			result = ItemUtil::CreateItem(userDbSession, userCache, staticItem, itemQuantity);

			if (Result::SUCCEEDED != result)
				break;
		}

		return result;
	}

	Result RewardGroupRatio(UserDbSession& userDbSession, UserCacheAccessor& userCache, const RewardGroupDoc* rewardGroupDoc, MAYBE_UNUSED size_t rewardGroupIdx)
	{
		Result result = Result::SUCCEEDED;

		do
		{
			if (false == rewardGroupDoc->mRewardGroupTable.Pick(rewardGroupIdx, *tRandomEngine64))
			{
				result = Result::FATAL_ERROR;
				break;
			}

			const STATIC_REWARD_GROUP* staticRewardGroup = rewardGroupDoc->mStaticRewardGroupList[rewardGroupIdx];
			const ItemDoc* itemDoc = rewardGroupDoc->mItemDocList[rewardGroupIdx];
			const STATIC_ITEM* staticItem = itemDoc->mStaticItem;
			const ItemQuantity itemQuantity = staticRewardGroup->Get_c_item_quantity();

			result = ItemUtil::CreateItem(userDbSession, userCache, staticItem, itemQuantity);
			if (Result::SUCCEEDED != result)
				break;

		} while (false);

		return result;
	}
}

Result RewardUtil::RewardToUser(UserDbSession& userDbSession, UserCacheAccessor& userCache, const RewardDoc* rewardDoc, size_t rewardGroupIdx)
{
	static std::function<Result(UserDbSession&, UserCacheAccessor&, const RewardGroupDoc*, size_t)> RewardGroupFuncArray[] = {
		&RewardUtil::RewardGroupAll,
		&RewardUtil::RewardGroupPick,
		&RewardUtil::RewardGroupProbability,
		&RewardUtil::RewardGroupRatio
	};

	Result result = Result::SUCCEEDED;

	do 
	{
		result = RewardUtil::RewardExp(userDbSession, userCache, rewardDoc);
		if (Result::SUCCEEDED != result)
			break;

		const RewardType rewardType = rewardDoc->mStaticReward->Get_c_reward_type();
		if (RewardType::MAX <= rewardType)
			break;

		result = (RewardGroupFuncArray[_IDX(rewardType)])(userDbSession, userCache, rewardDoc->mRewardGroupDoc, rewardGroupIdx);
		if (Result::SUCCEEDED != result)
			break;

	} while (false);

	return result;
}

