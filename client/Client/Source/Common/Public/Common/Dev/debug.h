// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


#if !defined(__UNREAL__)
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

#if _DETECT_MEMORY_VIOLATION // 정렬 문제를 해결해야 하용 가능

constexpr size_t _PAGE_SIZE = 1024;

template <typename _Type, typename... Args>
[[nodiscard]] _Type* _NEW(Args&&... args)
{
	const size_t size = sizeof(_Type);
	const size_t pageCount = (size + _PAGE_SIZE - 1) / _PAGE_SIZE;
	const size_t offset = pageCount * _PAGE_SIZE - size;
	void* base = ::VirtualAlloc(NULL, pageCount * _PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	void* address = reinterpret_cast<void*>(reinterpret_cast<size_t>(base) + offset);
	_Type* ptr = new (address)_Type(std::forward<Args>(args)...);
	return ptr;
}

template <typename _Type>
void _DELETE(_Type* ptr)
{
	ptr->~_Type();
	const size_t address = reinterpret_cast<size_t>(ptr);
	const size_t base = address - (address % _PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(base), 0, MEM_RELEASE);
}

template <typename _Type>
inline _Type* _MALLOC(size_t size)
{
	const size_t pageCount = (size + _PAGE_SIZE - 1) / _PAGE_SIZE;
	const size_t offset = pageCount * _PAGE_SIZE - size;
	void* base = ::VirtualAlloc(NULL, pageCount * _PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	void* address = reinterpret_cast<void*>(reinterpret_cast<size_t>(base) + offset);
	return address;
}

template <typename _Type>
inline void _FREE(_Type* ptr)
{
	const size_t address = reinterpret_cast<size_t>(ptr);
	const size_t base = address - (address % _PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(base), 0, MEM_RELEASE);
}

#elif 0

template <typename _Type, typename... Args>
[[nodiscard]] _Type* _NEW(Args&&... args)
{
	constexpr size_t alignment = alignof(_Type);
	void* ptr = ::operator new(sizeof(_Type), std::align_val_t(alignment));
	return ::new (ptr) _Type(std::forward<Args>(args)...);
}

template <typename _Type>
void _DELETE(_Type* ptr)
{
	constexpr size_t alignment = alignof(_Type);
	ptr->~_Type();
	::operator delete(ptr, std::align_val_t(alignment));
}

template <typename _Type>
inline _Type* _MALLOC(size_t count)
{
	constexpr size_t alignment = alignof(_Type);
	return reinterpret_cast<_Type*>(_aligned_malloc(sizeof(_Type) * count, alignment));
}

template <typename _Type>
inline void _FREE(_Type* ptr)
{
	_aligned_free(reinterpret_cast<void*>(ptr));
}

#else

template <typename _Type, typename... Args>
[[nodiscard]] _Type* _NEW(Args&&... args)
{
	void* raw = ::operator new(sizeof(_Type));
	return ::new (raw) _Type(std::forward<Args>(args)...);
}

template <typename _Type>
void _DELETE(_Type* ptr)
{
	if (!ptr) return;
	ptr->~_Type();
	::operator delete(ptr);
}

// ---------------------------- malloc/free -----------------------------
template <typename _Type>
[[nodiscard]] inline _Type* _MALLOC(std::size_t count)
{
	return static_cast<_Type*>(std::malloc(sizeof(_Type) * count));
}

template <typename _Type>
inline void _FREE(_Type* ptr)
{
	std::free(ptr);
}

#endif


// 생성자와 소멸자를 호출하지 않는 std 컨테이너 용 Allocator
template<class T>
struct MallocAllocator
{
	using value_type = T;

	[[nodiscard]]
	T* allocate(std::size_t n)
	{
		if (auto p = static_cast<T*>(std::malloc(n * sizeof(T))))
			return p;
		throw std::bad_alloc{};
	}

	void deallocate(T* p, std::size_t) noexcept
	{
		std::free(p);
	}

	template<class U> struct rebind { using other = MallocAllocator<U>; };
	using propagate_on_container_move_assignment = std::true_type;
	using is_always_equal = std::true_type;
};

template<class T, class U>
bool operator==(const MallocAllocator<T>&, const MallocAllocator<U>&) { return true; }
template<class T, class U>
bool operator!=(const MallocAllocator<T>&, const MallocAllocator<U>&) { return false; }


// 안드로이드 IOS 컴파일 문제
#if defined(__UNREAL__)

#	if defined(UE_EDITOR)

inline void Crash()
{
	throw std::runtime_error("Crash");
}

#		define __noop static_cast<void>(0)

#		define _TODO(msg) __pragma(message(".\n" __FILE__ "(" _CRT_STRINGIZE(__LINE__) "): " msg "\n."))
#		define _DEBUG_LOG(level, ...) __noop
#		define _DEBUG_RED __noop
#		define _DEBUG_WHITE __noop

#		define _DEBUG_OUTPUT __noop
#		define _DEBUG_BREAK __debugbreak()
#		define _ASSERT_CRASH(condition) if(!(condition)) Crash()


#define _DEBUG_FUNC_LINE (FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

template<typename Color, typename... _Args>
void DebugLogTempUE(Color color, const wchar_t* debugFuncLine, const std::wformat_string<_Args...>& format, _Args&&... args)
{
	std::wstring formattedMessage = std::format(format, std::forward<_Args>(args)...);
	std::wstring finalMessage = std::format(L"{} {} {}", CLOCK_NOW, debugFuncLine, formattedMessage);
	if (color == RED)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), finalMessage.c_str());
	}
	else //if (color == WHITE)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), finalMessage.c_str());
	}
}

#ifdef _DEBUG_LOG
#undef _DEBUG_LOG
#endif
#define _DEBUG_LOG(color, format, ...) DebugLogTempUE(color, *_DEBUG_FUNC_LINE, format, ##__VA_ARGS__)


#	else

#		define __noop static_cast<void>(0)

#		define _TODO(msg) __noop
#		define _DEBUG_LOG(level, ...) __noop
#		define _DEBUG_RED __noop
#		define _DEBUG_WHITE __noop

#		define _DEBUG_OUTPUT __noop
#		define _DEBUG_BREAK __noop
#		define _ASSERT_CRASH(condition) __noop

#	endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 메모리 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#	ifdef _CRTDBG_MAP_ALLOC
#		undef _CRTDBG_MAP_ALLOC
#	endif

#	define _DETECT_MEMORY_LEAK_START(_NewValue) __noop


#else

#	ifdef _DEBUG
#		include "Common/Dev/debug_win_debug.h"
#	else
#		include "Common/Dev/debug_win_release.h"
#	endif

#endif
