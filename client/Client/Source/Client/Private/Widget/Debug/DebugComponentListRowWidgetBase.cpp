// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "DebugComponentListRowWidgetBase.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

#include "MmoSync/GameInstance/GameInstance.h"
#include "MmoSync/Engine/Engine.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "UiObject/RowData/UiRowDataString.h"
#include "GameCore/GameCore.h"
#include "UiService/Debug/UiServiceDebug.h"
#include "UiService/Game/UiServiceGame.h"
#include "ClientUtil/ClientStringUtil.h"
#include "UiObject/RowData/UiRowDataComponent.h"


void UDebugComponentListRowWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == mBorder->OnMouseButtonUpEvent.IsBoundToObject(this))
	{
		mBorder->OnMouseButtonUpEvent.BindUFunction(this, FName(TEXT("OnClicked")));
	}
}

void UDebugComponentListRowWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDebugComponentListRowWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
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

	UUiRowDataComponent* rowData = Cast<UUiRowDataComponent>(ListItemObject);
	mEntityIdp = rowData->mEntityIdp;
	mComponentType = rowData->mComponentType;	
	if (const Entity* entity = gameInstance->GetEngine().GetEntity(mEntityIdp))
	{		
		const wchar_t* componentName = GetComponentTypeString(mComponentType);
		FText componentNameText = ClientStringUtil::w_to_t(componentName);
		const bool toDestroy = entity->GetComponentTypeValueToDestroy()[_IDX(mComponentType)];
		if (true == toDestroy)
		{
			mText->SetText(FText::Format(FText::FromString("[{0}]"), componentNameText));
		}
		else
		{
			mText->SetText(FText::Format(FText::FromString("{0}"), componentNameText));
		}
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UDebugComponentListRowWidgetBase::OnClicked()
{
	TSharedPtr<FUiServiceDebug> debugService = UUiServiceManager::Get(this)->GetService<FUiServiceDebug>();
	if (!debugService)
		return;

	debugService->SetSelectedEntityIdp(mEntityIdp);
	debugService->SetSelectedComponentType(mComponentType);

	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_DEBUG_COMPONENT_SELECTED>();
}

void UDebugComponentListRowWidgetBase::NativeOnItemSelectionChanged(bool bIsSelected)
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
