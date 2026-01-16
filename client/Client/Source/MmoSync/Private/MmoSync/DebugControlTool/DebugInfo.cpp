// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/DebugControlTool/DebugInfo.h"

#if defined(__UNREAL__)
DebugInfo gDebugInfo;
#endif

// ========================================================================
// Wide string 매크로 헬퍼
// ========================================================================
#define DEBUG_INFO_WIDEN2(x) L ## x
#define DEBUG_INFO_WIDEN(x) DEBUG_INFO_WIDEN2(x)
#define DEBUG_INFO_WSTR(x) DEBUG_INFO_WIDEN(#x)

// ========================================================================
// 맵 정의
// ========================================================================
std::unordered_map<std::wstring, void*> DebugInfo::sVarMap;
std::unordered_map<std::wstring, std::wstring> DebugInfo::sTypeMap;
std::unordered_map<std::wstring, DebugInfo::SetterFunc> DebugInfo::sSetterMap;
std::unordered_map<std::wstring, int64_t> DebugInfo::sKnownConstants = {
	{ L"CELL_SIZE_DESIGN_FACTOR", static_cast<int64_t>(CELL_SIZE_DESIGN_FACTOR) },
	{ L"TIME_STEP_INTERVAL_FACTOR", static_cast<int64_t>(TIME_STEP_INTERVAL_FACTOR) }
};
bool DebugInfo::sInitialized = false;

// ========================================================================
// 변수 정의 (DebugInfoDef.inl에서 자동 생성)
// 값 수정 시 이 파일(cpp)만 재컴파일됨
// ========================================================================
#define DEBUG_VAR_DEF(name, type, value) type DebugInfo::name = value;
#include "MmoSync/DebugControlTool/DebugInfoDef.inl"
#undef DEBUG_VAR_DEF

// ========================================================================
// 함수 구현
// ========================================================================

void DebugInfo::InitVarMap()
{
	if (sInitialized)
		return;
	sInitialized = true;

	// Setter 함수 등록
	sSetterMap[L"int"] = [](void* ptr, int64_t value) { *static_cast<int*>(ptr) = static_cast<int>(value); };
	sSetterMap[L"int32_t"] = [](void* ptr, int64_t value) { *static_cast<int32_t*>(ptr) = static_cast<int32_t>(value); };
	sSetterMap[L"int64_t"] = [](void* ptr, int64_t value) { *static_cast<int64_t*>(ptr) = value; };
	sSetterMap[L"Scalar"] = [](void* ptr, int64_t value) { *static_cast<Scalar*>(ptr) = value; };
	sSetterMap[L"Span"] = [](void* ptr, int64_t value) { *static_cast<Span*>(ptr) = static_cast<Span>(value); };
	sSetterMap[L"EntityId"] = [](void* ptr, int64_t value) { *static_cast<EntityId*>(ptr) = static_cast<EntityId>(value); };
	sSetterMap[L"StatValue"] = [](void* ptr, int64_t value) { *static_cast<StatValue*>(ptr) = static_cast<StatValue>(value); };
	sSetterMap[L"EntityCount"] = [](void* ptr, int64_t value) { *static_cast<EntityCount*>(ptr) = static_cast<EntityCount>(value); };

	// 변수 등록 (DebugInfoDef.inl에서 자동 생성)
#define DEBUG_VAR_DEF(name, type, value) \
	sVarMap[DEBUG_INFO_WSTR(name)] = &name; \
	sTypeMap[DEBUG_INFO_WSTR(name)] = DEBUG_INFO_WSTR(type);
#include "MmoSync/DebugControlTool/DebugInfoDef.inl"
#undef DEBUG_VAR_DEF
}

void DebugInfo::LoadDebugInfoVar(const wchar_t* key, int64_t value)
{
	InitVarMap();

	auto varIt = sVarMap.find(key);
	if (varIt == sVarMap.end())
	{
		_DEBUG_BREAK;
		return;
	}

	auto typeIt = sTypeMap.find(key);
	if (typeIt == sTypeMap.end()) 
	{
		_DEBUG_BREAK;
		return;
	}

	auto setterIt = sSetterMap.find(typeIt->second);
	if (setterIt == sSetterMap.end()) 
	{
		_DEBUG_BREAK;
		return;
	}

	setterIt->second(varIt->second, value);
}
