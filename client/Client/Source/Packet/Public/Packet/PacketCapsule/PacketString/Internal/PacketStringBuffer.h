// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


namespace PacketStringBuffer
{
	PACKET_API uint8_t* Pop(const wchar_t* functionName);
	PACKET_API void Push(uint8_t* buf);
};
