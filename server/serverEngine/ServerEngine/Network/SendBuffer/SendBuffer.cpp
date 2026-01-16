// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "SendBuffer.h"

#include "Common/MemPool/FixedPool.h"


namespace
{
	constexpr size_t UNIQUE_BUFFER_INFO_SIZE = sizeof(wchar_t**) + sizeof(wchar_t*); // 태그, 함수 이름
	constexpr const wchar_t* SEND_BUFFER_TAG = L"SEND_BUFFER";

	struct Buf
	{
		uint8_t mBuf[MAX_PACKET_BUFFER_SIZE + UNIQUE_BUFFER_INFO_SIZE];
	};

	using SendBufferPool = FixedPool<Buf, 12>;
	inline SendBufferPool& GetSendBufferPool()
	{
		static SendBufferPool pool{};
		return pool;
	}
}

namespace SendBuffer
{	
	uint8_t* Pop(const wchar_t* functionName)
	{
		uint8_t* ptr = reinterpret_cast<uint8_t*>(GetSendBufferPool().Pop());

		*reinterpret_cast<const wchar_t**>(ptr) = SEND_BUFFER_TAG;
		*reinterpret_cast<const wchar_t**>(ptr + sizeof(wchar_t*)) = functionName;

		return ptr + UNIQUE_BUFFER_INFO_SIZE;
	}
	
	void Push(uint8_t* buf)
	{
		Buf* ptr = reinterpret_cast<Buf*>(buf - UNIQUE_BUFFER_INFO_SIZE);

		_ASSERT_CRASH(SEND_BUFFER_TAG == *reinterpret_cast<const wchar_t**>(ptr));

		GetSendBufferPool().Push(ptr);
	}
};
