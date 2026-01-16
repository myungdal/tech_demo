// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DevPacketConverter.h"

#include "Packet/NetworkPacket/NetworkPacket.h"


void DevPacketConverter::SetDevPacketTypeMap(DevPacketConverter::DevPacketTypeMap&& devPacketTypeMap)
{
	WriteLock lock(mDevPacketTypeMapLock);

	mDevPacketTypeMap = std::move(devPacketTypeMap);
}

void DevPacketConverter::OnApplyDevPacketType(IN OUT NetworkPacket& rp)
{
	ReadLock lock(mDevPacketTypeMapLock);

	auto it = mDevPacketTypeMap.find(rp.RefPacketType());
	if (it != mDevPacketTypeMap.end())
	{
		rp.RefPacketType() = it->second;
	}
}
