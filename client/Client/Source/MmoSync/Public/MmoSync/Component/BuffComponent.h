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
// 버프 컴포넌트 - 엔티티에 적용된 긍정적 효과
// - 그룹 컴포넌트로 모든 버프 관리
// - 지속 시간 기반 자동 제거
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
// 버프 그룹 - 모든 버프의 부모 컴포넌트
struct BuffComponentGroup : Component<BuffComponentGroup>
{
};

// 힐 버프 - 지속 회복 효과
struct BuffComponentHeal : Component<BuffComponentHeal, BuffComponentGroup>
{
	Span now = 0;       // 경과 시간 (틱)
	Span duration = 0;  // 총 지속 시간 (틱)
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<BuffComponentGroup, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const BuffComponentGroup& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

template<typename _CharType>
struct std::formatter<BuffComponentHeal, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const BuffComponentHeal& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
