// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserItem/UserItemTable.h"

#include "Packet/StaticData/Item/ItemDoc.h"
#include "Packet/StaticData/Item/ItemDocLookup.h"
#include "Packet/UserCacheAccessor/UserCacheAccessor.h"
#include "Packet/UserCache/UserItem/UserItemRow.h"


UserItemRowPtr UserItemTable::Find(ItemId itemId)
{
	auto it = mTable.find(itemId);

	return (it != mTable.end()) ? it->second : nullptr;
}

UserItemRowPtr UserItemTable::FindItemByItemSid(ItemSid itemSid)
{
	ItemQuantity minQuantity = INT64_MAX;
	UserItemRowPtr result = nullptr;

	for (auto& it : mTable)
	{
		UserItemRowPtr& userItemRow = it.second;
		const ITEM& item = userItemRow->Data();

		if (itemSid == item.Get_c_item_sid())
		{
			if (item.Get_c_item_quantity() == 0)
				continue;

			if (minQuantity < item.Get_c_item_quantity())
				continue;

			minQuantity = item.Get_c_item_quantity();
			result = userItemRow;
		}
	}

	return result;
}

UserItemRowPtr UserItemTable::FindItemToStack(ItemSid itemSid, ItemQuantity maxItemQuantity)
{
	ItemQuantity maxQuantity = INT64_MIN;
	UserItemRowPtr result = nullptr;

	for (auto& it : mTable)
	{
		UserItemRowPtr& userItemRowPtr = it.second;
		const ITEM& item = userItemRowPtr->Data();

		if (itemSid == item.Get_c_item_sid())
		{
			if (item.Get_c_item_quantity() >= maxItemQuantity)
				continue;

			if (maxQuantity >= item.Get_c_item_quantity())
				continue;

			maxQuantity = item.Get_c_item_quantity();
			result = userItemRowPtr;
		}
	}

	return result;
}

void UserItemTable::Upsert(UserItemRowPtr row)
{
	const ItemId& itemId = row->Data().Get_c_item_id();

	auto it = mTable.find(itemId);

	if (it != mTable.end())
		it->second = row;
	else
		mTable.emplace(itemId, row);
}
