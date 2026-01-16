// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserAchievement/UserAchievementRow.h"

#include "Packet/StaticData/Achievement/AchievementDocLookup.h"
#include "Packet/StaticData/Achievement/AchievementDoc.h"
#include "Packet/StaticDataAccessor/StaticDataAccessor.h"
#include "Packet/StaticDataCatalog/StaticDataCatalog.h"


UserAchievementRow::UserAchievementRow(PacketKeep<ACHIEVEMENT>&& data)
	:
	mData(std::move(data))
{
}
const AchievementDoc* UserAchievementRow::GetDoc()
{
	if (!mDoc)
	{
		if (std::shared_ptr<AchievementDocLookup> view = gStaticDataAccessor->Get<AchievementDocLookup>())
		{
			const ACHIEVEMENT& achievement = Data();

			if (const AchievementDoc* doc = view->Find(achievement.Get_c_achievement_sid()))
			{
				mDoc = doc;
			}
		}
	}

	return mDoc;
}
