// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


// 패킷 로그 타입을 문자열로 변환
template<typename _CharType>
struct std::formatter<PacketType, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED const PacketType& t, auto& ctx) const {
		std::wstring result = std::format(L"{}", GetPacketTypeString(t));
		return std::ranges::copy(result, ctx.out()).out;
	}
};
