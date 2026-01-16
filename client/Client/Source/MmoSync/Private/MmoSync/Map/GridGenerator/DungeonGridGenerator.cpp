// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Map/GridGeneration/DungeonGridGenerator.h"
#include "MmoSync/Map/Types/MapGenerationSettings.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"
#include "Common/Random/DeterministicRandom/DeterministicRandom32.h"

using namespace MmoSync::Map::Special;
using namespace MmoSync::Map::TerrainLevel;

#include <algorithm>
#include <cstdint>
#include <queue>
#include <utility>
#include <vector>
#include <tuple>

namespace
{
	using Point = std::pair<int, int>;

	constexpr int kDirs4[4][2] = {
		{ 1, 0 },
		{ -1, 0 },
		{ 0, 1 },
		{ 0, -1 }
	};

	// Using constants from MapCharacterConstants.h

	inline size_t ToIndex(int x, int y, int width)
	{
		return static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x);
	}

	// Find the farthest point from a given start point using BFS
	Point FindFarthestPoint(const AsciiMap& map, const Point& start, int width, int height)
	{
		const size_t total = static_cast<size_t>(width) * static_cast<size_t>(height);
		std::vector<int> dist(total, -1);
		std::queue<Point> q;

		q.push(start);
		dist[ToIndex(start.first, start.second, width)] = 0;

		Point ret = start;
		int best = 0;

		while (!q.empty())
		{
			Point cur = q.front();
			q.pop();
			const int curDist = dist[ToIndex(cur.first, cur.second, width)];
			if (curDist > best)
			{
				best = curDist;
				ret = cur;
			}

			for (const auto& dir : kDirs4)
			{
				const int nx = cur.first + dir[0];
				const int ny = cur.second + dir[1];
				if (nx < 0 || ny < 0 || nx >= width || ny >= height) continue;
				if (map.Get(nx, ny) != LEVEL0) continue;

				const size_t ni = ToIndex(nx, ny, width);
				if (dist[ni] >= 0) continue;

				dist[ni] = curDist + 1;
				q.emplace(nx, ny);
			}
		}

		return ret;
	}
}

DungeonGridGenerator::DungeonGridGenerator(const MapConfig& mapConfig)
	: GridGeneratorBase(), mMapConfig(mapConfig)
{
}

std::unique_ptr<AsciiMap> DungeonGridGenerator::CreateMultiGrid(OUT MultiGrid& multiGrid, const MapGenerationSettings& settings)
{
	if (!IsConfigValid(settings))
		return nullptr;

	if (!settings.IsDungeon())
		return nullptr;

	return GenerateDungeon(multiGrid);
}

bool DungeonGridGenerator::IsConfigValid(const MapGenerationSettings& settings) const
{
	return settings.IsDungeon();
}


std::unique_ptr<AsciiMap> DungeonGridGenerator::GenerateDungeon(OUT MultiGrid& multiGrid)
{
	auto dungeonMap = std::make_unique<AsciiMap>();
	if (!GenerateDungeonMapInternal(*dungeonMap))
		return nullptr;

	ApplyDungeonMapToMultiGrid(multiGrid, *dungeonMap);
	return dungeonMap;
}

void DungeonGridGenerator::ApplyDungeonMapToMultiGrid(OUT MultiGrid& multiGrid, const AsciiMap& dungeonMap)
{
	const MmoSync::Map::Unified::MapTypeRegistry registry;

	for (int y = 0; y < dungeonMap.Height(); ++y)
	{
		for (int x = 0; x < dungeonMap.Width(); ++x)
		{
			wchar_t tile = dungeonMap.Get(x, y);
			GridCellBlockType blockType = registry.GetBlockTypeByAscii(tile);
			multiGrid.SetGridCellBlockType(static_cast<GridCol>(x), static_cast<GridRow>(y),
				blockType, blockType != GridCellBlockType::GROUND);
		}
	}

	BuildMultiGridLod(multiGrid);
}

