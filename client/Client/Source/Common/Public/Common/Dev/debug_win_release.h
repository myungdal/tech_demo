// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


#ifndef _DEBUG

#define _TODO(msg) __pragma(message(".\n" __FILE__ "(" _CRT_STRINGIZE(__LINE__) "): " msg "\n."))
//#define _TODO(msg) /##/

inline void PrintOutput(const wchar_t*)
{
}

template<typename _Color, typename _Format, typename... _Args>
void PrintOutputLog(const _Color, _Format&&, _Args&&...)
{
}

#define _DEBUG_LOG(color, ...) __noop
#define _DEBUG_RED __noop
#define _DEBUG_WHITE __noop

inline void Crash()
{
	throw std::runtime_error("Crash");
}

#define _DEBUG_OUTPUT __noop
#define _DEBUG_BREAK __noop
#define _ASSERT_CRASH(condition) if(!(condition)) Crash()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 메모리 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _CRTDBG_MAP_ALLOC
#undef _CRTDBG_MAP_ALLOC
#endif

#define _DETECT_MEMORY_LEAK_START(_NewValue) __noop

#endif
