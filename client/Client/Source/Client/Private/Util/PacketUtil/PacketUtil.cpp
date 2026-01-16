// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "PacketUtil.h"

#include "Packet/StaticDataAccessor/StaticDataAccessor.h"
#include "Packet/StaticData/Item/ItemDocLookup.h"

#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/NetworkContext/NetworkContext.h"


bool PacketUtil::ValidatePacket(const NetworkPacket& rp)
{
	if ((false == ValidatePacketFC(rp)) ||
		(false == ValidatePacketDC(rp)) ||
		(false == ValidatePacketGC(rp)) ||
		(false == ValidatePacketBC(rp)))
	{
		_DEBUG_RED;
		_DEBUG_BREAK;
		return false;
	}

	return true;
}

bool PacketUtil::IsReqPacket(const NetworkPacket& rp)
{
	return (rp.GetPacketTrait() == REQ) ? true : false;
}

bool PacketUtil::IsAckPacket(const NetworkPacket& rp)
{
	return (rp.GetPacketTrait() == ACK) ? true : false;
}

void PacketUtil::CacheUserData(const USER_DATA& rp)
{
	//UNetworkContext::Get(this)->GetUserCacheAccessor().SetAchievement(ACHIEVEMENT_W((uint8_t*)&rp.GetAchievement()));
	//UNetworkContext::Get(this)->GetUserCacheAccessor().SetCurrency(CURRENCY_W((uint8_t*)&rp.GetCurrency()));
	//UNetworkContext::Get(this)->GetUserCacheAccessor().SetMission(MISSION_W((uint8_t*)&rp.GetMission()));
	//UNetworkContext::Get(this)->GetUserCacheAccessor().SetStock(STOCK_W((uint8_t*)&rp.GetStock()));
	//UNetworkContext::Get(this)->GetUserCacheAccessor().SetUser(USER_W((uint8_t*)&rp.GetUser()));
	//// 레벨업이 필요한 경우, 레벨업 요청을 클라이언트가 해줘야 한다

	//UNetworkContext::Get(this)->GetUserCacheAccessor().ClearBuildingMap();
	//for (const BUILDING* building : rp.GetBuildingArray())
	//{
	//	UNetworkContext::Get(this)->GetUserCacheAccessor().InsertBuilding(building->GetBuilding_sid(), BUILDING_W((uint8_t*)building));
	//}

	//UNetworkContext::Get(this)->GetUserCacheAccessor().ClearCharacterMap();
	//for (const CHARACTER* character : rp.GetCharacterArray())
	//{
	//	UNetworkContext::Get(this)->GetUserCacheAccessor().InsertCharacter(character->ConstRefCharacter_sid(), CHARACTER_W((uint8_t*)character));
	//}

	//UNetworkContext::Get(this)->GetUserCacheAccessor().ClearItemMap();
	//for (const ITEM* item : rp.GetItemArray())
	//{
	//	ItemDocLookupPtr ItemDocLookup = gStaticDataAccessor->GetItemDocLookup();

	//	PacketKeep<ITEM>&& itemContainer = PacketKeep<ITEM>(item->GetPacketBufPtr());
	//	const ItemDocument* itemDocument = ItemDocLookup->Find(itemContainer->GetItem_sid());
	//	// itemDocument 가 없으면 치명적 에러

	//	UNetworkContext::Get(this)->GetUserCacheAccessor().InsertItem(item->GetItem_id(), ITEM_W((uint8_t*)item), itemDocument);
	//}

	//UNetworkContext::Get(this)->GetUserCacheAccessor().ClearQuestMap();
	//for (const QUEST* quest : rp.GetQuestArray())
	//{
	//	UNetworkContext::Get(this)->GetUserCacheAccessor().InsertQuest(quest->GetQuest_sid(), QUEST_W((uint8_t*)quest));
	//}
}

void PacketUtil::CacheUserAssetDelta(const USER_DATA& rp)
{
	//UserExp userExp = UNetworkContext::Get(this)->GetUserCacheAccessor().GetUserExp();
	//UserExp userExpDelta = rp.GetUserExp();
	//UNetworkContext::Get(this)->GetUserCacheAccessor().SetUserExp(userExp + userExpDelta);
	//// 레벨업이 필요한 경우, 레벨업 요청을 클라이언트가 해줘야 한다

	//for (size_t i = 0; i < rp.GetCurrencyTypeListSize(); ++i)
	//{
	//	const CurrencyType& currencyType = rp.GetCurrencyTypeList()[i];
	//	const CurrencyAmount& currencyAmountDelta = rp.GetCurrencyAmountList()[i];
	//	CurrencyAmount currencyAmount = UNetworkContext::Get(this)->GetUserCacheAccessor().GetCurrencyAmount(currencyType);
	//	UNetworkContext::Get(this)->GetUserCacheAccessor().SetCurrency(currencyType, currencyAmount + currencyAmountDelta);
	//}

	//for (size_t i = 0; i < rp.GetItemIdListSize(); ++i)
	//{
	//	const ItemId& itemId = rp.GetItemIdList()[i];
	//	UserItemRow* userItemRow = UNetworkContext::Get(this)->GetUserCacheAccessor().FindItem(itemId);
	//	if (nullptr == userItemRow)
	//	{
	//		// itemDocument 가 없으면 치명적 에러
	//		continue;
	//	}

	//	const ItemQuantity& itemQuantity = rp.GetItemQuantityList()[i];
	//	userCacheItem->AddItem_quantity(itemQuantity);
	//}

	//for (const ITEM* item : rp.GetItemArray())
	//{
	//	ItemDocLookupPtr ItemDocLookup = gStaticDataAccessor->GetItemDocLookup();

	//	PacketKeep<ITEM>&& itemContainer = PacketKeep<ITEM>(item->GetPacketBufPtr());
	//	const ItemDocument* itemDocument = ItemDocLookup->Find(itemContainer->GetItem_sid());
	//	// itemDocument 가 없으면 치명적 에러

	//	UNetworkContext::Get(this)->GetUserCacheAccessor().InsertItem(item->GetItem_id(), ITEM_W((uint8_t*)item), itemDocument);
	//}
}
