// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "WidgetBase.h"

#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/UiServiceManager/UiServiceManager.h"


void UWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	mOnDispatchUiEventHandle = UUiServiceManager::Get(this)->mOnDispatchUiEvent.AddUFunction(this, FName(TEXT("_OnDispatchUiEvent")));
	mOnDispatchPacketHandle = UNetworkManager::Get(this)->mOnDispatchPacket.AddUFunction(this, FName(TEXT("_OnDispatchPacket")));
}

void UWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	UUiServiceManager::Get(this)->mOnDispatchUiEvent.Remove(mOnDispatchUiEventHandle);	
	UNetworkManager::Get(this)->mOnDispatchPacket.Remove(mOnDispatchPacketHandle);
}

void UWidgetBase::_OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	OnDispatchUiEvent(uiEvent);
}

void UWidgetBase::_OnDispatchPacket(UPacketPtr* packetPtr)
{
	OnDispatchPacket(packetPtr);
}

