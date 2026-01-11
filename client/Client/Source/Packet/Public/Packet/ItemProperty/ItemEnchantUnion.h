// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ItemEnchantUnion
// 아이템의 8개 속성에 대한 강화 종류를 저장하는 union 구조체입니다.
// (0: 보통, 1: 성공)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct ItemEnchantUnion
{
	union
	{
		uint8_t itemEnchant[8]; // s_stat 테이블 c_stat_value_upgrade?, ? = grade + 0(보통) 또는 1(성공) 로 구할 수 있다.
		int64_t value = 0;
	};

	ItemEnchantUnion() = default;
	explicit ItemEnchantUnion(int64_t _value)
		:
		value(_value)
	{
	}
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<ItemEnchantUnion, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const ItemEnchantUnion& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		for (size_t i = 0; i < 8; ++i) {
			result += std::format(L"\n{}itemEnchant[{}]: {:b}", tTabStr, i, t.itemEnchant[i]);
		}
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
