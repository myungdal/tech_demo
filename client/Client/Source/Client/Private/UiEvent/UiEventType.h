// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "CoreMinimal.h"

#include "UiEvent/Base/UiMsgBase.h"
#include "UiEventType.generated.h"


template<>
struct FUiMsg<class UUiEvent_RED_DOT_ADD> : FUiMsgBase
{
	RedDotType mRedDotType = RedDotType::NONE;

	FUiMsg(RedDotType redDotType)
		:
		mRedDotType(redDotType)
	{
	}
};

template<>
struct FUiMsg<class UUiEvent_RED_DOT_REMOVE> : FUiMsgBase
{
	RedDotType mRedDotType = RedDotType::NONE;

	FUiMsg(RedDotType redDotType)
		:
		mRedDotType(redDotType)
	{
	}
};

template<>
struct FUiMsg<class UUiEvent_POPUP_SERVER_LIST_OPEN> : FUiMsgBase
{
	bool mUserCreateMode = true;

	FUiMsg(bool userCreateMode)
		:
		mUserCreateMode(userCreateMode)
	{
	}
};

// ========================================================================
// Shortcut 이벤트용 공통 메시지 (worldUserId 포함)
// ========================================================================
struct FUiMsgShortcut : FUiMsgBase
{
	WorldUserId mWorldUserId = 0;

	FUiMsgShortcut(WorldUserId worldUserId)
		: mWorldUserId(worldUserId)
	{
	}
};

template<> struct FUiMsg<class UUiEvent_SHORTCUT_ROLL_SPACEBAR> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_CHARGE_E> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_RADAR_R> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_INVENTORY_T> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_MINIMAP_TAB> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_SKILL_1> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_SKILL_2> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_SKILL_3> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_SKILL_4> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_SKILL_5> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_OK_ENTER> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_CANCEL_ESC> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_ITEM_Z> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_ITEM_X> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_ITEM_C> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_ITEM_V> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };
template<> struct FUiMsg<class UUiEvent_SHORTCUT_ITEM_B> : FUiMsgShortcut { using FUiMsgShortcut::FUiMsgShortcut; };


UENUM()
enum class UiEventType : uint8
{
	NONE = 0,
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 테스트
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	TEST_GAME_ENTER_CLICKED,
	TEST_GAME_LEAVE_CLICKED,
	TEST_GAME_START,
	TEST_GAME_START_FAST,
	TEST_GAME_STATIC_DATA_COMPLETED,
	TEST_GAME_FINISH,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 시스템
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CONNECTED,
	DISCONNECTED,	
	FATAL_ERROR,
	REQ,
	ACK,
	LOAD_ACCOUNT,
	RED_DOT_ADD,
	RED_DOT_REMOVE,
	RED_DOT_UPDATED,
	LOADING_START,
	LOADING_END,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 로그인 팝업
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	POPUP_LOGIN_OK_CLICKED,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 서버 목록 팝업
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	POPUP_SERVER_LIST_OPEN,
	POPUP_SERVER_LIST_CLOSE_CLICKED,
	POPUP_SERVER_LIST_OK_CLICKED,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 유저 이름 팝업
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	POPUP_USER_NAME_OPEN,
	POPUP_USER_NAME_OK_CLICKED,
	POPUP_USER_NAME_OK_CLICKED_FINAL,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 게임 목록 팝업
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	POPUP_GAME_LIST_REQ_CLICKED,
	POPUP_GAME_LIST_OPEN,
	POPUP_GAME_LIST_CLOSE_CLICKED,
	POPUP_GAME_LIST_OK_CLICKED,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 게임 입장/퇴장
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	GAME_ENTER,
	GAME_LEAVE,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 메뉴
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MENU_CLICKED,
	MENU_QUEST_CLICKED,
	MENU_INVENTORY_CLICKED,
	MENU_SHOP_CLICKED,
	MENU_MAIL_CLICKED,
	MENU_ACHIEVEMENT_CLICKED,
	MENU_MISSION_CLICKED,
	MENU_RANKING_CLICKED,
	MENU_EXIT_CLICKED,
	MENU_CONFIG_CLICKED,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 인벤토리
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	INVENTORY_OPEN,
	INVENTORY_CLOSE_CLICKED,
	INVENTORY_TAB_ALL_CLICKED,
	INVENTORY_TAB_STONE_CLICKED,
	INVENTORY_TAB_ETC_CLICKED,
	INVENTORY_TAB_STATE_UPDATE,
	INVENTORY_ITEM_LIST_RELOAD,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 스킬 버튼 클릭 (SkillControllerWidget에서 WorldUserId 채워서 SHORTCUT_ 이벤트로 변환)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BTN_ROLL_CLICKED,
	BTN_CHARGE_CLICKED,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 단축키
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SHORTCUT_ROLL_SPACEBAR,
	SHORTCUT_CHARGE_E,

	SHORTCUT_RADAR_R,
	SHORTCUT_INVENTORY_T,
	SHORTCUT_MINIMAP_TAB,

	SHORTCUT_SKILL_1,
	SHORTCUT_SKILL_2,
	SHORTCUT_SKILL_3,
	SHORTCUT_SKILL_4,
	SHORTCUT_SKILL_5,

	SHORTCUT_OK_ENTER,
	SHORTCUT_CANCEL_ESC,

	SHORTCUT_ITEM_Z,
	SHORTCUT_ITEM_X,
	SHORTCUT_ITEM_C,
	SHORTCUT_ITEM_V,
	SHORTCUT_ITEM_B,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 아이템 창
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ITEM_DETAIL_LAUNCH_GAME_OPEN,
	ITEM_DETAIL_REWARD_OPEN,
	ITEM_DETAIL_STONE_OPEN,
	ITEM_DETAIL_RELOAD,
	ITEM_DETAIL_CLOSE_CLICKED,
	ITEM_DELETE_CLICKED,
	ITEM_LOCK_CLICKED,
	ITEM_LAUNCH_GAME_CLICKED,
	ITEM_REWARD_CLICKED,
	ITEM_STONE_MERGE_CLICKED,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 디버깅
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DEBUG_TOGGLE_CLICKED,
	DEBUG_PAUSE_CLICKED,
	DEBUG_PLAY_CLICKED,
	DEBUG_STEP_CLICKED,
	DEBUG_EXPORT_CLICKED,
	DEBUG_SELECTE_ENTITY,
	DEBUG_ENTITY_SELECTED,
	DEBUG_COMPONENT_SELECTED,

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 치트
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CHEAT_TOGGLE_CLICKED,
	CHEAT_RUN_CLICKED,
	MAX
};
