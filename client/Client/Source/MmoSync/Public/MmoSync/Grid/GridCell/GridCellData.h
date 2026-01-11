// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/MultiGridDef.h"


struct GridCellData
{
	uint8_t mColCountFactor = 0;
	uint8_t mRowCountFactor = 0;
	GridCol mColCount = 0;
	GridRow mRowCount = 0;

	GridCol mLastCol = 0;
	GridRow mLastRow = 0;

	// 단순한 blocked/unblocked 상태 저장
	GridCollisionMap mCellBlockTable;

	explicit GridCellData(uint8_t colCountFactor, uint8_t rowCountFactor)
		:
		mColCountFactor(colCountFactor),
		mRowCountFactor(rowCountFactor),
		mColCount(1 << colCountFactor),
		mRowCount(1 << rowCountFactor),
		mLastCol(mColCount - 1),
		mLastRow(mRowCount - 1)
	{
	}
};

template<typename _CharType>
struct std::formatter<GridCellData, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const GridCellData& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}mColCountFactor: {}", tTabStr, t.mColCountFactor);
		result += std::format(L"\n{}mRowCountFactor: {}", tTabStr, t.mRowCountFactor);
		result += std::format(L"\n{}mColCount: {}", tTabStr, t.mColCount);
		result += std::format(L"\n{}mRowCount: {}", tTabStr, t.mRowCount);
		result += std::format(L"\n{}mLastCol: {}", tTabStr, t.mLastCol);
		result += std::format(L"\n{}mLastRow: {}", tTabStr, t.mLastRow);

		// blocked cells 출력
		result += std::format(L"\n{}mCellBlockTable true cells:", tTabStr);
		size_t totalSize = t.mRowCount * t.mColCount;
		for (size_t idx = 0; idx < totalSize; ++idx) {
			if (t.mCellBlockTable.test(idx)) {
				size_t col = idx % t.mColCount;
				size_t row = idx / t.mColCount;
				result += std::format(L"\n{}  - ({}, {})", tTabStr, col, row);
			}
		}

		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};

