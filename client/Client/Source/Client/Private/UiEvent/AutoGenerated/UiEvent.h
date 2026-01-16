// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Client/Private/UiEvent/Base/UiEventBase.h"
#include "Client/Private/UiEvent/UiEventType.h"

#include "CoreMinimal.h"
#include "UiEvent.generated.h"


template<typename _Owner, typename _UiEvent>
void DispatchUiEventFunc(_Owner * owner, UUiEventBase * uiEvent)
{
    _UiEvent* _uiEvent = StaticCast<_UiEvent*> (uiEvent);
    (owner->OnUiEvent)(* _uiEvent);
}


template <typename _Owner>
void DispatchUiEvent(_Owner* owner, UUiEventBase* uiEvent)
{
    static constexpr void(*DispatchUiEventFuncArray[static_cast<size_t>(UiEventType::MAX) + 1])(_Owner*, UUiEventBase*) =
    {
        &DispatchUiEventFunc<_Owner, UUiEvent_NONE>,
        &DispatchUiEventFunc<_Owner, UUiEvent_TEST_GAME_ENTER_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_TEST_GAME_LEAVE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_TEST_GAME_START>,
        &DispatchUiEventFunc<_Owner, UUiEvent_TEST_GAME_START_FAST>,
        &DispatchUiEventFunc<_Owner, UUiEvent_TEST_GAME_STATIC_DATA_COMPLETED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_TEST_GAME_FINISH>,
        &DispatchUiEventFunc<_Owner, UUiEvent_CONNECTED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DISCONNECTED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_FATAL_ERROR>,
        &DispatchUiEventFunc<_Owner, UUiEvent_REQ>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ACK>,
        &DispatchUiEventFunc<_Owner, UUiEvent_LOAD_ACCOUNT>,
        &DispatchUiEventFunc<_Owner, UUiEvent_RED_DOT_ADD>,
        &DispatchUiEventFunc<_Owner, UUiEvent_RED_DOT_REMOVE>,
        &DispatchUiEventFunc<_Owner, UUiEvent_RED_DOT_UPDATED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_LOADING_START>,
        &DispatchUiEventFunc<_Owner, UUiEvent_LOADING_END>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_LOGIN_OK_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_SERVER_LIST_OPEN>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_SERVER_LIST_CLOSE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_SERVER_LIST_OK_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_USER_NAME_OPEN>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_USER_NAME_OK_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_USER_NAME_OK_CLICKED_FINAL>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_GAME_LIST_REQ_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_GAME_LIST_OPEN>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_GAME_LIST_CLOSE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_POPUP_GAME_LIST_OK_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_GAME_ENTER>,
        &DispatchUiEventFunc<_Owner, UUiEvent_GAME_LEAVE>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_QUEST_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_INVENTORY_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_SHOP_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_MAIL_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_ACHIEVEMENT_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_MISSION_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_RANKING_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_EXIT_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MENU_CONFIG_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_INVENTORY_OPEN>,
        &DispatchUiEventFunc<_Owner, UUiEvent_INVENTORY_CLOSE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_INVENTORY_TAB_ALL_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_INVENTORY_TAB_STONE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_INVENTORY_TAB_ETC_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_INVENTORY_TAB_STATE_UPDATE>,
        &DispatchUiEventFunc<_Owner, UUiEvent_INVENTORY_ITEM_LIST_RELOAD>,
        &DispatchUiEventFunc<_Owner, UUiEvent_BTN_ROLL_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_BTN_CHARGE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_ROLL_SPACEBAR>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_CHARGE_E>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_RADAR_R>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_INVENTORY_T>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_MINIMAP_TAB>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_SKILL_1>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_SKILL_2>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_SKILL_3>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_SKILL_4>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_SKILL_5>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_OK_ENTER>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_CANCEL_ESC>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_ITEM_Z>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_ITEM_X>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_ITEM_C>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_ITEM_V>,
        &DispatchUiEventFunc<_Owner, UUiEvent_SHORTCUT_ITEM_B>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_DETAIL_LAUNCH_GAME_OPEN>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_DETAIL_REWARD_OPEN>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_DETAIL_STONE_OPEN>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_DETAIL_RELOAD>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_DETAIL_CLOSE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_DELETE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_LOCK_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_LAUNCH_GAME_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_REWARD_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_ITEM_STONE_MERGE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DEBUG_TOGGLE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DEBUG_PAUSE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DEBUG_PLAY_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DEBUG_STEP_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DEBUG_EXPORT_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DEBUG_SELECTE_ENTITY>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DEBUG_ENTITY_SELECTED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_DEBUG_COMPONENT_SELECTED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_CHEAT_TOGGLE_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_CHEAT_RUN_CLICKED>,
        &DispatchUiEventFunc<_Owner, UUiEvent_MAX>
    };

   const UiEventType uiEventType = uiEvent->GetUiEventType();
   DispatchUiEventFuncArray[static_cast<size_t>(uiEventType)](owner, uiEvent);
}


