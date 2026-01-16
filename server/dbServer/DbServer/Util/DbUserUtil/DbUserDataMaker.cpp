// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;

#include "DbUserDataMaker.h"


DbUserDataMaker::DbUserDataMaker(UserCacheAccessor& userCache)
	: 
	mUserCache(userCache),
	mAchievementIter(mUserCache.Get<UserAchievementTable>()->Data().begin()),
	mItemIter(mUserCache.Get<UserItemTable>()->Data().begin()),
	mMissionIter(mUserCache.Get<UserMissionTable>()->Data().begin()),
	mQuestIter(mUserCache.Get<UserQuestTable>()->Data().begin())
{
}
USER_DATA_WRITER DbUserDataMaker::MakeUserData(uint8_t* buf, OUT bool& isCompleted)
{
	USER_DATA::Writer wp(PARAM, buf);
	isCompleted = true;
	{
		auto& table = mUserCache.Get<UserAchievementTable>()->Data();
		auto achievementEnd = table.end();
		while (mAchievementIter != achievementEnd)
		{
			if (wp.GetPacketSize() > MAX_PACKET_SAFE_DATA_SIZE)
			{
				isCompleted = false;
				break;
			}

			UserAchievementRowPtr& row = mAchievementIter->second;
			ACHIEVEMENT& data = row->Data();
			wp.Write_achievementList(data);

			++mAchievementIter;
		}
	}

	//UserItemTable
	//UserMissionTable
	//UserQuestTable
	
	return USER_DATA::Writer(wp.GetPacketBufPtr());
}

