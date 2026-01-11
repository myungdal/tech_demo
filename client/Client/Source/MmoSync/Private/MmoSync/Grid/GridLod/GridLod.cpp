// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Grid/GridLod/GridLod.h"


bool GridLod::IsBlocked(size_t level, uint32_t u, uint32_t v) const
{
	if (mLodTable.empty())
		return false;

	if (level > mLevel)
		return false;

	const GridLodMap& map = *mLodTable[level];

	return map.IsBlocked(u, v);
}
