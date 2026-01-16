// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


constexpr size_t MAX_TLS_COMMON_BUFFER_HEADER_SIZE = sizeof(wchar_t*) + sizeof(wchar_t*); // 태그, 함수 이름
constexpr size_t MAX_TLS_COMMON_BUFFER_SIZE = MAX_PACKET_BUFFER_SIZE + MAX_TLS_COMMON_BUFFER_HEADER_SIZE; // 8k + 헤더사이즈
constexpr size_t MAX_TLS_COMMON_BUFFER_COUNT = 1024;
thread_local inline uint8_t* tCommonBuffer[MAX_TLS_COMMON_BUFFER_COUNT];
thread_local inline size_t tCommonBufferIdx = 0;
thread_local inline size_t tCommonBufferCount = 0;

namespace TlsCommonBuffer
{
	void ResetTlsCommonBufferIdx();
	void ReleaseTlsCommonBuffer();
	uint8_t* GetTlsCommonBuffer(const wchar_t* functionName);
}

#define TEMP_BUF TlsCommonBuffer::GetTlsCommonBuffer(__FUNCTIONW__)
