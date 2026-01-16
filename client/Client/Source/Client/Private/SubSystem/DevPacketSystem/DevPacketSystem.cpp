// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "DevPacketSystem.h"

#include "Packet/NetworkPacket/NetworkPacket.h"


void UDevPacketSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnInitialize();
}

void UDevPacketSystem::Deinitialize()
{
	OnRelease();
	Super::Deinitialize();
}

void UDevPacketSystem::OnInitialize()
{

}

void UDevPacketSystem::OnRelease()
{
}

void UDevPacketSystem::SetDevPacketTypeMap(DevPacketTypeMap&& devPacketTypeMap)
{
	mDevPacketTypeMap = std::move(devPacketTypeMap);
}

void UDevPacketSystem::OnApplyDevPacketType(IN OUT NetworkPacket& rp)
{
	auto it = mDevPacketTypeMap.find(rp.RefPacketType());
	if (it != mDevPacketTypeMap.end())
	{
		rp.RefPacketType() = it->second;
	}
}
