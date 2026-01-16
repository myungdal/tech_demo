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
// 스포니 객체의 데이터
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct SpawneeComponent : public Component<SpawneeComponent>
{
	EntityIdp spawnerIdp = INVALID_ENTITY_IDP; // 스폰너
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<SpawneeComponent, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const SpawneeComponent& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}spawnerIdp: {}", tTabStr, t.spawnerIdp);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
