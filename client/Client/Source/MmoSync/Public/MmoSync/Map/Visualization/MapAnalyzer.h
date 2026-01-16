// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MapRegistry.h"
#include "MapStats.h"
#include "MmoSync/Map/Types/AsciiMap.h"
#include <memory>
#include <map>
#include <set>
#include <queue>
#include <string>

namespace MmoSync::Map::Unified
{

	// Unified map analyzer for all map types
	class UnifiedMapAnalyzer
	{
	private:
		std::shared_ptr<MapTypeRegistry> mRegistry;

	public:
		explicit UnifiedMapAnalyzer(std::shared_ptr<MapTypeRegistry> registry);
		~UnifiedMapAnalyzer() = default;

		// Entity analysis
		template<typename MapType>
		std::map<std::wstring, int> AnalyzeEntities(const MapType& map);

		// Registry access
		std::shared_ptr<MapTypeRegistry> GetRegistry() const { return mRegistry; }

	private:
		// Helper methods
		template<typename MapType>
		wchar_t GetCharacterAt(const MapType& map, int x, int y);
	};

	// Template implementations (must be in header)
	template<typename MapType>
	inline std::map<std::wstring, int> UnifiedMapAnalyzer::AnalyzeEntities(const MapType& map)
	{
		std::map<std::wstring, int> entityCounts;

		for (int y = 0; y < map.Height(); ++y)
		{
			for (int x = 0; x < map.Width(); ++x)
			{
				wchar_t ch = GetCharacterAt(map, x, y);
				auto def = mRegistry->GetAsciiDefinition(ch);

				if (def.category == AsciiCategory::Npc)
				{
					entityCounts[def.name]++;
				}
			}
		}

		return entityCounts;
	}

	template<typename MapType>
	inline wchar_t UnifiedMapAnalyzer::GetCharacterAt(const MapType& map, int x, int y)
	{
		return map.Get(x, y);
	}
}
