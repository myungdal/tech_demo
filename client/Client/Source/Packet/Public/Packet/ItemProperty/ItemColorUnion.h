// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ItemColorUnion
// 아이템의 8개 속성에 대한 색상 정보를 저장하는 union 구조체입니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct ItemColorUnion
{
	union
	{
		ItemColor itemColor[8];
		int64_t value = 0;
	};

	ItemColorUnion() = default;
	explicit ItemColorUnion(int64_t _value)
		:
		value(_value)
	{
	}
};
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<ItemColorUnion, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const ItemColorUnion& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		for (size_t i = 0; i < 8; ++i) {
			result += std::format(L"\n{}itemColor[{}]: {}", tTabStr, i, t.itemColor[i]);
		}
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
