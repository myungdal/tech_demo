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
#include "DevPacketSystem.generated.h"


using DevPacketTypeMap = std::unordered_map<PacketType, PacketType>;

class NetworkPacket;

/**
 * 개발/디버그용 패킷 변환 시스템.
 * (패킷 타입 치환 등) 개발 편의를 위한 동작을 적용합니다.
 */
UCLASS()
class UDevPacketSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	DevPacketTypeMap mDevPacketTypeMap;

public:
	template<typename T>
	static UDevPacketSystem* Get(T* obj)
	{
		UWorld* world = nullptr;
		if constexpr (THasGetWorld<T>::value)
			world = obj->GetWorld();
		else
			world = gCurrWorld;

		if (world)
			if (UGameInstance* gameInstance = world->GetGameInstance())
				return gameInstance->GetSubsystem<UDevPacketSystem>();

		return nullptr;
	}

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	void OnInitialize();
	void OnRelease();

public:
	void SetDevPacketTypeMap(DevPacketTypeMap&& devPacketTypeMap);
	void OnApplyDevPacketType(IN OUT NetworkPacket& rp);
};
