// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace SendBuffer
{
	uint8_t* Pop(const wchar_t* functionName);
	void Push(uint8_t* buf);
};

//struct SendBufferNode : SLIST_ENTRY
//{
//	uint8_t* mBuffer = nullptr;
//	PacketSize mSize = 0;
//
//	static SendBufferNode* Acquire();
//	void Dispose();
//
//	SendBufferNode* GetNext() const noexcept { return static_cast<SendBufferNode*>(SLIST_ENTRY::Next); }
//};
//
