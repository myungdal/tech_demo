// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "WalletWidgetBase.h"

#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "Util/PacketUtil/PacketUtil.h"


void UWalletWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWalletWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWalletWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void UWalletWidgetBase::OnDispatchPacket(UPacketPtr* packetPtr)
{
	MAYBE_UNUSED const HandleResult handleResult = PacketUtil::DispatchPacket(this, *packetPtr->rp);
}
