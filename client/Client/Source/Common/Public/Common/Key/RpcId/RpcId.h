// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RPC 식별자 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 원격 호출(RPC)의 식별자.
// - 책임: RPC 호출 식별 및 매칭
#pragma pack(push, 1)
using RpcIdx = uint32_t;    // RPC 인덱스 타입
using RpcToken = uint32_t;  // RPC 토큰 타입

struct RpcId
{
	RpcIdx mIdx = UINT32_MAX;      // RPC 인덱스
	RpcToken mToken = UINT32_MAX;  // RPC 토큰

	// 두 RPC ID가 같은지 비교한다.
	// - other: 비교할 RPC ID
	// - return: 같으면 true
	bool operator==(const RpcId& other) const
	{
		return (mIdx == other.mIdx) && (mToken == other.mToken);
	}
};

constexpr RpcId INVALID_RPC_ID = RpcId{ UINT32_MAX, UINT32_MAX };  // 무효한 RPC ID
#pragma pack(pop)


template<typename _CharType>
struct std::formatter<RpcId, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const RpcId& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}RpcIdx: {}", tTabStr, t.mIdx);
		result += std::format(L"\n{}RpcToken: {}", tTabStr, t.mToken);
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

