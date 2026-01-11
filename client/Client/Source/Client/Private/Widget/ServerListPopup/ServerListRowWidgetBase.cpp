// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ServerListRowWidgetBase.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

#include "SubSystem/NetworkContext/NetworkContext.h"
#include "UiObject/RowData/UiRowDataServer.h"


void UServerListRowWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == mBorder->OnMouseButtonUpEvent.IsBoundToObject(this))
	{
		mBorder->OnMouseButtonUpEvent.BindUFunction(this, FName(TEXT("OnClicked")));
	}
}

void UServerListRowWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UServerListRowWidgetBase::OnClicked()
{
	// 선택된 서버를 NetworkContext에 저장
	PacketHeader& packetHeader = UNetworkContext::Get(this)->CurrPacketHeader();
	packetHeader.SetServerId(mServerData->Get_c_server_id());
}

void UServerListRowWidgetBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (bIsSelected)
		OnClicked();
}

void UServerListRowWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UUiRowDataServer* rowData = Cast<UUiRowDataServer>(ListItemObject);

	mServerData = rowData->mData;

	mServerId->SetText(FText::Format(FText::FromString(TEXT("Server ID: {0}")), mServerData->Get_c_server_id()));
	mServerName->SetText(FText::Format(FText::FromString(TEXT("Name: {0}")), FText::FromString(mServerData->Get_c_server_name())));
	mServerState->SetText(FText::Format(FText::FromString(TEXT("State: {0}")), static_cast<int32>(mServerData->Get_c_server_state())));

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}
