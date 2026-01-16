// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/UserCacheTableBase/UserCacheTableBase.h"


class PACKET_API UserAchievementTable final : public UserCacheTableBase
{
private:
	friend class UserCacheAccessor;
	static inline size_t mTableIdx = 0;

public:
	using Table = std::map<AchievementSid, UserAchievementRowPtr>;

private:
	Table mTable;

public:
	void Clear() { mTable.clear(); }
	Table& Data() { return mTable; }

public:
	UserAchievementRowPtr Find(AchievementSid achievementSid);
	void Upsert(UserAchievementRowPtr data);
}; 
