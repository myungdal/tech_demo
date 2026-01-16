// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "SubSystem/UiServiceManager/UiServiceBase/UiServiceBase.h"


class UserItemRow;

struct FUiServiceInventoryItemDisplayOpt
{
	bool mRedDot = false;
	bool mDisabled = false;
};

class FUiServiceInventory : public FUiServiceBase
{
private:
	friend class UUiServiceManager;
	static inline size_t mServiceIdx = 0;

public:
	using InventoryItem = std::pair<const UserItemRowPtr, FUiServiceInventoryItemDisplayOpt>;

private:
	TArray<InventoryItem> mInventoryItemList;

private:
	TSet<int64_t> mRedDotItemSet;

private:
	UiEventType mCurrentItemDetailOpenEventType = UiEventType::NONE;
	UiEventType mCurrentTabBtnEventType = UiEventType::INVENTORY_TAB_ALL_CLICKED;

private:
	const UserItemRow* mSelectedItem0 = nullptr;
	const UserItemRow* mSelectedItem1 = nullptr;

private:
	bool IsIncludedCurrentTab(ItemType itemType) const;

private:
	void OpenItemDetail();
	void UpdateRedDotItemSet(const class USER_CACHE_DIFF& rp);
	void UpdateItemInfoList();

public:
	const TArray<InventoryItem>& GetInventoryItemList() const { return mInventoryItemList; }

public:
	UiEventType GetCurrentTabBtnEventType() const { return mCurrentTabBtnEventType; }
	void SelectItem(const UserItemRow* userItemRow);
	void DeselectItem(const UserItemRow* userItemRow);
	const UserItemRow* GetSelectedItem0() const { return mSelectedItem0; }
	const UserItemRow* GetSelectedItem1() const { return mSelectedItem1; }
	bool IsSelectedItem0(const UserItemRow* userItemRow) const { return (userItemRow == mSelectedItem0) ? true : false; }
	bool IsSelectedItem1(const UserItemRow* userItemRow) const { return (userItemRow == mSelectedItem1) ? true : false; }

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_RED_DOT_UPDATED& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_OPEN& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_CLOSE_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_TAB_ALL_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_TAB_STONE_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_TAB_ETC_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_ITEM_DETAIL_CLOSE_CLICKED& uiEvent);
	//void OnUiEvent(UUiEvent_ITEM_LAUNCH_GAME_CLICKED& uiEvent);

public:
	void OnDispatchPacket(NetworkPacket& rp) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
	HandleResult OnPacket(class DC_ACK_USER_DATA& rp);
	HandleResult OnPacket(class DC_ACK_CHEAT& rp);
	//HandleResult OnPacket(class FC_ACK_GAME_LAUNCH& rp);
};
