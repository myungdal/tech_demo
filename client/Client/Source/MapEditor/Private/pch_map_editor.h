// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#pragma warning(disable: 4828 4100 4267 4324 4291 4251 4127)
#include "Common/PCH/pch_common.h"
#include "MmoSync/PCH/pch_mmoSync.h"
#include "Packet/PCH/pch_packet.h"


DECLARE_LOG_CATEGORY_EXTERN(MapEditor, Log, All);

template<typename Color, typename... _Args>
void DebugLogUE(Color color, const wchar_t* debugFuncLine, const std::wformat_string<_Args...>& format, _Args&&... args)
{
	std::wstring formattedMessage = std::format(format, std::forward<_Args>(args)...);
	std::wstring finalMessage = std::format(L"{} {} {}", CLOCK_NOW, debugFuncLine, formattedMessage);
	if (color == RED)
	{
		UE_LOG(MapEditor, Error, TEXT("%s"), finalMessage.c_str());
	}
	else //if (color == WHITE)
	{
		UE_LOG(MapEditor, Log, TEXT("%s"), finalMessage.c_str());
	}
}

#ifdef _DEBUG_LOG
#undef _DEBUG_LOG
#endif
#define _DEBUG_LOG(color, format, ...) DebugLogUE(color, *_DEBUG_FUNC_LINE, format, ##__VA_ARGS__)
