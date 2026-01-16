// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/ComponentBase/ComponentBase.h"
#include "MmoSync/Component/DamageInfo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 데미지 컴포넌트 - 데미지 정보 참조
// - 데미지 처리를 위한 정보 ID 저장
// - 실제 데미지 정보는 별도 풀에서 관리
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
// 데미지 컴포넌트 - 적용할 데미지 정보 ID
struct DamageComponent : Component<DamageComponent>
{
	DamageInfoId damageInfoId = INVALID_DAMAGE_INFO_ID;  // 데미지 정보 풀 ID
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<DamageComponent, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const DamageComponent& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}damageInfoId: {}", tTabStr, t.damageInfoId);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
