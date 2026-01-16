// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class NetworkPacket;

class DevPacketConverter
{
public:
	using DevPacketTypeMap = std::unordered_map<PacketType, PacketType>;

private:
	Lock mDevPacketTypeMapLock;
	DevPacketTypeMap mDevPacketTypeMap;


public:
	void SetDevPacketTypeMap(DevPacketTypeMap&& devPacketTypeMap);
	void OnApplyDevPacketType(IN OUT NetworkPacket& rp);
};

#ifdef _DEBUG
inline DevPacketConverter gDevPacketConverter;
#endif
