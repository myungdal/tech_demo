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
// 스포너 객체의 데이터
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct SpawnerComponent : public Component<SpawnerComponent>
{
	Scalar defaultCommandX = 0; // 기본 공격 지점
	Scalar defaultCommandY = 0; // 기본 공격 지점

	EntityCount currentActiveSpawnCount = 0; // 현재 활성화된 스폰된 개체 수

	Span totalAccumulatedSpawnTime = 0; // 누적된 스폰 코스트 시간
	Span elapsedSpawnCooldownTime = 0; // 스폰 간격 중 경과한 시간
	
	EntityIdp pairCastleIdp = INVALID_ENTITY_IDP; // 페어 캐슬의 EntityIdp (캐슬 전용)
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<SpawnerComponent, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SpawnerComponent& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}defaultCommandX: {}({})", tTabStr, t.defaultCommandX >> CELL_SIZE_FACTOR, t.defaultCommandX);
		result += std::format(L"\n{}defaultCommandY: {}({})", tTabStr, t.defaultCommandY >> CELL_SIZE_FACTOR, t.defaultCommandY);
		result += std::format(L"\n{}currentActiveSpawnCount: {}", tTabStr, t.currentActiveSpawnCount);
		result += std::format(L"\n{}totalAccumulatedSpawnTime: {}", tTabStr, t.totalAccumulatedSpawnTime);
		result += std::format(L"\n{}elapsedSpawnCooldownTime: {}", tTabStr, t.elapsedSpawnCooldownTime);
		result += std::format(L"\n{}pairCastleIdp: {}.{}", tTabStr, t.pairCastleIdp.first, t.pairCastleIdp.second);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
