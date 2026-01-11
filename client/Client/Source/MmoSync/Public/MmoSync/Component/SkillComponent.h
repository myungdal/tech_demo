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
// 스킬 컴포넌트 - 엔티티의 스킬 상태 및 렌더링 정보
// - 스킬별 쿨타임, 캐스팅, 타겟 관리
// - 클라이언트 렌더링을 위한 이펙트 정보 포함
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class STATIC_CHARACTER_SKILL;

#pragma pack(push, 1)
// 스킬 컴포넌트 베이스 - 모든 스킬의 공통 속성
template<typename _Component, typename _ComponentGroup = _Component>
struct SkillComponentBase : Component< _Component, _ComponentGroup>
{
	// 렌더링 정보 - 클라이언트 이펙트 표시용
	Scalar renderSkillX = 0;           // 이펙트 목표 지점 X (Line: 도착점, Circle/Obb/Sector: 중심점)
	Scalar renderSkillY = 0;           // 이펙트 목표 지점 Y
	Scalar renderSkillR = 0;           // 원형/부채꼴 이펙트 반지름
	Scalar renderSkillW = 0;           // 직사각형 이펙트 너비 (레이저 등)
	Scalar renderSkillH = 0;           // 직사각형 이펙트 높이
	Angle  renderSkillRotation = 0;    // 이펙트 회전 각도 (직사각형, 부채꼴 방향)
	Angle  renderSkillHalfAngle = 0;   // 부채꼴 중심각의 절반 (퍼짐 정도)

	// 스킬 로직 정보
	StaticCharacterSkillPtr staticCharacterSkillPtr = 0;  // 정적 스킬 데이터 포인터(수명: StaticData, 0이면 미설정)
	Span   cooltime = 0;                                  // 남은 쿨타임 (틱/프레임 단위, 시스템 정책에 따름)
	EntityIdp skillTargetIdp = INVALID_ENTITY_IDP;        // 스킬 타겟 엔티티 ID
	Span   castingDelay = 0;                              // 캐스팅 딜레이 (틱/프레임 단위, 시스템 정책에 따름)

	// 정적 스킬 데이터 접근
	STATIC_CHARACTER_SKILL& GetStaticCharacterSkill()
	{
		// [WHY] staticCharacterSkillPtr는 정적 데이터의 포인터를 들고 있으며, 유효한 타입/수명을 가정한다.
		return *reinterpret_cast<STATIC_CHARACTER_SKILL*>(staticCharacterSkillPtr);
	}
};

