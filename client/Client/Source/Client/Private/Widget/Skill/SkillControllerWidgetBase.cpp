// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "SkillControllerWidgetBase.h"

#include "SubSystem/NetworkContext/NetworkContext.h"
#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "Util/PacketUtil/PacketUtil.h"
#include "Widget/Button/ButtonWidgetBase.h"


void USkillControllerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 전용 이벤트 타입 사용 (WorldUserId 필요 없음)
	// OnUiEvent에서 WorldUserId를 채워서 SHORTCUT_ 이벤트로 변환
	mRollBtn->SetupButton(UUiEvent_BTN_ROLL_CLICKED::TYPE, TEXT("Roll(SPACEBAR)"));
	mChargeBtn->SetupButton(UUiEvent_BTN_CHARGE_CLICKED::TYPE, TEXT("Charge(E)"));
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

void USkillControllerWidgetBase::OnUiEvent(UUiEvent_BTN_ROLL_CLICKED& uiEvent)
{
	UNetworkContext* networkContext = UNetworkContext::Get(this);
	if (!networkContext)
		return;

	const WorldUserId worldUserId = networkContext->CurrPacketHeader().GetWorldUserId();
	if (worldUserId == INVALID_WORLD_USER_ID)
		return;

	// WorldUserId를 포함한 SHORTCUT 이벤트로 변환하여 브로드캐스트
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_SHORTCUT_ROLL_SPACEBAR>(worldUserId);
}

void USkillControllerWidgetBase::OnUiEvent(UUiEvent_BTN_CHARGE_CLICKED& uiEvent)
{
	UNetworkContext* networkContext = UNetworkContext::Get(this);
	if (!networkContext)
		return;

	const WorldUserId worldUserId = networkContext->CurrPacketHeader().GetWorldUserId();
	if (worldUserId == INVALID_WORLD_USER_ID)
		return;

	// WorldUserId를 포함한 SHORTCUT 이벤트로 변환하여 브로드캐스트
	UUiServiceManager::Get(this)->BroadcastUiEvent<UUiEvent_SHORTCUT_CHARGE_E>(worldUserId);
}
