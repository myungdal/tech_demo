// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "MenuWidgetBase.h"

#include "Components/VerticalBox.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "UiService/RedDot/UiServiceRedDot.h"
#include "Widget/Menu/MenuButtonWidgetBase.h"


void UMenuWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mExtendedMenuVerticalBox->SetVisibility(ESlateVisibility::Collapsed);

	mMenuButton->SetupMenuButton(UUiEvent_MENU_CLICKED::TYPE, TEXT("Menu"));
	mMenuButtonInventory->SetupMenuButton(UUiEvent_MENU_INVENTORY_CLICKED::TYPE, TEXT("가방"));
	mMenuButtonShop->SetupMenuButton(UUiEvent_MENU_SHOP_CLICKED::TYPE, TEXT("상점"));
	mMenuButtonQuest->SetupMenuButton(UUiEvent_MENU_QUEST_CLICKED::TYPE, TEXT("퀘스트"));
	mMenuButtonMail->SetupMenuButton(UUiEvent_MENU_MAIL_CLICKED::TYPE, TEXT("우편"));
	mMenuButtonMission->SetupMenuButton(UUiEvent_MENU_MISSION_CLICKED::TYPE, TEXT("미션"));
	mMenuButtonAchievement->SetupMenuButton(UUiEvent_MENU_ACHIEVEMENT_CLICKED::TYPE, TEXT("업적"));
	mMenuButtonRanking->SetupMenuButton(UUiEvent_MENU_RANKING_CLICKED::TYPE, TEXT("순위"));
	mMenuButtonConfig->SetupMenuButton(UUiEvent_MENU_CONFIG_CLICKED::TYPE, TEXT("설정"));
	mMenuButtonExit->SetupMenuButton(UUiEvent_MENU_EXIT_CLICKED::TYPE, TEXT("나가기"));
}

void UMenuWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMenuWidgetBase::ShowExtendedMenu()
{
	if (ESlateVisibility::Collapsed == mExtendedMenuVerticalBox->GetVisibility())
	{
		mExtendedMenuVerticalBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMenuWidgetBase::HideExtendedMenu()
{
	if (ESlateVisibility::Collapsed != mExtendedMenuVerticalBox->GetVisibility())
	{
		mExtendedMenuVerticalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMenuWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UMenuWidgetBase::OnUiEvent(UUiEvent_MENU_CLICKED& uiEvent)
{
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_INVENTORY_CLOSE_CLICKED>();

	if (ESlateVisibility::Collapsed == mExtendedMenuVerticalBox->GetVisibility())
	{
		ShowExtendedMenu();
	}
	else
	{
		HideExtendedMenu();
	}
}

void UMenuWidgetBase::OnUiEvent(UUiEvent_INVENTORY_OPEN& uiEvent)
{
	HideExtendedMenu();
}

void UMenuWidgetBase::OnUiEvent(UUiEvent_RED_DOT_UPDATED& uiEvent)
{
	auto redDot = UUiServiceManager::Get(this)->GetService<FUiServiceRedDot>();

	mMenuButtonInventory->SetRedDotVisible(redDot->HasRedDot(RedDotType::INVENTORY));
	mMenuButtonShop->SetRedDotVisible(redDot->HasRedDot(RedDotType::SHOP));
	mMenuButtonQuest->SetRedDotVisible(redDot->HasRedDot(RedDotType::QUEST));

	mMenuButtonMail->SetRedDotVisible(redDot->HasRedDot(RedDotType::MAIL));
	mMenuButtonMission->SetRedDotVisible(redDot->HasRedDot(RedDotType::MISSION));
	mMenuButtonAchievement->SetRedDotVisible(redDot->HasRedDot(RedDotType::ACHIEVEMENT));
	mMenuButtonRanking->SetRedDotVisible(redDot->HasRedDot(RedDotType::RANKING));

	mMenuButton->SetRedDotVisible(redDot->HasRedDot(RedDotType::MENU));
}
