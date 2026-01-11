// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "DebugEntityListRowWidgetBase.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

#include "MmoSync/GameInstance/GameInstance.h"
#include "MmoSync/Engine/Engine.h"

#include "GameCore/GameCore.h"
#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "UiObject/RowData/UiRowDataString.h"
#include "UiService/Debug/UiServiceDebug.h"
#include "UiService/Game/UiServiceGame.h"
#include "UiObject/RowData/UiRowDataEntity.h"


void UDebugEntityListRowWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == mBorder->OnMouseButtonUpEvent.IsBoundToObject(this))
	{
		mBorder->OnMouseButtonUpEvent.BindUFunction(this, FName(TEXT("OnClicked")));
	}
}

void UDebugEntityListRowWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDebugEntityListRowWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{	
	// 위젯 재사용 시 초기 색상으로 리셋 (하얀색)
	if (mBorder)
	{
		mBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	TSharedPtr<FUiServiceGame> gameService = UUiServiceManager::Get(this)->GetService<FUiServiceGame>();
	if (!gameService)
		return;

	FGameCorePauseGuard gameCorePauseGuard(gameService);
	TSharedPtr<FGameCore> gameCore = gameCorePauseGuard.GetGameCore();
	if (!gameCore)
		return;

	TSharedPtr<GameInstance> gameInstance = gameCore->GetGameInstance();
	if (!gameInstance)
		return;

	UUiRowDataEntity* rowData = Cast<UUiRowDataEntity>(ListItemObject);
	if (!rowData)
		return;

	mEntityIdp = rowData->mEntityIdp;
	if (const Entity* entity = gameInstance->GetEngine().GetEntity(mEntityIdp))
	{
		if (true == entity->IsDestroyReserved())
		{
			mText->SetText(FText::Format(FText::FromString("[{0}]"), mEntityIdp.first));
		}
		else
		{
			mText->SetText(FText::Format(FText::FromString("{0}"), mEntityIdp.first));
		}
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UDebugEntityListRowWidgetBase::OnClicked()
{
	TSharedPtr<FUiServiceDebug> debugService = UUiServiceManager::Get(this)->GetService<FUiServiceDebug>();
	if (!debugService)
		return;

	debugService->SetSelectedEntityIdp(mEntityIdp);

	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_DEBUG_ENTITY_SELECTED>();
}

void UDebugEntityListRowWidgetBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (bIsSelected)
	{
		// 선택된 상태: 밝은 파란색 (하늘색)
		mBorder->SetBrushColor(FLinearColor(0.2f, 0.5f, 1.0f, 1.0f));
		OnClicked();
	}
	else
	{
		// 선택 해제 상태: 하얀색 (원래 색상)
		mBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
} 
