// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


#ifdef _DEBUG

#define _TODO(msg) __pragma(message(".\n" __FILE__ "(" _CRT_STRINGIZE(__LINE__) "): " msg "\n."))
//#define _TODO(msg) /##/

inline void PrintOutput(const wchar_t* str)
{
	_RPTW0(_CRT_WARN, str);
}

template<typename _Color, typename... _Args>
void PrintOutputLog(const _Color color, const wchar_t* function, int line, const std::wformat_string<_Args...>& format, _Args&&... args)
{
	std::wstring formattedMessage = std::format(format, std::forward<_Args>(args)...);

	const wchar_t* prefix = L"";
	if (RED == color) prefix = L"R!";
	else if (GREEN == color) prefix = L"G!";
	else if (YELLOW == color) prefix = L"Y!";
	else if (WHITE == color) prefix = L"W!";

	std::wstring finalMessage = std::format(L"{}{}({})\t{}\n", prefix, function, line, formattedMessage);
	_RPTW0(_CRT_WARN, finalMessage.c_str());
}

#define _DEBUG_LOG(color, format, ...) PrintOutputLog(color, __FUNCTIONW__, __LINE__, format, ##__VA_ARGS__)
#define _DEBUG_RED PrintOutputLog(RED, __FUNCTIONW__, __LINE__, L"")
#define _DEBUG_WHITE PrintOutputLog(WHITE, __FUNCTIONW__, __LINE__, L"")

inline void Crash()
{
	throw std::runtime_error("Crash");
}

#define _DEBUG_OUTPUT(level, ...) PrintOutput(__VA_ARGS__)
#define _DEBUG_BREAK __debugbreak()
#define _ASSERT_CRASH(condition) if(!(condition)) Crash()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 메모리 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define _CRTDBG_MAP_ALLOC
#define _DETECT_MEMORY_LEAK_START(_NewValue) \
	long newValue = _NewValue; \
	if(0 < newValue) _CrtSetBreakAlloc(_NewValue); \
	_CrtDumpMemoryLeaks(); \
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#endif
