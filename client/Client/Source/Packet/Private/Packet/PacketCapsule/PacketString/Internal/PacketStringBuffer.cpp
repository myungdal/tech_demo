// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/PacketCapsule/PacketString/Internal/PacketStringBuffer.h"

#include "Common/MemPool/FixedPool.h"


namespace
{
	constexpr size_t PACKET_STRING_BUFFER_INFO_SIZE = sizeof(wchar_t**) + sizeof(wchar_t*); // 태그, 함수 이름
	constexpr const wchar_t* PACKET_STRING_BUFFER_TAG = L"PACKET_STRING_BUFFER";

	struct Buf
	{
		uint8_t mBuf[(MAX_PACKET_BUFFER_SIZE >> 1) + PACKET_STRING_BUFFER_INFO_SIZE];
	};

	using PacketStringBufferPool = FixedPool<Buf, 12>;
	inline PacketStringBufferPool& GetPacketStringBufferPool()
	{
		static PacketStringBufferPool pool{};
		return pool;
	}
}

uint8_t* PacketStringBuffer::Pop(const wchar_t* functionName)
{
	uint8_t* ptr = reinterpret_cast<uint8_t*>(GetPacketStringBufferPool().Pop());
	*reinterpret_cast<const wchar_t**>(ptr) = PACKET_STRING_BUFFER_TAG;
	*reinterpret_cast<const wchar_t**>(ptr + sizeof(wchar_t*)) = functionName;
	return ptr + PACKET_STRING_BUFFER_INFO_SIZE;
}

void PacketStringBuffer::Push(uint8_t* buf)
{
	Buf* ptr = reinterpret_cast<Buf*>(buf - PACKET_STRING_BUFFER_INFO_SIZE);
	_ASSERT_CRASH(PACKET_STRING_BUFFER_TAG == *reinterpret_cast<const wchar_t**>(ptr));
	GetPacketStringBufferPool().Push(ptr);
}
