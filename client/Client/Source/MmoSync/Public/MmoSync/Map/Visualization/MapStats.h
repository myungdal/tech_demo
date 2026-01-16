// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <map>
#include <string>

namespace MmoSync::Map::Unified
{
	// Unified map statistics structure
	struct UnifiedMapStats
	{
		// Basic counts
		int wallCount = 0;
		int floorCount = 0;
		int totalCells = 0;

		// Special locations
		int startCount = 0;
		int exitCount = 0;
		int doorCount = 0;

		// Entities
		int castleCount = 0;
		int nestCount = 0;
		int totalEntityCount = 0;

		// Terrain level distribution
		std::map<int, int> terrainLevelDistribution;

		// Calculated ratios
		double GetFloorRatio() const
		{
			return totalCells > 0 ? static_cast<double>(floorCount) / totalCells : 0.0;
		}

		double GetWallRatio() const
		{
			return totalCells > 0 ? static_cast<double>(wallCount) / totalCells : 0.0;
		}

		double GetEntityRatio() const
		{
			return totalCells > 0 ? static_cast<double>(totalEntityCount) / totalCells : 0.0;
		}

		// Validation
		bool IsValid() const
		{
			return startCount == 1 && exitCount == 1 && floorCount > 0;
		}

		bool IsValidForSmallMap() const
		{
			return startCount == 1 && exitCount == 1 && floorCount >= 10;
		}

		// Get terrain level distribution as percentage
		std::map<int, double> GetTerrainLevelPercentages() const
		{
			std::map<int, double> percentages;
			for (const auto& [level, count] : terrainLevelDistribution)
			{
				percentages[level] = totalCells > 0 ?
					static_cast<double>(count) / totalCells * 100.0 : 0.0;
			}
			return percentages;
		}
	};
}
