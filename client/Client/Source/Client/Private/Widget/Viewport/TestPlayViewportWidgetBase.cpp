// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "TestPlayViewportWidgetBase.h"

#include "Framework/Application/SlateApplication.h"

#include "Common/Random/RatioTable/RatioTable.h"

#include "Packet/StaticDataAccessor/StaticDataAccessor.h"
#include "Packet/StaticDataCatalog/StaticDataCatalog.h"
#include "Packet/StaticData/Game/GameDocLookup.h"
#include "Packet/StaticData/Game/GameDoc.h"
#include "Packet/StaticData/Map/MapDocLookup.h"
#include "Packet/StaticData/Map/MapDoc.h"

#include "MmoSync/ComponentRegisterer/Components.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/GameInstance/GameInstance.h"
#include "MmoSync/Map/Types/MapGenerationSettings.h"
#include "MmoSync/WorldManager/WorldManager.h"
#include "MmoSync/Util/TimeStep/TimeStepPlayer.h"
#include "MmoSync/Util/TimeStep/TimeStepGenerator.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/Map/Types/AsciiMap.h"
#include "SharedLib/WorldCreationHelper/WorldCreationHelper.h"

#include "GameCore/GameCore.h"
#include "GameCore/Internal/GameCoreTimeStepPlayer.h"
#include "GameRenderer/Map/MapRenderer.h"
#include "GameRenderer/Grid/RenderSystemGrid.h"
#include "GameRenderer/Character/RenderSystemCharacter.h"
#include "GameCameraController/GameCameraViewport.h"
#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/NetworkContext/NetworkContext.h"
#include "UiService/Game/UiServiceGame.h"
#include "UiService/Debug/UiServiceDebug.h"


void UTestPlayViewportWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mTimeStepGenerator = MakeShared<TimeStepGenerator>(TIME_STEP_INTERVAL, CLOCK_NOW);
	mTimeStepNow = 0;
}

void UTestPlayViewportWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTestPlayViewportWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	TSharedPtr<FUiServiceGame> gameService = UUiServiceManager::Get(this)->GetService<FUiServiceGame>();
	if (!gameService)
		return;

	if (!gameService->IsStarted())
		return;

	TimeStep timeStepToUpdate = mTimeStepGenerator->Generate(CLOCK_NOW);
	if (timeStepToUpdate == 0)
		return;

	const WorldUserId worldUserId = GetMyWorldUserId();

	TryAutoCameraCenter(InDeltaTime, worldUserId);

	FGameCorePauseGuard gameCorePauseGuard(gameService);
	TSharedPtr<FGameCore> gameCore = gameCorePauseGuard.GetGameCore();
	if (!gameCore)
		return;

	TSharedPtr<GameInstance> gameInstance = gameCore->GetGameInstance();
	if (!gameInstance)
		return;

	TSharedPtr<FRenderSystemCharacter> renderSystemCharacter = gameService->GetGameRenderSystemCharacter();
	if (!renderSystemCharacter)
		return;

	mTimeStepNow += timeStepToUpdate;

	gameService->Update(MyGeometry, InDeltaTime, mTimeStepNow, timeStepToUpdate);
}

int32 UTestPlayViewportWidgetBase::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
#if WITH_EDITOR
	if (!GetWorld() || !GetWorld()->HasBegunPlay())
		return LayerId;
#endif

	TSharedPtr<FUiServiceGame> gameService = UUiServiceManager::Get(this)->GetService<FUiServiceGame>();
	if (!gameService)
		return LayerId;

	FGameCorePauseGuard gameCorePauseGuard(gameService);
	TSharedPtr<FGameCore> gameCore = gameCorePauseGuard.GetGameCore();
	if (!gameCore)
		return LayerId;

	TSharedPtr<GameInstance> gameInstance = gameCore->GetGameInstance();
	if (!gameInstance)
		return LayerId;

	TSharedPtr<FMapRenderer> mapRenderer = gameService->GetMapRenderer();
	if (!mapRenderer)
		return LayerId;

	mapRenderer->Tick(AllottedGeometry.GetLayoutBoundingRect(), Args.GetCurrentTime());
	LayerId = mapRenderer->OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	gameCore->GetGameRenderSystemDataGrid()->SetBuilt(false);
	gameCore->GetGameRenderSystemDataCharacter()->SetBuilt(false);

	const FGameCameraViewport& viewport = mapRenderer->GetViewportForGrid();
	const FPaintParams paintParams(GetWorld(), viewport, Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	gameService->Render(paintParams);

	// 하단 NPC Follower UI 렌더링
	const WorldUserId worldUserId = GetMyWorldUserId();
	Engine& engine = gameInstance->GetEngine();
	LayerId = RenderFollowerUI(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled, engine, worldUserId);

	return LayerId;
}

WorldUserId UTestPlayViewportWidgetBase::GetMyWorldUserId() const
{
	return FSlateApplication::Get().GetModifierKeys().AreCapsLocked() ? WorldUserId(1) : WorldUserId(0);
}

void UTestPlayViewportWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UTestPlayViewportWidgetBase::OnUiEvent(UUiEvent_TEST_GAME_START& uiEvent)
{
	UNetworkContext* networkContext = UNetworkContext::Get(this);
	if (!networkContext)
		return;

	UNetworkManager* networkManager = UNetworkManager::Get(this);
	if (!networkManager)
		return;

	networkContext->LoadAccountJsonFile();

	networkManager->Connect();

	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_TEST_GAME_START_FAST>();
}

void UTestPlayViewportWidgetBase::OnUiEvent(UUiEvent_TEST_GAME_START_FAST& uiEvent)
{
	// 서버에 연결하여 StaticData를 다시 내려 받지 않고 바로 시작
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_TEST_GAME_STATIC_DATA_COMPLETED>();
}

void UTestPlayViewportWidgetBase::OnUiEvent(UUiEvent_TEST_GAME_STATIC_DATA_COMPLETED& uiEvent)
{
	if (UUiServiceManager::Get(this)->GetClientMode() != ClientMode::TEST)
		return;

	UUiServiceManager::Get(this)->OnLoadingEnd();

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

	TSharedPtr<FMapRenderer> mapRenderer = gameService->GetMapRenderer();
	if (!mapRenderer)
		return;

	// 원래는 DB 설정을 사용해야 한다
#if 0
	std::shared_ptr<GameDocLookup> gameDocLookup = gStaticDataAccessor->Get<GameDocLookup>();
	if (!gameDocLookup)
		return;

	const GameDoc* gameDoc = gameDocLookup->Find(GameSid(1));
	if (!gameDoc)
		return;

	const STATIC_MAP* staticMap = gameDoc->mMapDoc->mStaticMap;
	if (!staticMap)
		return;

	mapRenderer->Load(*staticMap);
#endif

	auto settings = std::make_unique<MapGenerationSettings>();

	if (static_cast<MapKind>(DebugInfo::DEFAULT_MAP_MAP_KIND) == MapKind::DUNGEON)
	{
		DungeonMapConfig dungeonConfig;
		dungeonConfig.mGridColCountFactor = DebugInfo::DEFAULT_MAP_GRID_FACTOR;
		dungeonConfig.mGridRowCountFactor = DebugInfo::DEFAULT_MAP_GRID_FACTOR;
		//std::mt19937 gen;
		//dungeonConfig.mRandomSeed = static_cast<uint64_t>(gen());
		dungeonConfig.mRandomSeed = 6;

		MapConfig mapConfig(dungeonConfig);
		settings->SetMapKind(mapConfig.mapKind);
		settings->mapConfig = mapConfig;
		settings->entitySettings.castleCount = DebugInfo::DEFAULT_MAP_CASTLE_CNT;
		settings->entitySettings.nestCount = DebugInfo::DEFAULT_MAP_NEST_CNT;
	}
	else if (static_cast<MapKind>(DebugInfo::DEFAULT_MAP_MAP_KIND) == MapKind::FIELD)
	{
		//FTempBuffer buffer;
		//STATIC_MAP::Writer wp(PARAM, *buffer,
		//	1, L"", 0,
		//	DebugInfo::DEFAULT_MAP_GRID_FACTOR, DebugInfo::DEFAULT_MAP_GRID_FACTOR,
		//	0, 40, 60, 70, 80, 120, 130,
		//	8, 9, 11, 13,
		//	8, 9, 11, 13,
		//	5, 5, 10, 7,
		//	0,
		//	L"Normal", L""
		//);
		//const STATIC_MAP* staticMap = &wp.GetPacket();
		//FieldMapConfig fieldConfig = WorldCreationHelper::MakeGridConfig(staticMap);

		FieldMapConfig fieldConfig;
		fieldConfig.mGridColCountFactor = DebugInfo::DEFAULT_MAP_GRID_FACTOR;
		fieldConfig.mGridRowCountFactor = DebugInfo::DEFAULT_MAP_GRID_FACTOR;
		fieldConfig.mRandomSeed = static_cast<uint64_t>(11);

		MapConfig mapConfig(fieldConfig);
		settings->SetMapKind(mapConfig.mapKind);
		settings->mapConfig = mapConfig;
		settings->entitySettings.castleCount = DebugInfo::DEFAULT_MAP_CASTLE_CNT;
		settings->entitySettings.nestCount = DebugInfo::DEFAULT_MAP_NEST_CNT;
	}

	std::unique_ptr<AsciiMap> asciiMap = gameInstance->CreateWorldGrid(*settings);
	gameInstance->CreateWorldEntity(*settings, *asciiMap);

	mapRenderer->Load(*asciiMap);

	gameInstance->GetWorldManager().CreateWorldUser(1, INVALID_UUID);
	gameInstance->GetWorldManager().CreateWorldUser(2, INVALID_UUID);

	gameService->GameCoreStart();
}
