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
// 충격
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct ImpulseComponent : public Component<ImpulseComponent>
{
	Scalar ix = 0;
	Scalar iy = 0;
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<ImpulseComponent, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const ImpulseComponent& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}ix: {}({})", tTabStr, t.ix >> CELL_SIZE_FACTOR, t.ix);
		result += std::format(L"\n{}iy: {}({})", tTabStr, t.iy >> CELL_SIZE_FACTOR, t.iy);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
