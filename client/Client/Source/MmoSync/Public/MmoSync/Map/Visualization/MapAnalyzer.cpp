// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MapAnalyzer.h"
#include <queue>
#include <set>
#include <algorithm>

namespace MmoSync::Map::Unified
{
	UnifiedMapAnalyzer::UnifiedMapAnalyzer(std::shared_ptr<MapTypeRegistry> registry)
		: mRegistry(registry)
	{
	}

}
