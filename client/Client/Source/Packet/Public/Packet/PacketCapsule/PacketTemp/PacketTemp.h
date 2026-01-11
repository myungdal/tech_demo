// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class NetworkPacket;
class Packet;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PacketTemp - 일시적 복제 (공용 메모리 풀 사용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PACKET_API PacketTemp
{
private:
	NetworkPacket* mPacket = nullptr;

public:
	PacketTemp() = default;
	explicit PacketTemp(const NetworkPacket& other);
	explicit PacketTemp(const Packet& other);
	PacketTemp(const PacketTemp& other);
	explicit PacketTemp(const uint8_t* packetBufPtr);
	~PacketTemp();

public:
	PacketTemp& operator=(const NetworkPacket& other);
	PacketTemp& operator=(const PacketTemp& other);
	PacketTemp& operator=(const uint8_t* packetBufPtr);

public:
	NetworkPacket* operator*() noexcept { return mPacket; }
	const NetworkPacket* operator*() const noexcept { return mPacket; }

	NetworkPacket* operator->() noexcept { return mPacket; }
	const NetworkPacket* operator->() const noexcept { return mPacket; }

public:
	Packet* GetPacket() noexcept { return reinterpret_cast<Packet*>(mPacket); }
	const Packet* GetPacket() const noexcept { return reinterpret_cast<const Packet*>(mPacket); }
};
