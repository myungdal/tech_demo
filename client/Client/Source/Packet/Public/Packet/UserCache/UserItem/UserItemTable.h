// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/UserCacheTableBase/UserCacheTableBase.h"


class PACKET_API UserItemTable final : public UserCacheTableBase
{
private:
	friend class UserCacheAccessor;
	static inline size_t mTableIdx = 0;

public:
	using Table = std::unordered_map<ItemId, UserItemRowPtr>;

private:
	Table mTable;

public:
	void Clear() { mTable.clear(); }
	Table& Data() { return mTable; }
	const Table& Data() const { return mTable; }

public:
	UserItemRowPtr Find(ItemId itemId);
	UserItemRowPtr FindItemByItemSid(ItemSid itemSid);
	UserItemRowPtr FindItemToStack(ItemSid itemSid, ItemQuantity maxItemQuantity);
	void Upsert(UserItemRowPtr row);
};
