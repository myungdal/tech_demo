// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;
import ServerEngineDb;
import ServerEngineCommon;

#include "ItemUtil.h"

namespace
{
	std::bitset<8> MakeRandomItemPattern()
	{
		return (*tRandomEngine32)(1, 255);
	}

	ItemColorUnion MakeRandomItemColorUnion(const std::bitset<8>& itemPattern)
	{
		ItemColorUnion itemColorUnion(0);

		for (size_t i = 0; i < 8; ++i)
		{			
			if (true == itemPattern[i])
			{
				uint32_t randValue = (*tRandomEngine32)(0, static_cast<uint32_t>(ItemColor::MAX) - 1);
				itemColorUnion.itemColor[i] = static_cast<ItemColor>(randValue);
			}
			else
			{
				itemColorUnion.itemColor[i] = ItemColor::NONE;
			}
		}

		return itemColorUnion;
	}
#if 0
	ItemStatUnion MakeRandomStatUnion(const STATIC_ITEM* staticItem, const std::bitset<8>& itemPattern)
	{
		ItemStatUnion itemStatUnion(0);

		const ItemGrade itemGrade = staticItem->Get_c_item_grade();

		const auto itemStatDocLookup = gStaticDataAccessor->Get<ItemStatDocLookup>();
		const ItemStatDoc* itemStatDoc = itemStatDocLookup->Find(itemGrade);

		if (nullptr == itemStatDoc)
		{
			return itemStatUnion;
		}

		for (size_t i = 0; i < 8; ++i)
		{
			if (true == itemPattern[i])
			{
				size_t idx = SIZE_MAX;
				itemStatDoc->mItemStatRatioTable.Pick(idx, tRandomEngine32->Generate());
				itemStatUnion.statType[i] = itemStatDoc->mStaticStatTable[idx][i]->mStaticStat->GetStat_type();
			}
			else
			{
				itemStatUnion.statType[i] = StatType::LOGGING_NONE;
			}
		}

		return itemStatUnion;
	}

#include "Packet/StaticData/ViewImpl/CharacterGachaView.h"

	CharacterSid GetRandomCharacterSid(const STATIC_ITEM* staticItem)
	{
		const ItemGrade itemGrade = staticItem->Get_c_item_grade();

		CharacterGachaViewPtr characterGachaViewPtr = gStaticDataAccessor->GetCharacterGachaView();
		const CharacterGachaDoc* characterGachaDoc = characterGachaViewPtr->Find(itemGrade);

		if (nullptr == characterGachaDoc)
		{
			return INVALID_SID;
		}

		size_t idx = SIZE_MAX;

		if (false == characterGachaDoc->mCharacterGachaRatioTable.Pick(idx, tRandomEngine32->Generate()))
		{
			return INVALID_SID;
		}

		const STATIC_CHARACTER_GACHA* staticCharacterGacha = characterGachaDoc->mStaticCharacterGachaList[idx];

		return staticCharacterGacha->GetCharacter_sid();
	}
#endif

	//ItemPattern GetRandomItemPattern(const STATIC_ITEM* staticItem)
	//{
	//	ItemPattern result = 0;
	//	ItemGrade itemGrade = staticItem->Get_c_item_grade();
	//	/*
	//	ItemGrade::BASIC		: 5개 연속 open
	//	ItemGrade::RARE			: 4개 연속 open
	//	ItemGrade::EPIC			: 3개 연속 open
	//	ItemGrade::LEGENDARY	: 2개 연속 open
	//	ItemGrade::MYTHIC		: 1개만 open
	//	*/
	// 
	//	const int32_t openCount = 6 - static_cast<int32_t>(itemGrade);
	//	const int32_t startPos = tRandomEngine32->Generate(0, 7) << 1;
	// 
	//	for (int32_t i = 0; i < openCount; ++i)
	//	{
	//		const ItemPattern color = tRandomEngine32->Generate(0, 3) + 1;
	//		const int32_t pos = ((startPos + i) & 7) << 1;
	//		result |= color << pos;
	//	}
	// 
	//	return result;
	//}
}

