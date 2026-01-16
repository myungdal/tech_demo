// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "UiEvent/UiEventType.h"

#include "UObject/StrongObjectPtr.h"
#include "Containers/Queue.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "UiServiceManager.generated.h"


class NetworkPacket;
class UUiEventBase;
class FUiAssetPreLoader;
class FUiServiceBase;

DECLARE_MULTICAST_DELEGATE_OneParam(UiEventDelegate, UUiEventBase*);

/**
 * UI 서비스 매니저.
 * UI 이벤트 브로드캐스트 및 각 UiService(화면/상태별 로직) 디스패치를 담당합니다.
 */
UCLASS()
class UUiServiceManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	friend class UUiButton;
	friend class UWidgetBase;
	friend class UNetworkManager;
	UiEventDelegate mOnDispatchUiEvent;

	TQueue<UUiEventBase*> mUiEventQueue;
	bool mBroadcastUiEventStarted = false;

private:
	TSharedPtr<FUiAssetPreLoader> mUiAssetPreLoader = nullptr;

private:
	TArray<TSharedPtr<FUiServiceBase>> mUiServiceList;

private:
	ClientMode mClientMode = ClientMode::GAME;

public:
	template<typename T>
	static UUiServiceManager* Get(T* obj)
	{
		UWorld* world = nullptr;
		if constexpr (THasGetWorld<T>::value)
			world = obj->GetWorld();
		else
			world = gCurrWorld;

		if (world)
			if (UGameInstance* gameInstance = world->GetGameInstance())
				return gameInstance->GetSubsystem<UUiServiceManager>();

		return nullptr;
	}

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	void OnInitialize();
	void OnRelease();

private:
	template <typename _Service>
	void RegisterUiService(size_t idx)
	{
		using _ServiceType = typename std::decay<_Service>::type;

		_ServiceType::mServiceIdx = idx;

		TSharedPtr<_ServiceType> ptr = MakeShared<_ServiceType>();
		mUiServiceList.Add(ptr);
	}

public:
	template <typename _Service>
	TSharedPtr<_Service> GetService() const
	{
		using _ServiceType = typename std::decay<_Service>::type;

		size_t mServiceIdx = _ServiceType::mServiceIdx;
		TSharedPtr<FUiServiceBase> ptr = mUiServiceList[_ServiceType::mServiceIdx];
		TSharedPtr<_ServiceType> castedPtr = StaticCastSharedPtr<_ServiceType>(ptr);
		return castedPtr;
	}

	friend class UTestViewportWidgetBase; // [TODO] 테스트 뷰포트 정리 후 제거
	friend class UTestPlayViewportWidgetBase;
protected:
	void OnConnected();
	void OnDisconnected();
	void OnFatalError();
	void OnLoadingStart();
	void OnLoadingEnd();
	void OnReq();
	void OnAck();

	void OnDispatchPacket(NetworkPacket& rp);
	void OnDispatchUiEventToPacketFlow(UUiEventBase* uiEvent);

private:
	void BroadcastUiEventImpl(UUiEventBase* uiEvent);

public:
	void BroadcastUiEvent(UiEventType uiEventType);

	template <typename _UiEvent, typename... _Args>
	void BroadcastUiEvent(_Args&&... args)
	{
		_UiEvent* uiEvent = NewObject<_UiEvent>();

		TSharedPtr<FUiMsg<_UiEvent>> msg = MakeShared<FUiMsg<_UiEvent>>(std::forward<_Args>(args)...);
		TSharedPtr<FUiMsgBase> msgBase = StaticCastSharedPtr<FUiMsgBase>(msg);
		uiEvent->SetMsg(msgBase);

		BroadcastUiEventImpl(uiEvent);
	}

public:
	void SetClientMode(ClientMode clientMode) { mClientMode = clientMode; }
	ClientMode GetClientMode() const { return mClientMode;  }
};