// 개별 스킬 컴포넌트 - 각 무기/스킬 타입별
// SkillComponentAxe: 도끼(근접) 스킬 상태
struct SkillComponentAxe : SkillComponentBase<SkillComponentAxe> {};          // 도끼 - 근접 공격
// SkillComponentBow: 활(원거리) 스킬 상태
struct SkillComponentBow : SkillComponentBase<SkillComponentBow> {};          // 활 - 원거리 공격
// SkillComponentSpear: 창(관통) 스킬 상태
struct SkillComponentSpear : SkillComponentBase<SkillComponentSpear> {};      // 창 - 관통 공격
// SkillComponentBazooka: 바주카(광역) 스킬 상태
struct SkillComponentBazooka : SkillComponentBase<SkillComponentBazooka> {};  // 바주카 - 광역 폭발
// SkillComponentBomb: 폭탄(설치/폭발) 스킬 상태
struct SkillComponentBomb : SkillComponentBase<SkillComponentBomb> {};        // 폭탄 - 설치/폭발
// SkillComponentHammer: 망치(넉백) 스킬 상태
struct SkillComponentHammer : SkillComponentBase<SkillComponentHammer> {};    // 망치 - 넉백 공격
// SkillComponentHeal: 회복 스킬 상태
struct SkillComponentHeal : SkillComponentBase<SkillComponentHeal> {};        // 힐 - 회복 스킬
// SkillComponentParasite: 기생(지속) 스킬 상태
struct SkillComponentParasite : SkillComponentBase<SkillComponentParasite> {};// 기생 - 지속 피해/흡수
// SkillComponentPoison: 독(지속) 스킬 상태
struct SkillComponentPoison : SkillComponentBase<SkillComponentPoison> {};    // 독 - 지속 피해
// SkillComponentWeb: 거미줄(이속 감소) 스킬 상태
struct SkillComponentWeb : SkillComponentBase<SkillComponentWeb> {};          // 거미줄 - 이동 속도 감소
// SkillComponentSpawn: 소환 스킬 상태
struct SkillComponentSpawn : SkillComponentBase<SkillComponentSpawn> {};      // 소환 - 엔티티 생성
// SkillComponentBlocking: 방어/차단 스킬 상태
struct SkillComponentBlocking : SkillComponentBase<SkillComponentBlocking> {};// 방어 - 피해 차단
// SkillComponentLaser: 레이저(직선) 스킬 상태
struct SkillComponentLaser : SkillComponentBase<SkillComponentLaser> {};      // 레이저 - 직선 관통
// SkillComponentSword: 검(기본 근접) 스킬 상태
struct SkillComponentSword : SkillComponentBase<SkillComponentSword> {};      // 검 - 기본 근접
// SkillComponentDash: 돌진(이동) 스킬 상태
struct SkillComponentDash : SkillComponentBase<SkillComponentDash> {};        // 돌진 - 이동 공격
// SkillComponentElectric: 전기(연쇄) 스킬 상태
struct SkillComponentElectric : SkillComponentBase<SkillComponentElectric> {};// 전기 - 연쇄 공격
// SkillComponentVirus: 바이러스(전염) 스킬 상태
struct SkillComponentVirus : SkillComponentBase<SkillComponentVirus> {};      // 바이러스 - 전염 피해
// SkillComponentVampire: 흡혈 스킬 상태
struct SkillComponentVampire : SkillComponentBase<SkillComponentVampire> {};  // 흡혈 - HP 흡수
// SkillComponentBlink: 위치 변경(블링크) 스킬 상태
struct SkillComponentBlink : SkillComponentBase<SkillComponentBlink> {};      // 순간이동 - 위치 변경

// 투사체 스킬 - 추가 상태 필요
struct SkillComponentProjectile : SkillComponentBase<SkillComponentProjectile>
{
	Scalar targetX = 0;         // 투사체 목표 위치 X (Scalar 고정소수점)
	Scalar targetY = 0;         // 투사체 목표 위치 Y (Scalar 고정소수점)
	Span   elapsedBombTime = 0; // 폭발까지 경과 시간(틱)
	Span   bombInterval = 0;    // 폭발 간격(틱)
};
#pragma pack(pop)


// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentAxe, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentAxe& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentBow, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentBow& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentSpear, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentSpear& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentBazooka, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentBazooka& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentBomb, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentBomb& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentHammer, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentHammer& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentHeal, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentHeal& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentParasite, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentParasite& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentPoison, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentPoison& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentWeb, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentWeb& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentSpawn, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentSpawn& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentBlocking, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentBlocking& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentLaser, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentLaser& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentSword, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentSword& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentDash, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentDash& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentElectric, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentElectric& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentVirus, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentVirus& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentVampire, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentVampire& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentBlink, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const SkillComponentBlink& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

// std::formatter 특수화: 디버그/로그 출력용
template<typename _CharType>
struct std::formatter<SkillComponentProjectile, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const SkillComponentProjectile& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}cooltime: {}", tTabStr, t.cooltime);
		result += std::format(L"\n{}skillTargetIdp: {}", tTabStr, t.skillTargetIdp);
		result += std::format(L"\n{}castingDelay: {}", tTabStr, t.castingDelay);
		result += std::format(L"\n{}targetX: {}({})", tTabStr, t.targetX >> CELL_SIZE_FACTOR, t.targetX);
		result += std::format(L"\n{}targetY: {}({})", tTabStr, t.targetY >> CELL_SIZE_FACTOR, t.targetY);
		result += std::format(L"\n{}elapsedBombTime: {}", tTabStr, t.elapsedBombTime);
		result += std::format(L"\n{}bombInterval: {}", tTabStr, t.bombInterval);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