bool DungeonGridGenerator::GenerateDungeonMapInternal(AsciiMap& out)
{
	// Initialize map with highest terrain level (walls)
	const auto& config = GetDungeonConfig();
	const int width = config.GetWidth();
	const int height = config.GetHeight();
	
	out.Init(width, height, LEVEL5);

	// Store dungeon configuration for later use
	out.SetConfig(config);

	DeterministicRandom32 rng(static_cast<uint32_t>(config.mRandomSeed));

	// Generate cave-like structure using cellular automata
	{
		// Initial random placement with border density control
		// Strategy: Reduce floor probability near edges for natural cave boundaries
		// Only reduce within 10% of edge to preserve dungeon size
		const int borderZone = std::max(1, std::max(width, height) / 10);  // 10% border zone (at least 1)
		
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				// Calculate distance to nearest edge
				int distX = std::min(x, width - x - 1);
				int distY = std::min(y, height - y - 1);
				int distanceFromEdge = std::min(distX, distY);
				
				// Adjust floor probability based on distance from edge
				int adjustedPercent = config.caveInitialFloorPercent;
				if (distanceFromEdge >= borderZone)
				{
					// Beyond border zone: full probability
					adjustedPercent = config.caveInitialFloorPercent;
				}
				else
				{
					// Within border zone: reduce probability smoothly using integer arithmetic
					// Minimum 40% of base probability, reaching 100% at borderZone
					const int ratioScaled = (distanceFromEdge * 1000) / borderZone; // 0 ~ 1000
					const int minProbScaled = 400; // 0.4 represented in thousandths
					const int adjustedRatioScaled = minProbScaled + ((1000 - minProbScaled) * ratioScaled + 500) / 1000;
					adjustedPercent = (config.caveInitialFloorPercent * adjustedRatioScaled + 500) / 1000;
				}
				
				adjustedPercent = std::clamp(adjustedPercent, 0, 100);
				if (rng(0, 100) < adjustedPercent)
					out.Set(x, y, LEVEL0);
			}
		}

		// Apply smoothing passes
		for (int pass = 0; pass < config.caveSmoothingPasses; ++pass)
		{
			AsciiMap temp = out;
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					int wallCount = out.CountNeighborsMatching(x, y, LEVEL5, 1, true);
					if (wallCount >= 6)
						temp.Set(x, y, LEVEL5);
					else
						temp.Set(x, y, LEVEL0);
				}
			}
			out = temp;
		}

		// Apply relaxed smoothing passes
		for (int pass = 0; pass < config.caveRelaxedPasses; ++pass)
		{
			AsciiMap temp = out;
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					int wallCount = out.CountNeighborsMatching(x, y, LEVEL5, 1, true);
					if (wallCount >= 5)
						temp.Set(x, y, LEVEL5);
					else
						temp.Set(x, y, LEVEL0);
				}
			}
			out = temp;
		}
	}

	// Apply border interpolation first
	ApplyBorderInterpolation(out);

	// Then ensure connectivity and center the AABB (this was moved after border interpolation)
	if (config.caveEnsureConnected && width <= 512 && height <= 512)
	{
		// Find the largest connected component
		// Find the largest connected region of floors using single-pass algorithm
		std::vector<std::pair<int, int>> largestRegion;
		const size_t total = static_cast<size_t>(width) * static_cast<size_t>(height);
		std::vector<uint8_t> visited(total, 0);

		auto toIndex = [&](int x, int y) -> size_t
			{
				return static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x);
			};

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				if (out.Get(x, y) == LEVEL0 && !visited[toIndex(x, y)])
				{
					std::vector<std::pair<int, int>> currentRegion;
					out.FloodFillWithVisited(x, y, LEVEL0, currentRegion, visited);
					if (currentRegion.size() > largestRegion.size())
						largestRegion = std::move(currentRegion);
				}
			}
		}

		// Calculate AABB of the largest region
		if (!largestRegion.empty())
		{
			int minX = largestRegion[0].first;
			int maxX = largestRegion[0].first;
			int minY = largestRegion[0].second;
			int maxY = largestRegion[0].second;
			
			for (const auto& [x, y] : largestRegion)
			{
				minX = std::min(minX, x);
				maxX = std::max(maxX, x);
				minY = std::min(minY, y);
				maxY = std::max(maxY, y);
			}
			
			// Calculate offset to center the AABB
			int regionWidth = maxX - minX + 1;
			int regionHeight = maxY - minY + 1;
			int offsetX = (width - regionWidth) / 2 - minX;
			int offsetY = (height - regionHeight) / 2 - minY;
			
			// Clear all floors
			for (int y = 0; y < height; ++y)
				for (int x = 0; x < width; ++x)
					if (out.Get(x, y) == LEVEL0)
						out.Set(x, y, LEVEL5);
			
			// Place largest region centered in the map
			for (const auto& [x, y] : largestRegion)
			{
				int newX = x + offsetX;
				int newY = y + offsetY;
				
				// Ensure new position is within bounds
				if (newX >= 0 && newX < width && newY >= 0 && newY < height)
					out.Set(newX, newY, LEVEL0);
			}
		}
	}

	// Place start and exit points, then generate distance-based terrain levels
	{
		// Collect all floor tiles first
		std::vector<std::pair<int, int>> floorTiles;
		const int borderZone = 8;
		for (int y = borderZone; y < height - borderZone; ++y)
		{
			for (int x = borderZone; x < width - borderZone; ++x)
			{
				if (out.Get(x, y) == LEVEL0)
					floorTiles.push_back({ x, y });
			}
		}

		if (floorTiles.empty())
			return true;  // No floors to place points

		// Place 3-5 START points, distributed across the dungeon
		std::vector<std::pair<int, int>> startPositions;
		const int numStarts = std::min(5, static_cast<int>(floorTiles.size()));
		
		// Use deterministic random to select START positions
		for (int i = 0; i < numStarts; ++i)
		{
			// rng(min, max) returns [min, max] inclusive, so subtract 1 for index range
			int index = rng(0, static_cast<int>(floorTiles.size()) - 1);
			startPositions.push_back(floorTiles[index]);
			out.Set(floorTiles[index].first, floorTiles[index].second, Special::START);
			// Remove to avoid duplicate
			floorTiles.erase(floorTiles.begin() + index);
		}

		// Find the farthest point from all START positions for EXIT
		// Use the first START position as reference for finding EXIT
		if (!startPositions.empty())
		{
			auto exitPos = FindFarthestPoint(out, startPositions[0], width, height);
			out.Set(exitPos.first, exitPos.second, Special::EXIT);

			// Generate distance-based terrain levels using closest START
			GenerateDistanceBasedTerrainLevels(out, startPositions, exitPos);
		}
	}

	return true;
}

