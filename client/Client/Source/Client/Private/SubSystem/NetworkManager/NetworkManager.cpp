// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "NetworkManager.h"

#include "SubSystem/NetworkManager/Socket/ClientSocket.h"
#include "SubSystem/NetworkManager/PacketPtr/PacketPtr.h"
#include "SubSystem/NetworkContext/NetworkContext.h"
#include "ClientPacketFlow/Common/ClientPacketFlowCommon.h"
#include "ClientPacketFlow/Game/ClientPacketFlowGame.h"
#include "ClientPacketFlow/Login/ClientPacketFlowLogin.h"
#include "Util/PacketUtil/PacketUtil.h"
#include "SubSystem/UiServiceManager/UiServiceManager.h"


void UNetworkManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnInitialize();
}

void UNetworkManager::Deinitialize()
{
	OnRelease();
	Super::Deinitialize();
}

void UNetworkManager::OnInitialize()
{
	if (nullptr != mClientSocket)
		return;

	mClientSocket = MakeShared<FClientSocket>();
	mClientPacketFlowList.Add(MakeShared<FClientPacketFlowCommon>());
	mClientPacketFlowList.Add(MakeShared<FClientPacketFlowGame>());
	mClientPacketFlowList.Add(MakeShared<FClientPacketFlowLogin>());

	mClientSocket->mOnConnected.BindUObject(this, &UNetworkManager::OnConnected);
	mClientSocket->mOnDisconnected.BindUObject(this, &UNetworkManager::OnDisconnected);
	mClientSocket->mOnDispatchPacket.BindUObject(this, &UNetworkManager::OnDispatchPacket);
}

void UNetworkManager::OnRelease()
{
}

void UNetworkManager::Connect() const
{
	UUiServiceManager::Get(this)->OnLoadingStart();

	if (mClientSocket->IsConnectedSession())
	{
		OnConnected();
	}
	else
	{
		mClientSocket->ConnectSession(L"127.0.0.1", 8001);
	}
}

void UNetworkManager::Send(NetworkPacketWriter& wp) const
{
	wp.SetHeader(UNetworkContext::Get(this)->CurrPacketHeader());

	if (true == mClientSocket->SendNetworkPacket(wp))
	{
		const bool isReqPacket = PacketUtil::IsReqPacket(*wp.GetNetworkPacketPtr());
		if (isReqPacket)
			UUiServiceManager::Get(this)->OnReq();
	}
	else
	{
		UUiServiceManager::Get(this)->OnDisconnected();
	}
}

void UNetworkManager::Disconnect() const
{
	UUiServiceManager::Get(this)->OnLoadingStart();

	mClientSocket->DisconnectSession();
}

void UNetworkManager::FatalError() const
{
	UNetworkContext::Get(this)->OnFatalError();

	for (TSharedPtr<FClientPacketFlowBase> ClientPacketFlow : mClientPacketFlowList)
	{
		ClientPacketFlow->OnFatalError();
	}

	UUiServiceManager::Get(this)->OnFatalError();
}

void UNetworkManager::OnConnected() const
{
	for (TSharedPtr<FClientPacketFlowBase> ClientPacketFlow : mClientPacketFlowList)
	{
		ClientPacketFlow->OnConnected();
	}

	UUiServiceManager::Get(this)->OnConnected();
}

void UNetworkManager::OnDisconnected() const
{
	UNetworkContext::Get(this)->OnDisconnected();

	for (TSharedPtr<FClientPacketFlowBase> ClientPacketFlow : mClientPacketFlowList)
	{
		ClientPacketFlow->OnDisconnected();
	}

	UUiServiceManager::Get(this)->OnDisconnected();
}

void UNetworkManager::OnDispatchPacket(NetworkPacket& rp) const
{
	if (false == PacketUtil::ValidatePacket(rp))
	{
		_DEBUG_LOG(RED, L"ValidatePacket Error, {}[{}]", GetPacketTypeString(rp.GetPacketType()), rp.GetPacketSize());
		Disconnect();
		return;
	}

	if (true == PacketUtil::IsAckPacket(rp))
	{
		UNetworkContext::Get(this)->CurrPacketHeader() = rp.GetHeader();

		UUiServiceManager::Get(this)->OnAck();
	}

	for (TSharedPtr<FClientPacketFlowBase> ClientPacketFlow : mClientPacketFlowList)
	{
		ClientPacketFlow->OnDispatchPacket(rp);
	}

	UUiServiceManager::Get(this)->OnDispatchPacket(rp);

	UPacketPtr* packetPtr = NewObject<UPacketPtr>();
	packetPtr->rp = &rp;
	mOnDispatchPacket.Broadcast(packetPtr);
}

void UNetworkManager::OnDispatchUiEvent(UUiEventBase* uiEvent) const
{
	// ClientPacketFlow에 UiEvent 전달
	for (TSharedPtr<FClientPacketFlowBase> ClientPacketFlow : mClientPacketFlowList)
	{
		ClientPacketFlow->OnDispatchUiEvent(uiEvent);
	}
}

bool UNetworkManager::IsConnected() const
{
	return mClientSocket->IsConnectedSession();
}

ClockPoint UNetworkManager::GetGlobalNow() const
{
	return CLOCK_NOW + mClockPointGlobalDelta;
}

ClockPoint UNetworkManager::GetLocalNow() const
{
	const ClockPoint now = CLOCK_NOW;
	return now;
}

void UNetworkManager::SetupGlobalNow(ClockPoint globalNow)
{
	mClockPointGlobalDelta = CLOCK_MS_FROM_DR(globalNow - CLOCK_NOW);
}

void UNetworkManager::Tick(float DeltaTime)
{
	if (mFatalError.load())
	{
		mFatalError.store(false);
		FatalError();
		return;
	}

	for (TSharedPtr<FClientPacketFlowBase> ClientPacketFlow : mClientPacketFlowList)
	{
		ClientPacketFlow->OnUpdate();
	}

	if (nullptr != mClientSocket)
	{
		mClientSocket->Update();
	}
}

TStatId UNetworkManager::GetStatId() const
{
	return UObject::GetStatID();
}
