// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#include "UiAssetManager.h"

#include "Engine/AssetManager.h"


void UUiAssetManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnInitialize();
}

void UUiAssetManager::Deinitialize()
{
	OnRelease();
	Super::Deinitialize();
}

void UUiAssetManager::OnInitialize()
{
	TArray<FSoftObjectPath> assetPathList;
	assetPathList.AddUnique(FSoftObjectPath(TEXT("/Game/Sprite/Ui/ItemIcon/ItemIcon_0.ItemIcon_0")));

	FStreamableManager& streamableManager = UAssetManager::GetStreamableManager();
	TSharedPtr<FStreamableHandle> streamableHandle = streamableManager.RequestAsyncLoad(assetPathList);
	mStreamableHandleList.Add(streamableHandle);
}

void UUiAssetManager::OnRelease()
{
}