const TCHAR* GetUiEventTypeString(UiEventType uiEventType);
UiEventType GetUiEventTypeEnum(const TCHAR* uiEventTypeString);


UCLASS()
class UUiEvent_NONE : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::NONE;
   const TSharedPtr<FUiMsg<UUiEvent_NONE>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_NONE>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_TEST_GAME_ENTER_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::TEST_GAME_ENTER_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_TEST_GAME_ENTER_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_TEST_GAME_ENTER_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_TEST_GAME_LEAVE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::TEST_GAME_LEAVE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_TEST_GAME_LEAVE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_TEST_GAME_LEAVE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_TEST_GAME_START : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::TEST_GAME_START;
   const TSharedPtr<FUiMsg<UUiEvent_TEST_GAME_START>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_TEST_GAME_START>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_TEST_GAME_START_FAST : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::TEST_GAME_START_FAST;
   const TSharedPtr<FUiMsg<UUiEvent_TEST_GAME_START_FAST>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_TEST_GAME_START_FAST>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_TEST_GAME_STATIC_DATA_COMPLETED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::TEST_GAME_STATIC_DATA_COMPLETED;
   const TSharedPtr<FUiMsg<UUiEvent_TEST_GAME_STATIC_DATA_COMPLETED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_TEST_GAME_STATIC_DATA_COMPLETED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_TEST_GAME_FINISH : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::TEST_GAME_FINISH;
   const TSharedPtr<FUiMsg<UUiEvent_TEST_GAME_FINISH>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_TEST_GAME_FINISH>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_CONNECTED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::CONNECTED;
   const TSharedPtr<FUiMsg<UUiEvent_CONNECTED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_CONNECTED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DISCONNECTED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DISCONNECTED;
   const TSharedPtr<FUiMsg<UUiEvent_DISCONNECTED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DISCONNECTED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_FATAL_ERROR : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::FATAL_ERROR;
   const TSharedPtr<FUiMsg<UUiEvent_FATAL_ERROR>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_FATAL_ERROR>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_REQ : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::REQ;
   const TSharedPtr<FUiMsg<UUiEvent_REQ>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_REQ>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ACK : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ACK;
   const TSharedPtr<FUiMsg<UUiEvent_ACK>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ACK>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_LOAD_ACCOUNT : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::LOAD_ACCOUNT;
   const TSharedPtr<FUiMsg<UUiEvent_LOAD_ACCOUNT>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_LOAD_ACCOUNT>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_RED_DOT_ADD : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::RED_DOT_ADD;
   const TSharedPtr<FUiMsg<UUiEvent_RED_DOT_ADD>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_RED_DOT_ADD>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_RED_DOT_REMOVE : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::RED_DOT_REMOVE;
   const TSharedPtr<FUiMsg<UUiEvent_RED_DOT_REMOVE>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_RED_DOT_REMOVE>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_RED_DOT_UPDATED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::RED_DOT_UPDATED;
   const TSharedPtr<FUiMsg<UUiEvent_RED_DOT_UPDATED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_RED_DOT_UPDATED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_LOADING_START : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::LOADING_START;
   const TSharedPtr<FUiMsg<UUiEvent_LOADING_START>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_LOADING_START>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_LOADING_END : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::LOADING_END;
   const TSharedPtr<FUiMsg<UUiEvent_LOADING_END>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_LOADING_END>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_LOGIN_OK_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_LOGIN_OK_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_LOGIN_OK_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_LOGIN_OK_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_SERVER_LIST_OPEN : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_SERVER_LIST_OPEN;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_SERVER_LIST_OPEN>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_SERVER_LIST_OPEN>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_SERVER_LIST_CLOSE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_SERVER_LIST_CLOSE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_SERVER_LIST_CLOSE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_SERVER_LIST_CLOSE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_SERVER_LIST_OK_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_SERVER_LIST_OK_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_SERVER_LIST_OK_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_SERVER_LIST_OK_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_USER_NAME_OPEN : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_USER_NAME_OPEN;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_USER_NAME_OPEN>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_USER_NAME_OPEN>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_USER_NAME_OK_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_USER_NAME_OK_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_USER_NAME_OK_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_USER_NAME_OK_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_USER_NAME_OK_CLICKED_FINAL : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_USER_NAME_OK_CLICKED_FINAL;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_USER_NAME_OK_CLICKED_FINAL>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_USER_NAME_OK_CLICKED_FINAL>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_GAME_LIST_REQ_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_GAME_LIST_REQ_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_GAME_LIST_REQ_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_GAME_LIST_REQ_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_GAME_LIST_OPEN : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_GAME_LIST_OPEN;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_GAME_LIST_OPEN>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_GAME_LIST_OPEN>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_GAME_LIST_CLOSE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_GAME_LIST_CLOSE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_GAME_LIST_CLOSE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_GAME_LIST_CLOSE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_POPUP_GAME_LIST_OK_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::POPUP_GAME_LIST_OK_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_POPUP_GAME_LIST_OK_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_POPUP_GAME_LIST_OK_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_GAME_ENTER : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::GAME_ENTER;
   const TSharedPtr<FUiMsg<UUiEvent_GAME_ENTER>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_GAME_ENTER>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_GAME_LEAVE : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::GAME_LEAVE;
   const TSharedPtr<FUiMsg<UUiEvent_GAME_LEAVE>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_GAME_LEAVE>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_QUEST_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_QUEST_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_QUEST_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_QUEST_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_INVENTORY_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_INVENTORY_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_INVENTORY_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_INVENTORY_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_SHOP_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_SHOP_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_SHOP_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_SHOP_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_MAIL_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_MAIL_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_MAIL_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_MAIL_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_ACHIEVEMENT_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_ACHIEVEMENT_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_ACHIEVEMENT_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_ACHIEVEMENT_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_MISSION_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_MISSION_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_MISSION_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_MISSION_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_RANKING_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_RANKING_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_RANKING_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_RANKING_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_EXIT_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_EXIT_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_EXIT_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_EXIT_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MENU_CONFIG_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MENU_CONFIG_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_MENU_CONFIG_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MENU_CONFIG_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_INVENTORY_OPEN : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::INVENTORY_OPEN;
   const TSharedPtr<FUiMsg<UUiEvent_INVENTORY_OPEN>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_INVENTORY_OPEN>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_INVENTORY_CLOSE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::INVENTORY_CLOSE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_INVENTORY_CLOSE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_INVENTORY_CLOSE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_INVENTORY_TAB_ALL_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::INVENTORY_TAB_ALL_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_INVENTORY_TAB_ALL_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_INVENTORY_TAB_ALL_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_INVENTORY_TAB_STONE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::INVENTORY_TAB_STONE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_INVENTORY_TAB_STONE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_INVENTORY_TAB_STONE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_INVENTORY_TAB_ETC_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::INVENTORY_TAB_ETC_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_INVENTORY_TAB_ETC_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_INVENTORY_TAB_ETC_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_INVENTORY_TAB_STATE_UPDATE : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::INVENTORY_TAB_STATE_UPDATE;
   const TSharedPtr<FUiMsg<UUiEvent_INVENTORY_TAB_STATE_UPDATE>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_INVENTORY_TAB_STATE_UPDATE>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_INVENTORY_ITEM_LIST_RELOAD : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::INVENTORY_ITEM_LIST_RELOAD;
   const TSharedPtr<FUiMsg<UUiEvent_INVENTORY_ITEM_LIST_RELOAD>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_INVENTORY_ITEM_LIST_RELOAD>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_BTN_ROLL_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::BTN_ROLL_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_BTN_ROLL_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_BTN_ROLL_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_BTN_CHARGE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::BTN_CHARGE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_BTN_CHARGE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_BTN_CHARGE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_ROLL_SPACEBAR : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_ROLL_SPACEBAR;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ROLL_SPACEBAR>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ROLL_SPACEBAR>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_CHARGE_E : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_CHARGE_E;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_CHARGE_E>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_CHARGE_E>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_RADAR_R : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_RADAR_R;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_RADAR_R>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_RADAR_R>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_INVENTORY_T : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_INVENTORY_T;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_INVENTORY_T>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_INVENTORY_T>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_MINIMAP_TAB : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_MINIMAP_TAB;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_MINIMAP_TAB>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_MINIMAP_TAB>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_SKILL_1 : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_SKILL_1;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_1>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_1>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_SKILL_2 : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_SKILL_2;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_2>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_2>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_SKILL_3 : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_SKILL_3;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_3>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_3>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_SKILL_4 : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_SKILL_4;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_4>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_4>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_SKILL_5 : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_SKILL_5;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_5>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_SKILL_5>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_OK_ENTER : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_OK_ENTER;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_OK_ENTER>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_OK_ENTER>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_CANCEL_ESC : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_CANCEL_ESC;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_CANCEL_ESC>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_CANCEL_ESC>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_ITEM_Z : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_ITEM_Z;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_Z>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_Z>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_ITEM_X : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_ITEM_X;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_X>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_X>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_ITEM_C : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_ITEM_C;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_C>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_C>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_ITEM_V : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_ITEM_V;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_V>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_V>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_SHORTCUT_ITEM_B : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::SHORTCUT_ITEM_B;
   const TSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_B>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_SHORTCUT_ITEM_B>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_DETAIL_LAUNCH_GAME_OPEN : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_DETAIL_LAUNCH_GAME_OPEN;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_LAUNCH_GAME_OPEN>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_LAUNCH_GAME_OPEN>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_DETAIL_REWARD_OPEN : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_DETAIL_REWARD_OPEN;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_REWARD_OPEN>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_REWARD_OPEN>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_DETAIL_STONE_OPEN : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_DETAIL_STONE_OPEN;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_STONE_OPEN>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_STONE_OPEN>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_DETAIL_RELOAD : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_DETAIL_RELOAD;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_RELOAD>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_RELOAD>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_DETAIL_CLOSE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_DETAIL_CLOSE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_CLOSE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_DETAIL_CLOSE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_DELETE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_DELETE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_DELETE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_DELETE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_LOCK_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_LOCK_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_LOCK_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_LOCK_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_LAUNCH_GAME_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_LAUNCH_GAME_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_LAUNCH_GAME_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_LAUNCH_GAME_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_REWARD_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_REWARD_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_REWARD_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_REWARD_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_ITEM_STONE_MERGE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::ITEM_STONE_MERGE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_ITEM_STONE_MERGE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_ITEM_STONE_MERGE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DEBUG_TOGGLE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DEBUG_TOGGLE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_DEBUG_TOGGLE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DEBUG_TOGGLE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DEBUG_PAUSE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DEBUG_PAUSE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_DEBUG_PAUSE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DEBUG_PAUSE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DEBUG_PLAY_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DEBUG_PLAY_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_DEBUG_PLAY_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DEBUG_PLAY_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DEBUG_STEP_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DEBUG_STEP_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_DEBUG_STEP_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DEBUG_STEP_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DEBUG_EXPORT_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DEBUG_EXPORT_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_DEBUG_EXPORT_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DEBUG_EXPORT_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DEBUG_SELECTE_ENTITY : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DEBUG_SELECTE_ENTITY;
   const TSharedPtr<FUiMsg<UUiEvent_DEBUG_SELECTE_ENTITY>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DEBUG_SELECTE_ENTITY>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DEBUG_ENTITY_SELECTED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DEBUG_ENTITY_SELECTED;
   const TSharedPtr<FUiMsg<UUiEvent_DEBUG_ENTITY_SELECTED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DEBUG_ENTITY_SELECTED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_DEBUG_COMPONENT_SELECTED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::DEBUG_COMPONENT_SELECTED;
   const TSharedPtr<FUiMsg<UUiEvent_DEBUG_COMPONENT_SELECTED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_DEBUG_COMPONENT_SELECTED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_CHEAT_TOGGLE_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::CHEAT_TOGGLE_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_CHEAT_TOGGLE_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_CHEAT_TOGGLE_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_CHEAT_RUN_CLICKED : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::CHEAT_RUN_CLICKED;
   const TSharedPtr<FUiMsg<UUiEvent_CHEAT_RUN_CLICKED>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_CHEAT_RUN_CLICKED>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


UCLASS()
class UUiEvent_MAX : public UUiEventBase 
{ 
    GENERATED_BODY()

public:
   static constexpr UiEventType TYPE = UiEventType::MAX;
   const TSharedPtr<FUiMsg<UUiEvent_MAX>>GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEvent_MAX>>(mMsg); }
   virtual UiEventType GetUiEventType() const { return TYPE; }
};


