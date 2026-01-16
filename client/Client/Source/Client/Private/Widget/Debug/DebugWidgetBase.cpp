// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "DebugWidgetBase.h"

#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Border.h"

#include "MmoSync/ComponentRegisterer/Components.h"
#include "MmoSync/GameInstance/GameInstance.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"

#include "ClientUtil/ClientStringUtil.h"
#include "ClientUtil/ClientFileUtil.h"

#include "GameCore/GameCore.h"
#include "Widget/Button/ButtonWidgetBase.h"
#include "UiService/Game/UiServiceGame.h"
#include "UiService/Debug/UiServiceDebug.h"
#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "GameRenderer/Map/MapRenderer.h"
#include "GameRenderer/Grid/RenderSystemGrid.h"
#include "GameRenderer/Character/RenderSystemCharacter.h"
#include "GameCameraController/GameCameraViewport.h"
#include "Util/GameUtil/GameComponentPrinter.h"
#include "UiObject/RowData/UiRowDataComponent.h"
#include "UiObject/RowData/UiRowDataEntity.h"


void UDebugWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// 디버그 위젯을 반투명하게 설정 (0.0 = 완전 투명, 1.0 = 완전 불투명)
	SetRenderOpacity(0.7f);

	mCloseBtn->SetupButton(UUiEvent_DEBUG_TOGGLE_CLICKED::TYPE, TEXT("X"));

	mPauseBtn->SetupButton(UUiEvent_DEBUG_PAUSE_CLICKED::TYPE, TEXT("||"));
	mPlayBtn->SetupButton(UUiEvent_DEBUG_PLAY_CLICKED::TYPE, TEXT(">"));
	mStepBtn->SetupButton(UUiEvent_DEBUG_STEP_CLICKED::TYPE, TEXT("|>"));
	mExportBtn->SetupButton(UUiEvent_DEBUG_EXPORT_CLICKED::TYPE, TEXT("Export"));

	mPauseBtn->SetIsEnabled(true);
	mPlayBtn->SetIsEnabled(false);
	mStepBtn->SetIsEnabled(false);

	// ListView를 Single Selection 모드로 설정 (한 번에 하나만 선택)
	if (mTestEntityListView)
	{
		mTestEntityListView->SetSelectionMode(ESelectionMode::Single);
	}
	if (mTestComponentListView)
	{
		mTestComponentListView->SetSelectionMode(ESelectionMode::Single);
	}

	OnReloadDebugInfoFile();

	GetWorld()->GetTimerManager().SetTimer(mTimerHandleReloadDebugInfoFile, this, &UDebugWidgetBase::OnReloadDebugInfoFile, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(mTimerHandleUpdateComponentDetail, this, &UDebugWidgetBase::OnUpdateComponentDetail, 1.0f, true);
}

void UDebugWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDebugWidgetBase::OnReloadDebugInfoFile()
{
	FClientFileUtil::ReloadDebugInfoFile();
}

void UDebugWidgetBase::OnUpdateComponentDetail()
{
	TSharedPtr<FUiServiceDebug> debugService = UUiServiceManager::Get(this)->GetService<FUiServiceDebug>();
	if (!debugService)
		return;

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

	// 선택된 엔티티와 컴포넌트 타입 가져오기
	EntityIdp entityIdp = debugService->GetSelectedEntityIdp();
	ComponentType componentType = debugService->GetSelectedComponentType();

	if (mTestComponentListView)
	{
		if (const Entity* entity = gameInstance->GetEngine().GetEntity(entityIdp))
		{
			const ComponentTypeValue& componentValue = entity->GetComponentTypeValue();

			// 현재 리스트와 실제 컴포넌트 비교
			TArray<ComponentType> currentListComponents;
			for (auto* item : mTestComponentListView->GetListItems())
			{
				if (UUiRowDataComponent* row = Cast<UUiRowDataComponent>(item))
					currentListComponents.Add(row->mComponentType);
			}

			TArray<ComponentType> actualComponents;
			for (size_t i = 0; i < _IDX(ComponentType::MAX); ++i)
			{
				if (componentValue[i])
					actualComponents.Add(static_cast<ComponentType>(i));
			}

			bool isDifferent = (currentListComponents.Num() != actualComponents.Num());
			if (!isDifferent)
			{
				for (int32 i = 0; i < currentListComponents.Num(); ++i)
				{
					if (currentListComponents[i] != actualComponents[i])
					{
						isDifferent = true;
						break;
					}
				}
			}

			if (isDifferent)
			{
				// 선택 상태 저장
				UObject* itemToSelect = nullptr;

				mTestComponentListView->ClearSelection();
				mTestComponentListView->ClearListItems();

				for (ComponentType type : actualComponents)
				{
					UUiRowDataComponent* rowData = NewObject<UUiRowDataComponent>();
					rowData->mEntityIdp = entityIdp;
					rowData->mComponentType = type;
					mTestComponentListView->AddItem(rowData);

					if (type == componentType)
						itemToSelect = rowData;
				}

				// 선택 복원
				if (itemToSelect)
				{
					mTestComponentListView->SetSelectedItem(itemToSelect);
				}
				else if (componentType != ComponentType::NONE)
				{
					// 이전에 선택된 컴포넌트가 사라졌다면 선택 해제
					debugService->SetSelectedComponentType(ComponentType::NONE);
				}
			}
		}
		else
		{
			// 엔티티가 없으면 리스트 클리어
			if (mTestComponentListView->GetNumItems() > 0)
			{
				mTestComponentListView->ClearSelection();
				mTestComponentListView->ClearListItems();
			}
		}
	}

	if (entityIdp == INVALID_ENTITY_IDP || componentType == ComponentType::NONE)
		return;

	// 엔티티가 여전히 존재하고 해당 컴포넌트를 가지고 있는지 확인
	if (const Entity* entity = gameInstance->GetEngine().GetEntity(entityIdp))
	{
		if (const ComponentBase* componentBase = gameInstance->GetEngine().TryGetComponentBase(*entity, componentType))
		{
			// formatter에서 사용할 WorldManager 설정
			tWorldManager = &gameInstance->GetWorldManager();
			
			// 컴포넌트 정보를 최신화
			FString str = ComponentsPrinter::Print(*componentBase, componentType);
			mTestComponentDetailText->SetText(FText::FromString(str));
			
			// 사용 후 초기화
			tWorldManager = nullptr;
		}
	}
}

void UDebugWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UDebugWidgetBase::OnUiEvent(UUiEvent_DEBUG_PAUSE_CLICKED& uiEvent)
{
	TSharedPtr<FUiServiceDebug> debugService = UUiServiceManager::Get(this)->GetService<FUiServiceDebug>();
	if (!debugService)
		return;

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

	mPauseBtn->SetIsEnabled(false);
	mPlayBtn->SetIsEnabled(true);
	mStepBtn->SetIsEnabled(true);

	// STOP_TIME_STEP 으로 설정하여 플레이 멈춤.
	debugService->SetTimeStepToPlay(STOP_TIME_STEP);

	// Entity 및 Component 리스트 초기화
	mTestEntityListView->ClearSelection();
	mTestEntityListView->ClearListItems();
	mTestComponentListView->ClearSelection();
	mTestComponentListView->ClearListItems();
	mTestComponentDetailText->SetText(FText::FromString(TEXT("Detail")));
	gameInstance->GetEngine().ForEachEntity(
		[this](Entity& entity)
		{
			UUiRowDataEntity* item = NewObject<UUiRowDataEntity>();
			item->mEntityIdp = entity.GetEntityIdp();
			mTestEntityListView->AddItem(item);
		}
	);
}

void UDebugWidgetBase::OnUiEvent(UUiEvent_DEBUG_PLAY_CLICKED& uiEvent)
{
	TSharedPtr<FUiServiceDebug> debugService = UUiServiceManager::Get(this)->GetService<FUiServiceDebug>();
	if (!debugService)
		return;

	mPauseBtn->SetIsEnabled(true);
	mPlayBtn->SetIsEnabled(false);
	mStepBtn->SetIsEnabled(false);

	debugService->SetTimeStepToPlay(INVALID_TIME_STEP);
}

void UDebugWidgetBase::OnUiEvent(UUiEvent_DEBUG_STEP_CLICKED& uiEvent)
{
	TSharedPtr<FUiServiceDebug> debugService = UUiServiceManager::Get(this)->GetService<FUiServiceDebug>();
	if (!debugService)
		return;

	debugService->SetTimeStepToPlay(DebugInfo::DEBUG_STEP_BNT_COUNT);

	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_DEBUG_ENTITY_SELECTED>();
}

void UDebugWidgetBase::OnUiEvent(UUiEvent_DEBUG_EXPORT_CLICKED& uiEvent)
{
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

	gameInstance->PrintDebugInfo(L"DEBUG_EXPORT_CLICKED");
}

