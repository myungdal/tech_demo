// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <unordered_map>
#include <functional>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테스트용 데이터 실시간 반영 (소스코드를 동적 데이터 겸용으로 사용)
// - 사용법: DebugInfo::DEBUG_ENTITY_ID (직접 접근)
// - 런타임 수정: gDebugInfo.LoadDebugInfoVar(L"DEBUG_ENTITY_ID", value)
// 
// 주의: static 변수는 모듈(DLL)간 공유를 위해 .cpp에서 정의됨
//
// 변수 추가 방법:
// - DebugInfoDef.inl 파일에 DEBUG_VAR_DEF(이름, 타입, 값) 한 줄만 추가하면 됨
// - 값에는 상수 표현식도 사용 가능 (예: CELL_SIZE_HALF, 1000 << FACTOR)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MMOSYNC_API DebugInfo
{
	// ========================================================================
	// 타입별 Setter 함수 타입
	// ========================================================================
	using SetterFunc = std::function<void(void*, int64_t)>;

	// ========================================================================
	// 변수 맵: 이름 -> 주소
	// 타입 맵: 이름 -> 타입 문자열 (파싱으로 채워짐)
	// Setter 맵: 타입 문자열 -> Setter 함수
	// 상수 맵: 파싱 시 사용되는 알려진 상수들 (CELL_SIZE_FACTOR 등)
	// ========================================================================
	static std::unordered_map<std::wstring, void*> sVarMap;
	static std::unordered_map<std::wstring, std::wstring> sTypeMap;
	static std::unordered_map<std::wstring, SetterFunc> sSetterMap;
	static std::unordered_map<std::wstring, int64_t> sKnownConstants;
	static bool sInitialized;

	// ========================================================================
	// 변수 선언 (DebugInfoDef.inl에서 자동 생성)
	// ========================================================================
#define DEBUG_VAR_DEF(name, type, value) static type name;
#include "MmoSync/DebugControlTool/DebugInfoDef.inl"
#undef DEBUG_VAR_DEF

	// ========================================================================
	// 함수 선언 (구현은 DebugInfo.cpp)
	// ========================================================================
	static void InitVarMap();
	void LoadDebugInfoVar(const wchar_t* key, int64_t value);
};

#if defined(__UNREAL__)
MMOSYNC_API extern DebugInfo gDebugInfo;
#else
inline DebugInfo gDebugInfo;
#endif
