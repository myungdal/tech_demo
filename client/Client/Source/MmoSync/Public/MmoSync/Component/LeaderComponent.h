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
// Leader (리더) - 파티의 리더 역할을 표시하는 태그 컴포넌트
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct LeaderComponent : Component<LeaderComponent>
{
	EntityIdp leaderEntityTargetIdp = INVALID_ENTITY_IDP;

	uint8_t worldUserCommandKeyState = (uint8_t)0;
	uint8_t followerCnt = 0;

	__declspec(noinline) bool HasWorldUserCommandKeyState() const { return worldUserCommandKeyState != 0; }
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<LeaderComponent, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const LeaderComponent& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}leaderEntityTargetIdp: {}", tTabStr, t.leaderEntityTargetIdp);
		result += std::format(L"\n{}worldUserCommandKeyState: {}", tTabStr, t.worldUserCommandKeyState);
		result += std::format(L"\n{}followerCnt: {}", tTabStr, t.followerCnt);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

