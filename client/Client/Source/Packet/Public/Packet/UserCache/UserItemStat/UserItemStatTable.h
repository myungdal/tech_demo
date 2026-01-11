// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/UserCacheTableBase/UserCacheTableBase.h"
#include <unordered_map>


class PACKET_API UserItemStatTable final : public UserCacheTableBase
{
private:
	friend class UserCacheAccessor;
	static inline size_t mTableIdx = 0;

public:
	using StatMap = std::unordered_map<StatType, UserItemStatRowPtr>; // StatType -> Row
	using Table = std::unordered_map<ItemId, StatMap>;                // ItemId -> StatMap

private:
	Table mTable;

public:
	void Clear() { mTable.clear(); }
	Table& Data() { return mTable; }
	const Table& Data() const { return mTable; }

public:
	void Upsert(const UserItemStatRowPtr& row);
};