void DungeonGridGenerator::GenerateDistanceBasedTerrainLevels(AsciiMap& out, const std::vector<std::pair<int, int>>& startPositions, const std::pair<int, int>& exitPos)
{
	const auto& config = GetDungeonConfig();
	const int width = config.GetWidth();
	const int height = config.GetHeight();

	// Simple hash function for position-based noise
	auto hashPosition = [](int x, int y) -> uint32_t {
		uint32_t h = static_cast<uint32_t>(x * 73856093) ^ static_cast<uint32_t>(y * 19349663);
		h = h * 1103515245 + 12345;
		return h;
	};

	// Generate terrain levels based on distance from closest start
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			wchar_t currentTile = out.Get(x, y);

			// Process all non-wall tiles (floors and special tiles)
			// Skip START and EXIT tiles
			if (currentTile == Special::START || currentTile == Special::EXIT)
				continue;

			if (currentTile != LEVEL5) // LEVEL5 is wall
			{
				// Find distance from closest START position
				int distanceFromStart = INT_MAX;
				for (const auto& startPos : startPositions)
				{
					int dist = std::abs(x - startPos.first) + std::abs(y - startPos.second);
					if (dist < distanceFromStart)
						distanceFromStart = dist;
				}
				
				// Calculate Manhattan distance from exit position
				int distanceFromExit = std::abs(x - exitPos.first) + std::abs(y - exitPos.second);

				// Calculate terrain level based on distance ratio with noise variation
				int terrainLevel;
				{
					// Calculate ratio: distanceFromStart / (distanceFromStart + distanceFromExit)
					// 출발지에 가까울수록 비율이 낮고, 목적지에 가까울수록 비율이 높음
					int totalDistance = distanceFromStart + distanceFromExit;
					if (totalDistance == 0)
					{
						terrainLevel = 0; // Avoid division by zero
					}
					else
					{
						// 출발지 거리 비율 계산 (0-100%)
						int startRatio = (distanceFromStart * 100) / totalDistance;

						// Generate position-based noise (-15 ~ +15 range) using integer arithmetic
						uint32_t hash = hashPosition(x, y);
						int noise = static_cast<int>(hash % 31) - 15;

						// Apply noise to startRatio (clamped to 0-100)
						int finalRatio = startRatio + noise;
						finalRatio = std::clamp(finalRatio, 0, 100);

						// Assign terrain levels based on adjusted start distance ratio
						// 출발지에 가까울수록 쉬운 레벨(LEVEL3), 목적지에 가까울수록 어려운 레벨(LEVEL0)
						if (finalRatio < 40)
							terrainLevel = 3;  // 0-40%: 가장 쉬운 레벨 (LEVEL3) - 출발지 근처
						else if (finalRatio < 65)
							terrainLevel = 2;  // 40-65%: 보통 레벨 (LEVEL2)
						else if (finalRatio < 85)
							terrainLevel = 1;  // 65-85%: 어려운 레벨 (LEVEL1)
						else
							terrainLevel = 0;  // 85-100%: 가장 어려운 레벨 (LEVEL0) - 목적지 근처
					}
				}

				// Set appropriate terrain level (border interpolation already applied earlier)
			switch (terrainLevel)
			{
			case 0: out.Set(x, y, LEVEL0); break;
			case 1: out.Set(x, y, LEVEL1); break;
			case 2: out.Set(x, y, LEVEL2); break;
			case 3: out.Set(x, y, LEVEL3); break;
			default: out.Set(x, y, LEVEL5); break;
			}
			}
		}
	}
}

