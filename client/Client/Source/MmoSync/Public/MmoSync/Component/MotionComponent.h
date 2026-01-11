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
// 모션
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
// GroupA: 스스로 움직일 수 있는 모션 그룹
struct MotionComponentGroupA : Component<MotionComponentGroupA>
{
};

// SkillA: GroupA(이동 가능) 스킬 모션
struct MotionComponentSkillA : Component<MotionComponentSkillA, MotionComponentGroupA>
{
	ComponentType skillComponentType = ComponentType::NONE;
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};

// Bleeding: GroupA(이동 가능) 출혈 모션
struct MotionComponentBleeding : Component<MotionComponentBleeding, MotionComponentGroupA>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};

// Juggle: GroupA(이동 가능) 공중 띄우기/저글 모션
struct MotionComponentJuggle : Component<MotionComponentJuggle, MotionComponentGroupA>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};

// Dash: GroupA(이동 가능) 대시 모션
struct MotionComponentDash: Component<MotionComponentDash, MotionComponentGroupA>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
	int32_t speedRate = 2048;  // SetSpeed에 전달할 속도 배율 (1024=100%, 2048=200%)
};

// Web: GroupA(이동 가능) 거미줄 모션
struct MotionComponentWeb : Component<MotionComponentWeb, MotionComponentGroupA>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};

// Damage: GroupA(이동 가능) 피격 모션(경직 등)
struct MotionComponentDamage : Component<MotionComponentDamage, MotionComponentGroupA>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};

// GroupB: 스스로 움직이지 못하는 모션 그룹
struct MotionComponentGroupB : Component<MotionComponentGroupB>
{
};

// SkillB: GroupB(이동 불가) 스킬 모션
struct MotionComponentSkillB : Component<MotionComponentSkillB, MotionComponentGroupB>
{
	ComponentType skillComponentType = ComponentType::NONE;
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};

// Shock: GroupB(이동 불가) 쇼크 모션
struct MotionComponentShock : Component<MotionComponentShock, MotionComponentGroupB>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};

// Stun: GroupB(이동 불가) 스턴 모션
struct MotionComponentStun : Component<MotionComponentStun, MotionComponentGroupB>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};

// GroupC: 절대 움직이지 못하는 모션 그룹(스폰/사망 등)
struct MotionComponentGroupC : Component<MotionComponentGroupC>
{
};

// Dying: GroupC(이동 불가) 사망 연출/대기 모션
struct MotionComponentDying : Component<MotionComponentDying, MotionComponentGroupC>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
	EntityIdp killerEntityIdp = INVALID_ENTITY_IDP; // NPC를 죽인 PC의 EntityIdp
};

// Dead: GroupC(이동 불가) 사망 상태 모션
struct MotionComponentDead : Component<MotionComponentDead, MotionComponentGroupC>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
	EntityIdp killerEntityIdp = INVALID_ENTITY_IDP; // NPC를 죽인 PC의 EntityIdp (Dying에서 전달, 참고용)
	EntityIdp capturerIdp = INVALID_ENTITY_IDP;     // Dead 기간 중 충돌한 PC Leader의 EntityIdp (포획 대상)
};

// Arrival: GroupC(이동 불가) 스폰/등장 모션
struct MotionComponentArrival : Component<MotionComponentArrival, MotionComponentGroupC>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
}; 

// Departure: GroupC(이동 불가) 퇴장 모션
struct MotionComponentDeparture : Component<MotionComponentDeparture, MotionComponentGroupC>
{
	Span now = 0;       // 경과 시간(틱)
	Span duration = 0;  // 전체 지속 시간(틱)
};
#pragma pack(pop)


// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentGroupA, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const MotionComponentGroupA& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};


// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentGroupB, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const MotionComponentGroupB& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};


// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentGroupC, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const MotionComponentGroupC& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}", tTabStr);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentBleeding, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentBleeding& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentJuggle, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentJuggle& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentShock, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentShock& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentStun, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentStun& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentDash, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentDash& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentWeb, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentWeb& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentSkillA, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentSkillA& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentSkillB, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentSkillB& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentDamage, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentDamage& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentDying, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentDying& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentDead, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentDead& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};


// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentArrival, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentArrival& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<MotionComponentDeparture, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const MotionComponentDeparture& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}now: {}", tTabStr, t.now);
		result += std::format(L"\n{}duration: {}", tTabStr, t.duration);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
