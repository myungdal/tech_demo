// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "UiAssetManager.generated.h"


struct FStreamableHandle;

UCLASS()
class CLIENT_API UUiAssetManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	TArray<TSharedPtr<FStreamableHandle>> mStreamableHandleList;

public:
	template<typename T>
	static UUiAssetManager* Get(T* obj)
	{
		UWorld* world = nullptr;
		if constexpr (THasGetWorld<T>::value)
			world = obj->GetWorld();
		else
			world = gCurrWorld;

		if (world)
			if (UGameInstance* gameInstance = world->GetGameInstance())
				return gameInstance->GetSubsystem<UUiAssetManager>();

		return nullptr;
	}

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	void OnInitialize();
	void OnRelease();
};
