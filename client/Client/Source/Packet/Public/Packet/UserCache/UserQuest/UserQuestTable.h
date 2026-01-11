// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/UserCacheTableBase/UserCacheTableBase.h"


class PACKET_API UserQuestTable final : public UserCacheTableBase
{
private:
	friend class UserCacheAccessor;
	static inline size_t mTableIdx = 0;

public:
	using Table = std::map<QuestSid, UserQuestRowPtr>;

private:
	Table mTable;

public:
	void Clear() { mTable.clear(); }
	Table& Data() { return mTable; }

public:
	UserQuestRowPtr Find(QuestSid sid);
	void Upsert(UserQuestRowPtr data);
};
