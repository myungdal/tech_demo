// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/BasePacket/PacketWriter.h"

#include "Packet/BasePacket/Packet.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PacketWriter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 새 패킷 생성 시 사용하는 생성자
PacketWriter::PacketWriter(PacketTraitType trait, PacketType packetType, uint8_t* buf)
	:
	mPacketPtr(reinterpret_cast<Packet*>(buf)),
	mPacketType(mPacketPtr->mPacketType),
	mPacketSize(mPacketPtr->mPacketSize),
	mPacketTrait(mPacketPtr->mPacketTrait)
{
	mPacketType = packetType;
	mPacketSize = sizeof(Packet);
	mPacketTrait = trait;
}

// 기존 패킷 버퍼를 래핑하는 생성자
PacketWriter::PacketWriter(uint8_t* buf)
	:
	mPacketPtr(reinterpret_cast<Packet*>(buf)),
	mPacketType(mPacketPtr->mPacketType),
	mPacketSize(mPacketPtr->mPacketSize),
	mPacketTrait(mPacketPtr->mPacketTrait)
{
}

// 문자열 직렬화: null terminator 포함하여 버퍼에 씀
bool PacketWriter::WriteString(const wchar_t* str, PacketSize len) const
{
	++len;

	if (MAX_PACKET_BUFFER_SIZE < (mPacketSize + sizeof(wchar_t) * len))
		return false;

#if defined(_WIN32) || defined(_WIN64)
	wcscpy_s(reinterpret_cast<wchar_t*>(reinterpret_cast<uint8_t*>(mPacketPtr) + mPacketSize), len, str);
#else
	wcscpy(reinterpret_cast<wchar_t*>(mPacketPtr + mPacketSize), str);
#endif
	mPacketSize += static_cast<PacketSize>(sizeof(wchar_t) * len);

	return true;
}

// 바이너리 데이터 직렬화
bool PacketWriter::WriteBinary(const uint8_t* bin, PacketSize size) const
{
	if ((0 == size) || (INVALID_PACKET_SIZE == size))
		return false;

	if (MAX_PACKET_BUFFER_SIZE < (mPacketSize + size))
		return false;

	std::copy_n(bin, size, reinterpret_cast<uint8_t*>(mPacketPtr) + mPacketSize);
	mPacketSize += size;

	return true;
}
