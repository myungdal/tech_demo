// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DebugMapSystem.h"
#include <fstream>

namespace MmoSync::Map::Unified
{
	UnifiedMapSystem::UnifiedMapSystem(std::shared_ptr<MapTypeRegistry> registry)
		: mRegistry(registry)
	{
		InitializeComponents();
	}


	std::wstring UnifiedMapSystem::GetLegend() const
	{
		return mRenderer->GenerateLegend();
	}

	bool UnifiedMapSystem::IsValid() const
	{
		return mRegistry != nullptr && mAnalyzer != nullptr && mRenderer != nullptr;
	}

	void UnifiedMapSystem::InitializeComponents()
	{
		if (mRegistry)
		{
			mAnalyzer = std::make_unique<UnifiedMapAnalyzer>(mRegistry);
			mRenderer = std::make_unique<UnifiedMapRenderer>(mRegistry);
		}
	}
}
