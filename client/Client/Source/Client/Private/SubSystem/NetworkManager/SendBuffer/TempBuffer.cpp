// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "TempBuffer.h"

#include "SubSystem/NetworkManager/SendBuffer/SendBuffer.h"


FTempBuffer::FTempBuffer()
{
	mBuffer = FSendBuffer::Pop();
}

FTempBuffer::~FTempBuffer()
{
	FSendBuffer::Push(mBuffer);
}