Result ItemUtil::CreateItem(UserDbSession& userDbSession, UserCacheAccessor& userCache, const STATIC_ITEM* staticItem, ItemQuantity itemQuantity)
{
	UserItemTablePtr userItemTable = userCache.Get<UserItemTable>();
	const ItemSid itemSid = staticItem->Get_c_item_sid();
	const ItemQuantity maxItemQuantity = staticItem->Get_c_max_quantity();

	Result result = Result::SUCCEEDED;

	while (itemQuantity > 0)
	{
		if (nullptr == userItemTable)
		{
			result = Result::DB_ERROR;
			break;
		}

		UserItemRowPtr userItemRow = userItemTable->FindItemToStack(itemSid, maxItemQuantity);

		ItemId itemId = INVALID_UUID;
		ItemQuantity original = 0;
		ItemQuantity increasement = itemQuantity;

		if (nullptr != userItemRow)
		{
			ITEM& item = userItemRow->Data();
			itemId = item.Get_c_item_id();
			original = item.Get_c_item_quantity();
		}

		ItemQuantity sum = (original + increasement);

		if (sum > maxItemQuantity)
		{
			itemQuantity = (sum - maxItemQuantity);
			increasement = (maxItemQuantity - increasement);
			sum = maxItemQuantity;
		}
		else
		{
			itemQuantity = 0;
		}

		if (nullptr != userItemRow)
		{
			SP_ITEM_UPDATE_QUANTITY spItemUpdateQuantity(
				userDbSession,
				itemId,
				sum,
				tClock.GetGlobalNowTs()
			);
			result = DbUtil::ExecuteSp(spItemUpdateQuantity);
			if (Result::SUCCEEDED != result)
				break;

			userItemRow->IncreaseItemQuantity(increasement);
		}
		else
		{
			ItemId newItemId = UuidUtil::GenerateUuid();
			ItemId parentItemId = INVALID_UUID.GetData();
			MailId parentMailId = INVALID_UUID.GetData();
			int64_t inventorySlotIndex = 0;
			int64_t quickSlotIndex = 0;
			int64_t equipSlotIndex = 0;
			const ItemLevel itemLevel = MIN_ITEM_LEVEL;
			ItemColorUnion itemColorUnion(0);
			ItemStatUnion itemStatUnion(0);
			const ItemEnchantUnion itemEnchantUnion(0);
			int64_t itemStateFlag = (int64_t)ItemStateFlag::NONE;

			if (ItemType::STONE == staticItem->Get_c_item_type())
			{
				std::bitset<8> itemPattern = MakeRandomItemPattern();
				itemColorUnion = MakeRandomItemColorUnion(itemPattern);
				//itemStatUnion = MakeRandomStatUnion(staticItem, itemPattern);
			}

			USER& user = userCache.GetUser();
			const UserId userId = user.Get_c_user_id();

			SP_ITEM_INSERT spItemInsert(userDbSession,
				userId.GetData(),
				newItemId,
				parentItemId,
				parentMailId,
				itemSid,
				inventorySlotIndex,
				quickSlotIndex,
				equipSlotIndex,
				sum,
				itemLevel,
				itemColorUnion.value,
				itemStatUnion.value,
				itemEnchantUnion.value,
				itemStateFlag,
				tClock.GetGlobalNowTs(),
				tClock.GetGlobalNowTs(),
				tClock.GetGlobalNowTs(),
				tClock.GetGlobalNowTs()
			);
			result = DbUtil::ExecuteSp(spItemInsert);
			if (Result::SUCCEEDED != result)
				break;

			SP_ITEM_SELECT spItemSelect(
				userDbSession,
				spItemInsert.mOut.p_new_item_id
			);
			result = DbUtil::ExecuteSp(spItemSelect);
			if (Result::SUCCEEDED != result)
				break;

			ITEM::Writer item(PARAM, TEMP_BUF,
				userId,
				spItemInsert.mOut.p_new_item_id,
				parentItemId,
				parentMailId,
				itemSid,
				inventorySlotIndex,
				quickSlotIndex,
				equipSlotIndex,
				sum,
				itemLevel,
				itemColorUnion,
				itemStatUnion,
				itemEnchantUnion,
				(ItemStateFlag)itemStateFlag,
				tClock.GetGlobalNowTt(),
				tClock.GetGlobalNowTt(),
				tClock.GetGlobalNowTt(),
				tClock.GetGlobalNowTt()
			);

			PacketKeep<ITEM> itemContainer(item.GetPacketBufPtr());

			UserItemRowPtr newUserItemRow = std::make_shared<UserItemRow>(std::move(itemContainer), nullptr);

			userItemTable->Upsert(newUserItemRow);
		}
	}

	return result;
}

Result ItemUtil::DeleteItem(UserDbSession& userDbSession, UserCacheAccessor& userCache, const ItemId itemId, ItemQuantity itemQuantity)
{
	Result result = Result::SUCCEEDED;

	do
	{
		if (0 == itemQuantity)
			break;

		UserItemTablePtr userItemTable = userCache.Get<UserItemTable>();
		
		if (nullptr == userItemTable)
		{
			result = Result::DB_ERROR;
			break;
		}

		UserItemRowPtr userItemRow = userItemTable->Find(itemId);

		if (nullptr == userItemRow)
		{
			result = Result::NOT_ENOUGH_ITEM;
			break;
		}

		ITEM& item = userItemRow->Data();

		if (item.Get_c_item_quantity() < itemQuantity)
		{
			result = Result::NOT_ENOUGH_ITEM;
			break;
		}

		// 롤백을 대비해 개수가 0이어도 삭제하지 않는다
		if (item.Get_c_item_quantity() >= itemQuantity)
		{
			const ItemQuantity sum = (item.Get_c_item_quantity() - itemQuantity);
			
			SP_ITEM_UPDATE_QUANTITY spItemUpdateQuantity(
				userDbSession,
				item.Get_c_item_id(),
				sum,
				tClock.GetGlobalNowTs()
			);
			result = DbUtil::ExecuteSp(spItemUpdateQuantity);
			if (Result::SUCCEEDED != result)
				break;

			userItemRow->SetItemQuantity(sum);
		}

	} while (false);

	return result;
}

// 아이템 삭제 후 정리의 개념으로 실행, 0개인 아이템들을 DB에서 삭제한다
// [정책] DB에서는 삭제하고, 캐시 테이블에서는 수량 0으로 유지 (클라이언트 동기화/표시 용도)
void ItemUtil::TrimInventory(UserDbSession& userDbSession, UserCacheAccessor& UserCacheAccessor)
{
	if (UserItemTablePtr userItemTable = UserCacheAccessor.Get<UserItemTable>())
	{
		UserItemTable::Table& itemTable = userItemTable->Data();

		for (auto& [itemId, userItemRowPtr] : itemTable)
		{
			ITEM& item = userItemRowPtr->Data();

			// 0개 이하인 아이템만 DB에서 삭제 (캐시 테이블에서는 유지)
			if (0 >= item.Get_c_item_quantity())
			{
				SP_ITEM_DELETE spItemDelete(
					userDbSession,
					UuidUtil::GenerateUuid(),
					item.Get_c_item_id(),
					tClock.GetGlobalNowTs()
				);
				DbUtil::ExecuteSp(spItemDelete);
				// [정책] DB 삭제 성공/실패와 무관하게 캐시에서는 제거하지 않음
			}
		}
	}
}
