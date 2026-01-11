// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ChattingWidgetBase.h"

#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "Util/PacketUtil/PacketUtil.h"


void UChattingWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UChattingWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UChattingWidgetBase::OnDispatchPacket(UPacketPtr* packetPtr)
{
	MAYBE_UNUSED const HandleResult handleResult = PacketUtil::DispatchPacket(this, *packetPtr->rp);
}

