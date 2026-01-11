// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define _WILL_USE(x) { void* ptr = reinterpret_cast<void*>(&x); ptr = ptr; }

#define _IDX(x) static_cast<size_t>(x)

#ifndef MAYBE_UNUSED
#define MAYBE_UNUSED [[maybe_unused]]
#endif

#define DISABLE_COPY(CLASSNAME)	\
public: \
	CLASSNAME(const CLASSNAME&) = delete; \
	CLASSNAME(CLASSNAME&&) = delete; \
	CLASSNAME& operator = (const CLASSNAME&) = delete; \
	CLASSNAME& operator = (CLASSNAME&&) = delete
