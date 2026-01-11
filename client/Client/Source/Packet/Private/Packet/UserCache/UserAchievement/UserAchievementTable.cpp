// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserAchievement/UserAchievementTable.h"

#include "Packet/UserCache/UserAchievement/UserAchievementRow.h"


UserAchievementRowPtr UserAchievementTable::Find(AchievementSid achievementSid)
{
	auto it = mTable.find(achievementSid);

	return (it != mTable.end()) ? it->second : nullptr;
}

void UserAchievementTable::Upsert(UserAchievementRowPtr data)
{
	const AchievementSid achievementSid = data->Data().Get_c_achievement_sid();

	auto it = mTable.find(achievementSid);

	if(it != mTable.end())
	{
		it->second = data;
	}
	else
	{
		mTable.emplace(achievementSid, data);
	}
}
