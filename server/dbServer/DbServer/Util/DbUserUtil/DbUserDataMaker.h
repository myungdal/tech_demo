// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/UserCache/UserAchievement/UserAchievementRow.h"
#include "Packet/UserCache/UserAchievement/UserAchievementTable.h"
#include "Packet/UserCache/UserItem/UserItemRow.h"
#include "Packet/UserCache/UserItem/UserItemTable.h"
#include "Packet/UserCache/UserMission/UserMissionRow.h"
#include "Packet/UserCache/UserMission/UserMissionTable.h"
#include "Packet/UserCache/UserQuest/UserQuestRow.h"
#include "Packet/UserCache/UserQuest/UserQuestTable.h"
#include "Packet/UserCacheAccessor/UserCacheAccessor.h"


class DbUserDataMaker
{
private:
	UserCacheAccessor& mUserCache;

	UserAchievementTable::Table::iterator mAchievementIter;
	UserItemTable::Table::iterator mItemIter;
	UserMissionTable::Table::iterator mMissionIter;
	UserQuestTable::Table::iterator mQuestIter;

public:
	explicit DbUserDataMaker(UserCacheAccessor& userCache);

public:
	USER_DATA_WRITER MakeUserData(uint8_t* buf, OUT bool& isCompleted);
};
