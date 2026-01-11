// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/PacketCapsule/PacketTemp/PacketTemp.h"

#include "Common/MemPool/FixedPool.h"

#include "Packet/NetworkPacket/NetworkPacket.h"


namespace
{
	struct PacketTempBuffer
	{
		uint8_t mBuf[MAX_PACKET_BUFFER_SIZE];
	};

	using PacketTempBufferPool = FixedPool<PacketTempBuffer, 13>;
	inline PacketTempBufferPool& GetPacketTempBufferPool()
	{
		static PacketTempBufferPool pool{};
		return pool;
	}
}

PacketTemp::PacketTemp(const NetworkPacket& other)
{
	if (nullptr == mPacket)
		mPacket = reinterpret_cast<NetworkPacket*>(GetPacketTempBufferPool().Pop());

	std::copy_n(other.GetPacketBufPtr(), other.GetPacketSize(), (uint8_t*)mPacket);
}

PacketTemp::PacketTemp(const Packet& other)
{
	if (nullptr == mPacket)
		mPacket = reinterpret_cast<NetworkPacket*>(GetPacketTempBufferPool().Pop());

	std::copy_n(other.GetPacketBufPtr(), other.GetPacketSize(), (uint8_t*)mPacket);
}

PacketTemp::PacketTemp(const PacketTemp& other)
{
	mPacket = other.mPacket;
	const_cast<PacketTemp&>(other).mPacket = nullptr;
}

PacketTemp::PacketTemp(const uint8_t* packetBufPtr)
{
	if (nullptr == mPacket)
		mPacket = reinterpret_cast<NetworkPacket*>(GetPacketTempBufferPool().Pop());

	NetworkPacket* other = reinterpret_cast<NetworkPacket*>(const_cast<uint8_t*>(packetBufPtr));

	std::copy_n(other->GetPacketBufPtr(), other->GetPacketSize(), (uint8_t*)mPacket);
}

PacketTemp::~PacketTemp()
{
	if (nullptr != mPacket)
	{
		GetPacketTempBufferPool().Push(reinterpret_cast<PacketTempBuffer*>(mPacket));
		mPacket = nullptr;
	}
}

PacketTemp& PacketTemp::operator=(const NetworkPacket& other)
{
	if (nullptr == mPacket)
		mPacket = reinterpret_cast<NetworkPacket*>(GetPacketTempBufferPool().Pop());

	std::copy_n(other.GetPacketBufPtr(), other.GetPacketSize(), (uint8_t*)mPacket);

	return *this;
}

PacketTemp& PacketTemp::operator=(const PacketTemp& other)
{
	mPacket = other.mPacket;
	const_cast<PacketTemp&>(other).mPacket = nullptr;

	return *this;
}

PacketTemp& PacketTemp::operator=(const uint8_t* packetBufPtr)
{
	mPacket = reinterpret_cast<NetworkPacket*>(const_cast<uint8_t*>(packetBufPtr));

	return *this;
}
