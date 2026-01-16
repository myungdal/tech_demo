// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCacheDiff/UserCacheDiff.h"

#include "Packet/UserCache/UserItem/UserItemTable.h"
#include "Packet/UserCache/UserItem/UserItemRow.h"
#include "Packet/UserCacheAccessor/UserCacheAccessor.h"


UserCacheDiff::UserCacheDiff(const std::shared_ptr<UserCacheAccessor>& userCacheAccessor)
	:
	mUserCacheAccessor(userCacheAccessor)
{
	mCheckOuted = false;
}
void UserCacheDiff::Checkout()
{
	if (mCheckOuted)
		return;

	if (mUserCacheAccessor->IsEmpty())
		return;

	const USER& user = mUserCacheAccessor->GetUser();

	mCheckOuted = true;

	mUserExp = user.Get_c_user_exp();

	mItemQuantityMap.clear();

	const UserItemTable::Table& itemTable = mUserCacheAccessor->Get<UserItemTable>()->Data();

	for (const auto& [itemId, itemRow] : itemTable)
	{
		const ITEM& item = itemRow->Data();

		mItemQuantityMap.emplace(item.Get_c_item_id(), item.Get_c_item_quantity());
	}	
}

void UserCacheDiff::Rollback()
{
	if (!mCheckOuted)
		return;

	mCheckOuted = false;

	USER& user = mUserCacheAccessor->GetUser();

	user.Ref_c_user_exp() = mUserExp;

	UserItemTable::Table& itemTable = mUserCacheAccessor->Get<UserItemTable>()->Data();

	for (auto& [itemId, itemRow] : itemTable)
	{
		ITEM& item = itemRow->Data();

		auto it = mItemQuantityMap.find(item.Get_c_item_id());

		if (mItemQuantityMap.end() == it)
		{
			item.Ref_c_item_quantity() = 0;
		}
		else
		{
			item.Ref_c_item_quantity() = it->second;
		}
	}
}

void UserCacheDiff::Commit(OUT USER_CACHE_DIFF::Writer& diff)
{
	if (!mCheckOuted)
		return;

	mCheckOuted = false;

	// 아이템 차이 취합
	mModifiedItemIdList.clear();
	mModifiedItemQuantityList.clear();
	mNewItemList.clear();

	const UserItemTable::Table& itemTable = mUserCacheAccessor->Get<UserItemTable>()->Data();

	// 아이템 차이 계산
	for (const auto& [itemId, itemRow] : itemTable)
	{
		const ITEM& item = itemRow->Data();

		auto it = mItemQuantityMap.find(item.Get_c_item_id());

		if (mItemQuantityMap.end() != it)
		{
			if (it->second != item.Get_c_item_quantity())
			{
				mModifiedItemIdList.emplace_back(item.Get_c_item_id());
				mModifiedItemQuantityList.emplace_back(item.Get_c_item_quantity());
			}
		}
		else
		{
			mNewItemList.emplace_back(item.Get_c_item_id(), itemRow);
		}
	}

	USER& user = mUserCacheAccessor->GetUser();

	diff.SetValues(
		(user.Get_c_user_exp() - mUserExp),
		mModifiedItemIdList.data(), static_cast<PacketSize>(mModifiedItemIdList.size()),
		mModifiedItemQuantityList.data(), static_cast<PacketSize>(mModifiedItemQuantityList.size())
	);

	// 새로 생성된 아이템 추가.
	for (const auto& [itemId, itemRow] : mNewItemList)
	{
		const ITEM& item = itemRow->Data();

		auto it = mItemQuantityMap.find(item.Get_c_item_id());

		if (mItemQuantityMap.end() == it)
		{
			diff.Write_itemList(item);
		}
	}
}
