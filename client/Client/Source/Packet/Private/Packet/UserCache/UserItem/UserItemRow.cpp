// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserItem/UserItemRow.h"

#include "Packet/StaticData/Item/ItemDocLookup.h"
#include "Packet/StaticData/Item/ItemDoc.h"
#include "Packet/StaticDataAccessor/StaticDataAccessor.h"
#include "Packet/StaticDataCatalog/StaticDataCatalog.h"


UserItemRow::UserItemRow(
	PacketKeep<ITEM>&& data, 
	UserCharacterRowPtr userCharacterRow
)
	: 
	mData(std::move(data)),
	mUserCharacterRow(userCharacterRow)
{
}
const ItemDoc* UserItemRow::GetDoc() const
{
	if (!mDoc)
	{
		if (std::shared_ptr<ItemDocLookup> view = gStaticDataAccessor->Get<ItemDocLookup>())
		{
			const ITEM& item = Data();

			if (const ItemDoc* doc = view->Find(item.Get_c_item_sid()))
			{
				mDoc = doc;
			}
		}
	}

	return mDoc;
}

Result UserItemRow::SetItemQuantity(ItemQuantity itemQuantity)
{
	if (nullptr == *mData)
		return Result::CACHE_ERROR;

	if (itemQuantity < 0)
		return Result::INVALID_REQ;

	if (itemQuantity > mDoc->mStaticItem->Get_c_max_quantity())
		return Result::INVALID_REQ;

	mData->Ref_c_item_quantity() = itemQuantity;

	return Result::SUCCEEDED;
}

Result UserItemRow::IncreaseItemQuantity(ItemQuantity itemQuantity)
{
	const Result result = SetItemQuantity(mData->Get_c_item_quantity() + itemQuantity);
	return result;
}

bool UserItemRow::CanStoneMerge(const UserItemRow& anotherStone) const
{
	if (ItemType::STONE != mDoc->mStaticItem->Get_c_item_type())
		return false;

	if (ItemType::STONE != anotherStone.mDoc->mStaticItem->Get_c_item_type())
		return false;

	const ItemColorUnion a = mData->Get_c_item_color_union();
	const ItemColorUnion b = anotherStone.mData->Get_c_item_color_union();

	for (int i = 0; i < 8; ++i)
	{
		if ((ItemColor::NONE != a.itemColor[i]) &&
			(ItemColor::NONE != b.itemColor[i]))
		{
			return false;
		}
	}

	return true;
}
