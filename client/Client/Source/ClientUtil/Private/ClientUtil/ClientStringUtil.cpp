// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "ClientUtil/ClientStringUtil.h"


Buf_wchar_t ClientStringUtil::s_to_w(const FString& str)
{
	Buf_wchar_t _w;
	int32 srcLen = str.Len() + 1;
	int32 destLen = FPlatformString::ConvertedLength<WIDECHAR>(*str, srcLen);

	if (MAX_PACKET_BUFFER_SIZE <= destLen)
	{
		FMemory::Memcpy((WIDECHAR*)(*_w), L"", sizeof(WIDECHAR));
		return Buf_wchar_t(_w);
	}

	TArray<WIDECHAR> converted;
	converted.AddUninitialized(destLen);
	FPlatformString::Convert(converted.GetData(), destLen, *str, srcLen);

	FMemory::Memcpy(reinterpret_cast<WIDECHAR*>(*_w), converted.GetData(), sizeof(WIDECHAR) * destLen);
	return Buf_wchar_t(_w);
}

FString ClientStringUtil::w_to_s(const wchar_t* wchar)
{
	return FString::Printf(TEXT("%s"), wchar);
}

FText ClientStringUtil::w_to_t(const wchar_t* wchar)
{
	return FText::FromString(w_to_s(wchar));
}