void UDebugWidgetBase::OnUiEvent(UUiEvent_DEBUG_ENTITY_SELECTED& uiEvent)
{
	TSharedPtr<FUiServiceDebug> debugService = UUiServiceManager::Get(this)->GetService<FUiServiceDebug>();
	if (!debugService)
		return;

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

	// DEBUG_ENTITY_ID 에 설정하여 브레이크포인트 걸기 용이하도록 함
	const EntityIdp entityIdp = debugService->GetSelectedEntityIdp();	
	gDebugInfo.LoadDebugInfoVar(L"DEBUG_ENTITY_ID", entityIdp.first);

	// Entity 리스트가 비어있으면 자동으로 채우기 (게임 중 클릭으로 선택한 경우)
	const TArray<UObject*>& entityListItems = mTestEntityListView->GetListItems();
	if (entityListItems.Num() == 0)
	{
		gameInstance->GetEngine().ForEachEntity(
			[this](Entity& entity)
			{
				UUiRowDataEntity* item = NewObject<UUiRowDataEntity>();
				item->mEntityIdp = entity.GetEntityIdp();
				mTestEntityListView->AddItem(item);
			}
		);
	}

	// EntityListView에서 선택된 항목 업데이트 - 먼저 모든 선택 해제
	mTestEntityListView->ClearSelection();
	
	const TArray<UObject*>& updatedEntityListItems = mTestEntityListView->GetListItems();
	for (UObject* item : updatedEntityListItems)
	{
		if (UUiRowDataEntity* rowData = Cast<UUiRowDataEntity>(item))
		{
			if (rowData->mEntityIdp == entityIdp)
			{
				// 새로운 항목 선택
				mTestEntityListView->SetSelectedItem(item);
				// 선택된 항목으로 스크롤
				mTestEntityListView->ScrollIndexIntoView(mTestEntityListView->GetIndexForItem(item));
				break;
			}
		}
	}

	mTestComponentDetailText->SetText(FText::FromString(TEXT("Detail")));

	// 이전에 선택된 컴포넌트 타입 기억
	ComponentType previouslySelectedComponentType = debugService->GetSelectedComponentType();

	// Component 리스트 갱신 전 선택 해제
	mTestComponentListView->ClearSelection();
	mTestComponentListView->ClearListItems();
	if (const Entity* entity = gameInstance->GetEngine().GetEntity(entityIdp))
	{
		const ComponentTypeValue& componentValue = entity->GetComponentTypeValue();
		bool foundPreviousComponent = false;

		for (size_t i = 0; i < _IDX(ComponentType::MAX); ++i)
		{
			if (false == componentValue[i])
				continue;

			UUiRowDataComponent* rowData = NewObject<UUiRowDataComponent>();
			rowData->mEntityIdp = entityIdp;
			rowData->mComponentType = static_cast<ComponentType>(i);
			mTestComponentListView->AddItem(rowData);

			// 이전에 선택된 컴포넌트 타입이 현재 엔티티에 있는지 확인
			if (static_cast<ComponentType>(i) == previouslySelectedComponentType)
			{
				foundPreviousComponent = true;
			}
		}

		// 이전에 선택된 컴포넌트가 현재 엔티티에 있으면 자동으로 선택
		if (foundPreviousComponent && previouslySelectedComponentType != ComponentType::NONE)
		{
			debugService->SetSelectedComponentType(previouslySelectedComponentType);
			UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_DEBUG_COMPONENT_SELECTED>();
		}
		else
		{
			// 이전 선택이 없거나 현재 엔티티에 없으면 선택 초기화
			debugService->SetSelectedComponentType(ComponentType::NONE);
			mTestComponentListView->ClearSelection();
		}
	}
}

void UDebugWidgetBase::OnUiEvent(UUiEvent_DEBUG_COMPONENT_SELECTED& uiEvent)
{
	TSharedPtr<FUiServiceDebug> debugService = UUiServiceManager::Get(this)->GetService<FUiServiceDebug>();
	if (!debugService)
		return;

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

	ComponentType componentType = debugService->GetSelectedComponentType();

	// ComponentListView에서 선택된 항목 업데이트 - 먼저 모든 선택 해제
	mTestComponentListView->ClearSelection();
	
	const TArray<UObject*>& componentListItems = mTestComponentListView->GetListItems();
	for (UObject* item : componentListItems)
	{
		if (UUiRowDataComponent* rowData = Cast<UUiRowDataComponent>(item))
		{
			if (rowData->mComponentType == componentType)
			{
				// 새로운 항목 선택
				mTestComponentListView->SetSelectedItem(item);
				// 선택된 항목으로 스크롤
				mTestComponentListView->ScrollIndexIntoView(mTestComponentListView->GetIndexForItem(item));
				break;
			}
		}
	}

	mTestComponentDetailText->SetText(FText::FromString(TEXT("Detail")));

	EntityIdp entityIdp = debugService->GetSelectedEntityIdp();
	if (const Entity* entity = gameInstance->GetEngine().GetEntity(entityIdp))
	{
		if (const ComponentBase* componentBase = gameInstance->GetEngine().TryGetComponentBase(*entity, componentType))
		{
			// formatter에서 사용할 WorldManager 설정
			tWorldManager = &gameInstance->GetWorldManager();
			
			FString str = ComponentsPrinter::Print(*componentBase, componentType);
			mTestComponentDetailText->SetText(FText::FromString(str));
			
			// 사용 후 초기화
			tWorldManager = nullptr;
		}
	}
}
