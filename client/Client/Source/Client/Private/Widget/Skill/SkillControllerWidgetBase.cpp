// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "SkillControllerWidgetBase.h"

#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "Util/PacketUtil/PacketUtil.h"
#include "Widget/Button/ButtonWidgetBase.h"


void USkillControllerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	mRollBtn->SetupButton(UUiEvent_SHORTCUT_ROLL_SPACEBAR::TYPE, TEXT("Roll(SPACEBAR)"));
	mChargeBtn->SetupButton(UUiEvent_SHORTCUT_CHARGE_E::TYPE, TEXT("Charge(E)"));
}

void USkillControllerWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkillControllerWidgetBase::OnDispatchUiEvent(UUiEventBase* uiEvent)
{
	DispatchUiEvent(this, uiEvent);
}

void USkillControllerWidgetBase::OnDispatchPacket(UPacketPtr* packetPtr)
{
	MAYBE_UNUSED const HandleResult handleResult = PacketUtil::DispatchPacket(this, *packetPtr->rp);
}
