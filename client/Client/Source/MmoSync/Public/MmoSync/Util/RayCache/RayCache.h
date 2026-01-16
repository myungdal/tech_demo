// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Util/GridUtil/GridUtil.h"
#include <unordered_map>
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RayCache - 프레임 단위 시야 체크 캐싱
// 같은 프레임에 동일한 (시작셀, 종료셀) 조합의 시야 체크 결과를 캐싱하여 중복 연산 방지
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RayCache
{
private:
	std::unordered_map<uint64_t, bool> mCache;

public:
	// 캐시 초기화 (매 프레임 시작 시 호출)
	void Clear()
	{
		mCache.clear();
	}

	// 캐시 키 생성 (양방향 정규화: min/max로 정렬하여 A→B와 B→A가 같은 키 사용)
	// 주의: col/row는 0~65535 범위로 제한됨 (16비트 패킹)
	static uint64_t MakeKey(GridCol c0, GridRow r0, GridCol c1, GridRow r1)
	{
		// 범위 검증: 16비트 패킹이므로 0~65535 범위여야 함
		_ASSERT(c0 >= 0 && c0 <= 65535 && "RayCache: col out of 16-bit range");
		_ASSERT(r0 >= 0 && r0 <= 65535 && "RayCache: row out of 16-bit range");
		_ASSERT(c1 >= 0 && c1 <= 65535 && "RayCache: col out of 16-bit range");
		_ASSERT(r1 >= 0 && r1 <= 65535 && "RayCache: row out of 16-bit range");

		// 정규화: (c0,r0) < (c1,r1) 순서로 정렬
		if (c0 > c1 || (c0 == c1 && r0 > r1))
		{
			std::swap(c0, c1);
			std::swap(r0, r1);
		}

		// 상위 32비트: 시작 셀, 하위 32비트: 종료 셀
		// 각 셀은 16비트씩 (col, row)
		const uint64_t start = (static_cast<uint64_t>(static_cast<uint16_t>(c0)) << 16) |
			static_cast<uint64_t>(static_cast<uint16_t>(r0));
		const uint64_t end = (static_cast<uint64_t>(static_cast<uint16_t>(c1)) << 16) |
			static_cast<uint64_t>(static_cast<uint16_t>(r1));

		return (start << 32) | end;
	}

	// 캐시에서 조회 (없으면 false 반환, found에 히트 여부 저장)
	bool TryGet(uint64_t key, bool& outResult) const
	{
		auto it = mCache.find(key);
		if (it != mCache.end())
		{
			outResult = it->second;
			return true; // 캐시 히트
		}
		return false; // 캐시 미스
	}

	// 캐시에 저장
	void Set(uint64_t key, bool result)
	{
		mCache[key] = result;
	}

	// 캐시된 시야 체크 (템플릿으로 IsBlockFunc 받음)
	template<typename IsBlockFunc>
	bool IsRayBlockedCached(
		GridCol lastCol, GridRow lastRow,
		GridCol c0, GridRow r0,
		GridCol c1, GridRow r1,
		int32_t rayLength,
		IsBlockFunc isBlockFunc)
	{
		const uint64_t key = MakeKey(c0, r0, c1, r1);

		// 캐시 조회
		bool cachedResult;
		if (TryGet(key, cachedResult))
			return cachedResult;

		// 캐시 미스: 실제 시야 체크 수행
		const bool result = GridUtil::IsRayBlocked(lastCol, lastRow, c0, r0, c1, r1, rayLength, isBlockFunc);

		// 결과 캐싱
		Set(key, result);

		return result;
	}

	// 캐시 사이즈 (디버깅용)
	size_t GetCacheSize() const { return mCache.size(); }
};

