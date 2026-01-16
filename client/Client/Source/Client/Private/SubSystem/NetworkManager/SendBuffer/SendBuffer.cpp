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
	struct Buf
	{
		uint8_t mBuf[MAX_PACKET_BUFFER_SIZE];
	};

	using SendBufferPool = FixedPool<Buf, 12>;
	static SendBufferPool gSendBufferPool;
};

uint8_t* FSendBuffer::Pop()
{
	return reinterpret_cast<uint8_t*>(gSendBufferPool.Pop());
}

void FSendBuffer::Push(uint8_t* buf)
{
	gSendBufferPool.Push(reinterpret_cast<Buf*>(buf));
}
