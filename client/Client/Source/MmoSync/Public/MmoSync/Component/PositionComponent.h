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
// 위치
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct PositionComponent : public Component<PositionComponent>
{
	Scalar x = 0;
	Scalar y = 0;

	inline Scalar CalcSquaredLength(const PositionComponent& p1) const
	{
		const Scalar dx = (x - p1.x);
		const Scalar dy = (y - p1.y);
		return (dx * dx) + (dy * dy);
	}

	inline Scalar CalcSquaredLength(Scalar _x, Scalar _y) const
	{
		const Scalar dx = (x - _x);
		const Scalar dy = (y - _y);
		return (dx * dx) + (dy * dy);
	}
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<PositionComponent, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const PositionComponent& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}x: {}({})", tTabStr, t.x >> CELL_SIZE_FACTOR, t.x);
		result += std::format(L"\n{}y: {}({})", tTabStr, t.y >> CELL_SIZE_FACTOR, t.y);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