void DungeonGridGenerator::ApplyBorderInterpolation(AsciiMap& out)
{
	const auto& config = GetDungeonConfig();
	const int width = config.GetWidth();
	const int height = config.GetHeight();

	// Apply border interpolation: convert floors to walls near edges
	// 8-layer transition from wall at edge to original floor level
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			wchar_t currentTile = out.Get(x, y);
			
			// Only process floor tiles (LEVEL0)
			if (currentTile == LEVEL0)
			{
				// Calculate distance to nearest border
				int distToLeft = x;
				int distToRight = width - x - 1;
				int distToTop = y;
				int distToBottom = height - y - 1;
				int distanceToBorder = std::min({ distToLeft, distToRight, distToTop, distToBottom });

				// Apply 8-layer border interpolation
				const int borderLayerThickness = 8;
				if (distanceToBorder < borderLayerThickness)
				{
					// Integer-based weighted average: wallWeight * LEVEL5
					const int numerator = (borderLayerThickness - distanceToBorder) * 5;
					int terrainLevel = (numerator + borderLayerThickness / 2) / borderLayerThickness;
					terrainLevel = std::clamp(terrainLevel, 0, 7);

				// Set the resulting level
				switch (terrainLevel)
				{
				case 0: out.Set(x, y, LEVEL0); break;
				case 1: out.Set(x, y, LEVEL1); break;
				case 2: out.Set(x, y, LEVEL2); break;
				case 3: out.Set(x, y, LEVEL3); break;
				default: out.Set(x, y, LEVEL5); break;
				}
				}
			}
		}
	}
}
