// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "DebugControllerWidgetBase.h"

#include "Widget/Button/ButtonWidgetBase.h"
#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "Util/PacketUtil/PacketUtil.h"


void UDebugControllerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mReqGameListBtn->SetupButton(UUiEvent_POPUP_GAME_LIST_REQ_CLICKED::TYPE, TEXT("GameList"));
	mDebugBtn->SetupButton(UUiEvent_DEBUG_TOGGLE_CLICKED::TYPE, TEXT("Debug"));
	mCheatBtn->SetupButton(UUiEvent_CHEAT_TOGGLE_CLICKED::TYPE, TEXT("Cheat"));
}

void UDebugControllerWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDebugControllerWidgetBase::OnDispatchPacket(UPacketPtr* packetPtr)
{
	MAYBE_UNUSED const HandleResult handleResult = PacketUtil::DispatchPacket(this, *packetPtr->rp);
}
