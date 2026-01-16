// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "MenuWidgetBase.generated.h"


class UVerticalBox;
class UMenuButtonWidgetBase;

/**
 * 게임 내 메뉴 UI 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UMenuWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonShop = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonInventory = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonQuest = nullptr;

public:
	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonMail = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonMission = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonAchievement = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonRanking = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonConfig = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButtonExit = nullptr;
	
public:
	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidgetBase* mMenuButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* mExtendedMenuVerticalBox = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

private:
	void ShowExtendedMenu();
	void HideExtendedMenu();
public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}

	void OnUiEvent(UUiEvent_MENU_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_INVENTORY_OPEN& uiEvent);

	void OnUiEvent(UUiEvent_RED_DOT_UPDATED& uiEvent);
};
