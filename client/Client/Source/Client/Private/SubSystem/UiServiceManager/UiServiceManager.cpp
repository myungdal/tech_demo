// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "UiServiceManager.h"

#include "UiService/Debug/UiServiceDebug.h"
#include "UiService/Game/UiServiceGame.h"
#include "UiService/Inventory/UiServiceInventory.h"
#include "SubSystem/NetworkManager/NetworkManager.h"
#include "UiService/RedDot/UiServiceRedDot.h"


void UUiServiceManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnInitialize();
}

void UUiServiceManager::Deinitialize()
{
	OnRelease();
	Super::Deinitialize();
}

void UUiServiceManager::OnInitialize()
{
	size_t idx = 0;
	RegisterUiService<FUiServiceGame>(idx++);
	RegisterUiService<FUiServiceRedDot>(idx++);
	RegisterUiService<FUiServiceDebug>(idx++);
	RegisterUiService<FUiServiceDebug>(idx++);
}

void UUiServiceManager::OnRelease()
{
	mUiServiceList.Reset();
}

void UUiServiceManager::OnConnected()
{
	OnLoadingEnd();
	BroadcastUiEvent<UUiEvent_CONNECTED>();
}

void UUiServiceManager::OnDisconnected()
{
	OnLoadingStart();
	BroadcastUiEvent<UUiEvent_DISCONNECTED>();
}

void UUiServiceManager::OnFatalError()
{
	OnLoadingStart();
	BroadcastUiEvent<UUiEvent_FATAL_ERROR>();
}

void UUiServiceManager::OnLoadingStart()
{
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_LOADING_START>();
}

void UUiServiceManager::OnLoadingEnd()
{
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_LOADING_END>();
}

void UUiServiceManager::OnReq()
{
	OnLoadingStart();
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_REQ>();
}

void UUiServiceManager::OnAck()
{
	OnLoadingEnd();
	//UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_LOADING_END>();
}

void UUiServiceManager::OnDispatchPacket(NetworkPacket& rp)
{
	for (TSharedPtr<FUiServiceBase> UiServiceBase : mUiServiceList)
	{
		UiServiceBase->OnDispatchPacket(rp);
	}
}

void UUiServiceManager::OnDispatchUiEventToPacketFlow(UUiEventBase* uiEvent)
{
	// NetworkManager를 통해 ClientPacketFlow에 전달
	UNetworkManager* networkManager = UNetworkManager::Get(this);
	if (networkManager)
	{
		networkManager->OnDispatchUiEvent(uiEvent);
	}
}

void UUiServiceManager::BroadcastUiEventImpl(UUiEventBase* uiEvent)
{
	_DEBUG_LOG(WHITE, L"UiEventType: {}", GetUiEventTypeString(uiEvent->GetUiEventType()));

	mUiEventQueue.Enqueue(uiEvent);

	if (mBroadcastUiEventStarted)
		return;

	mBroadcastUiEventStarted = true;

	UUiEventBase* uiEventPtr;
	while (mUiEventQueue.Dequeue(OUT uiEventPtr))
	{
		// UiService에 전달
		for (TSharedPtr<FUiServiceBase> UiServiceBase : mUiServiceList)
		{
			UiServiceBase->OnDispatchUiEvent(uiEventPtr);
		}

		// Widget에 전달
		mOnDispatchUiEvent.Broadcast(uiEventPtr);

		// ClientPacketFlow에 전달
		OnDispatchUiEventToPacketFlow(uiEventPtr);
	}

	mBroadcastUiEventStarted = false;
}

void UUiServiceManager::BroadcastUiEvent(UiEventType uiEventType)
{
	UUiEventEmpty* uiEvent = NewObject<UUiEventEmpty>();
	uiEvent->SetUiEventType(uiEventType);

	BroadcastUiEventImpl(uiEvent);
}

