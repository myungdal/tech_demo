// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "TlsCommonBuffer.h"


namespace TlsCommonBuffer
{
	constexpr const wchar_t* TLS_COMMON_BUFFER_TAG = L"SEND_BUFFER";

	void ResetTlsCommonBufferIdx()
	{
		tCommonBufferIdx = 0;
	}

	void ReleaseTlsCommonBuffer()
	{
		for (size_t i = 0; i < tCommonBufferCount; ++i)
		{
			_FREE(reinterpret_cast<void*>(tCommonBuffer[i]));
		}
	}


	uint8_t* GetTlsCommonBuffer(const wchar_t* functionName)
	{
		if (tCommonBufferIdx == tCommonBufferCount)
		{
			if (tCommonBufferIdx == MAX_TLS_COMMON_BUFFER_COUNT)
			{
				_ASSERT(false);
				return nullptr;
			}

			tCommonBuffer[tCommonBufferIdx] = _MALLOC<uint8_t>(MAX_TLS_COMMON_BUFFER_SIZE);
			++tCommonBufferCount;
		}

		uint8_t* ptr = tCommonBuffer[tCommonBufferIdx++];
		*reinterpret_cast<const wchar_t**>(ptr) = TLS_COMMON_BUFFER_TAG;
		*reinterpret_cast<const wchar_t**>(ptr + sizeof(wchar_t*)) = functionName;
		return ptr + MAX_TLS_COMMON_BUFFER_HEADER_SIZE;
	}
}
