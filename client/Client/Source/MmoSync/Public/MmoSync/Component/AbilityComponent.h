// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/ComponentBase/ComponentBase.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 능력 컴포넌트 - 엔티티의 특수 능력 태그
// - 그리드 차단/해제, 회복, 타게팅 불가 등의 능력 표시
// - 태그형 컴포넌트로 존재 여부만으로 능력 판별
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
// 그리드 차단 능력 - 이 컴포넌트가 있으면 그리드를 차단해야 함
struct AbilityComponentGridBlock : Component<AbilityComponentGridBlock>
{
};

// 그리드 해제 능력 - 이 컴포넌트가 있으면 그리드 차단을 해제해야 함
struct AbilityComponentGridUnblock : Component<AbilityComponentGridUnblock>
{
};

// 그리드 적용 상태 - 그리드 차단/해제가 실제로 적용된 상태
struct AbilityComponentGridApplied : Component<AbilityComponentGridApplied>
{
	bool isBlocked = false;   // 차단 여부
	Scalar x = -1;            // 적용된 위치 X
	Scalar y = -1;            // 적용된 위치 Y
	GridCol c = -1;           // 적용된 그리드 컬럼
	GridRow r = -1;           // 적용된 그리드 로우
};

// 회복 능력 - HP 자동 회복 가능
struct AbilityComponentRecovery : Component<AbilityComponentRecovery>
{
};

// 타게팅 불가 - 적의 타겟이 될 수 없음
struct AbilityComponentUntargetrable : Component<AbilityComponentUntargetrable>
{
}; 
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<AbilityComponentGridBlock, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const AbilityComponentGridBlock& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

template<typename _CharType>
struct std::formatter<AbilityComponentGridUnblock, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const AbilityComponentGridUnblock& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

template<typename _CharType>
struct std::formatter<AbilityComponentGridApplied, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const AbilityComponentGridApplied& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

template<typename _CharType>
struct std::formatter<AbilityComponentRecovery, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const AbilityComponentRecovery& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

template<typename _CharType>
struct std::formatter<AbilityComponentUntargetrable, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const AbilityComponentUntargetrable& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
