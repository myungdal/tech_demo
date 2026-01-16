// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class GridLodMap 
{
private:
	uint32_t mWidth = 0;
	uint32_t mHeight = 0;
	std::vector<bool> mData;

public:
	explicit GridLodMap(uint32_t width, uint32_t height)
		:
		mWidth(width), 
		mHeight(height)
	{
		mData.resize(width * height, false);
	}

	bool IsBlocked(uint32_t u, uint32_t v) const { return mData.at(CalcIndex(u, v)); }
	void SetBlocked(uint32_t u, uint32_t v) { mData.at(CalcIndex(u, v)) = true; }

private:
	size_t CalcIndex(uint32_t u, uint32_t v) const { return std::clamp(v, 0u, mHeight - 1) * mWidth + std::clamp(u, 0u, mWidth - 1); }
};
