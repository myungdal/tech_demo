// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/GridLod/Internal/GridLodMap.h"


class GridLod
{
private:
	using Table = std::vector<std::unique_ptr<GridLodMap>>;

	Table mLodTable;

private:
	size_t mLevel = 0;
	size_t mWidth = 0;
	size_t mHeight = 0;

public:
	size_t GetLevel() const noexcept { return mLevel; }

public:
	template <typename _IsBlocked>
	void Build(uint32_t width, uint32_t height, _IsBlocked IsBlocked)
	{
		mLevel = 0;
		mWidth = width;
		mHeight = height;

		while (true)
		{
			auto map = std::make_unique<GridLodMap>(width, height);

			if (mLevel == 0)
			{
				for (uint32_t v = 0; v < height; ++v)
					for (uint32_t u = 0; u < width; ++u)
						if (IsBlocked(u, v))
							map->SetBlocked(u, v);
			}
			else
			{
				GridLodMap& parent = *mLodTable[mLevel - 1];

				for (uint32_t v = 0; v < height; ++v)
				{
					for (uint32_t u = 0; u < width; ++u)
					{
						const uint32_t pu = (u << 1);
						const uint32_t ph = (v << 1);

						// parent 의 4개의 값을 확인하여 두개 이상이 true이면 true로 설정
						int sum = 0;
						sum += parent.IsBlocked(pu + 0, ph + 0) ? 1 : 0;
						sum += parent.IsBlocked(pu + 1, ph + 0) ? 1 : 0;
						sum += parent.IsBlocked(pu + 0, ph + 1) ? 1 : 0;
						sum += parent.IsBlocked(pu + 1, ph + 1) ? 1 : 0;
						if (sum >= 2)
							map->SetBlocked(u, v);

					}
				}
			}

			mLodTable.push_back(std::move(map));

			const bool canBuild = ((width > 0) && (width % 2) == 0 && (height > 0) && (height % 2) == 0);
			if (!canBuild)
				break;

			++mLevel;
			width = (width >> 1);
			height = (height >> 1);
		}
	}

public:
	bool IsBlocked(size_t level, uint32_t u, uint32_t v) const;
};
