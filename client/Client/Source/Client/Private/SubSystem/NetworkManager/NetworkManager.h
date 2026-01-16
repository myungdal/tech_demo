// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Tickable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "NetworkManager.generated.h"

class NetworkPacketWriter;
class NetworkPacket;
class UPacketPtr;
class UUiEventBase;
class FClientSocket;
class FClientPacketFlowBase;


DECLARE_MULTICAST_DELEGATE(NetworkEventDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(NetworkPacketDelegate, UPacketPtr*);

/**
 * 네트워크 서브시스템.
 * 연결/송수신/패킷 디스패치 및 패킷 플로우 업데이트를 담당합니다.
 */
UCLASS()
class CLIENT_API UNetworkManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	NetworkPacketDelegate mOnDispatchPacket;

private:
	TSharedPtr<FClientSocket> mClientSocket;
	TArray<TSharedPtr<FClientPacketFlowBase>> mClientPacketFlowList;

private:
	ClockMs mClockPointGlobalDelta = 0ms;
	std::atomic<bool> mFatalError = false;

public:
	template<typename T>
	static UNetworkManager* Get(T* obj)
	{
		UWorld* world = nullptr;
		if constexpr (THasGetWorld<T>::value)
			world = obj->GetWorld();
		else
			world = gCurrWorld;

		if (world)
			if (UGameInstance* gameInstance = world->GetGameInstance())
				return gameInstance->GetSubsystem<UNetworkManager>();

		return nullptr;
	}

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	void OnInitialize();
	void OnRelease();

public:
	void Connect() const;	
	void Send(NetworkPacketWriter& wp) const;
	void Disconnect() const;
	void FatalErrorAsync() { mFatalError.store(true); }
	void FatalError() const;

private:
	void OnConnected() const;
	void OnDisconnected() const;
	void OnDispatchPacket(NetworkPacket& rp) const;

	friend class UUiServiceManager;
	void OnDispatchUiEvent(UUiEventBase* uiEvent) const;

public:
	bool IsConnected() const;

public:
	ClockPoint GetGlobalNow() const;

public:
	ClockPoint GetLocalNow() const;
	void SetupGlobalNow(ClockPoint globalNow);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};
