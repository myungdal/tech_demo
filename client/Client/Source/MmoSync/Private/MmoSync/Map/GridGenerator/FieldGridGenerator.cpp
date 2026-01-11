// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Map/GridGeneration/FieldGridGenerator.h"
#include "MmoSync/Map/Types/MapGenerationSettings.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"
#include "MmoSync/Map/Visualization/MapRegistry.h"

#include "MmoSync/Grid/MultiGridDef.h"
#include "MmoSync/Util/Noise/NaturalNoise.h"
#include "MmoSync/Util/Noise/PerlinNoise.h"
#include "Common/Random/DeterministicRandom/DeterministicRandom32.h"

#include <algorithm>
#include <climits>
#include <map>
#include <queue>
#include <random>


FieldGridGenerator::FieldGridGenerator(const MapConfig& mapConfig)
	: GridGeneratorBase(), mMapConfig(mapConfig)
{
}
std::unique_ptr<AsciiMap> FieldGridGenerator::CreateMultiGrid(OUT MultiGrid& multiGrid, const MapGenerationSettings& settings)
{
	if (!IsConfigValid(settings))
		return nullptr;
	if (!settings.IsField())
		return nullptr;
	return GenerateField(multiGrid, settings);
}
bool FieldGridGenerator::IsConfigValid(const MapGenerationSettings& settings) const
{
	return settings.IsField();
}

std::unique_ptr<AsciiMap> FieldGridGenerator::GenerateField(OUT MultiGrid& multiGrid, const MapGenerationSettings& settings)
{
	auto fieldMap = std::make_unique<AsciiMap>();
	if (!GenerateFieldMapInternal(*fieldMap, settings))
	{
		_DEBUG_RED;
		return nullptr;
	}

	ApplyFieldMapToMultiGrid(multiGrid, *fieldMap);

	return fieldMap;
}

void FieldGridGenerator::ApplyFieldMapToMultiGrid(OUT MultiGrid& multiGrid, const AsciiMap& fieldMap)
{	
	const MmoSync::Map::Unified::MapTypeRegistry registry;

	for (int y = 0; y < fieldMap.Height(); ++y)
	{
		for (int x = 0; x < fieldMap.Width(); ++x)
		{
			wchar_t tile = fieldMap.Get(x, y);
			GridCol col = static_cast<GridCol>(x);
			GridRow row = static_cast<GridRow>(y);
			
			// ASCII 문자를 GridCellBlockType으로 변환
			GridCellBlockType blockType = registry.GetBlockTypeByAscii(tile);
			
			// MultiGrid에 적절한 블록 타입 설정
			if (blockType != GridCellBlockType::MAX) // 유효한 블록 타입인 경우
				multiGrid.SetGridCellBlockType(col, row, blockType, true);
		}
	}

	BuildMultiGridLod(multiGrid);
}

// Helper function to check if all floor tiles are connected
static bool AreAllFloorsConnected(const AsciiMap& asciiMap)
{
	const MmoSync::Map::Unified::MapTypeRegistry registry;
	int mapWidth = asciiMap.Width();
	int mapHeight = asciiMap.Height();
	
	std::vector<uint8_t> visited(static_cast<size_t>(mapWidth) * static_cast<size_t>(mapHeight), 0);
	
	auto toIndex = [&](int x, int y) -> size_t
	{
		return static_cast<size_t>(y) * static_cast<size_t>(mapWidth) + static_cast<size_t>(x);
	};
	
	auto isFloor = [&](wchar_t tile) -> bool
	{
		for (int level = 0; level <= 6; ++level)
		{
			if (tile == registry.GetAsciiByTerrainLevel(level))
				return true;
		}
		return false;
	};
	
	// Find first floor tile to start BFS
	std::pair<int, int> startPos = {-1, -1};
	for (int y = 0; y < mapHeight && startPos.first == -1; ++y)
	{
		for (int x = 0; x < mapWidth && startPos.first == -1; ++x)
		{
			if (isFloor(asciiMap.Get(x, y)))
			{
				startPos = {x, y};
			}
		}
	}
	
	if (startPos.first == -1)
		return false; // No floor tiles found
	
	// BFS to count reachable floor tiles
	int reachableCount = 0;
	std::queue<std::pair<int, int>> q;
	q.push(startPos);
	visited[toIndex(startPos.first, startPos.second)] = 1;
	
	int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();
		reachableCount++;
		
		for (int i = 0; i < 4; ++i)
		{
			int nx = x + dirs[i][0];
			int ny = y + dirs[i][1];
			
			if (nx < 0 || nx >= mapWidth || ny < 0 || ny >= mapHeight)
				continue;
			if (visited[toIndex(nx, ny)])
				continue;
			
			wchar_t neighbor = asciiMap.Get(nx, ny);
			if (isFloor(neighbor))
			{
				visited[toIndex(nx, ny)] = 1;
				q.push({nx, ny});
			}
		}
	}
	
	// Count total floor tiles
	int totalFloorCount = 0;
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			if (isFloor(asciiMap.Get(x, y)))
				totalFloorCount++;
		}
	}
	
	// All floor tiles must be reachable (allow 1% tolerance for floating point errors)
	return (reachableCount * 100 >= totalFloorCount * 99);
}

// Helper function to generate initial LevelDivisionList
static Noise::LevelDivisionList GenerateInitialLevelDivisionList()
{
	Noise::LevelDivisionList list;
	list.reserve(7);
	list.emplace_back(static_cast<uint8_t>(0));   // LEVEL0 - GROUND
	list.emplace_back(static_cast<uint8_t>(40));  // LEVEL1 - GROUND
	list.emplace_back(static_cast<uint8_t>(60));  // LEVEL2 - GROUND
	list.emplace_back(static_cast<uint8_t>(70));  // LEVEL3 - GROUND
	list.emplace_back(static_cast<uint8_t>(90));  // LEVEL4 - HILL
	list.emplace_back(static_cast<uint8_t>(110)); // LEVEL5 - WATER
	list.emplace_back(static_cast<uint8_t>(130)); // LEVEL6 - TUNNEL
	return list;
}

// Helper function to adjust LevelDivisionList to generate more floor tiles
static Noise::LevelDivisionList AdjustLevelDivisionList(const Noise::LevelDivisionList& original, int attempt)
{
	Noise::LevelDivisionList adjusted = original;
	
	// Lower the thresholds to generate more floor tiles (LEVEL0-3)
	// Each attempt reduces thresholds by approximately 5% (integer arithmetic)
	const int reductionPercent = std::clamp(attempt * 5, 0, 100);

	for (size_t i = 1; i < adjusted.size(); ++i)
	{
		int value = adjusted[i];
		int reduction = (value * reductionPercent + 50) / 100; // rounded reduction
		value -= reduction;
		value = std::clamp(value, 0, 255);
		adjusted[i] = static_cast<uint8_t>(value);
	}
	
	return adjusted;
}

bool FieldGridGenerator::GenerateFieldMapInternal(AsciiMap& out, const MapGenerationSettings& settings)
{
	const MmoSync::Map::Unified::MapTypeRegistry registry;

	const auto& fieldConfig = settings.mapConfig.GetFieldConfig();
	
	size_t mapWidth = fieldConfig.GetWidth();
	size_t mapHeight = fieldConfig.GetHeight();

	// Use deterministic random seed from config
	DeterministicRandom32 rng(static_cast<uint32_t>(settings.mapConfig.GetSeed()));

	// Initialize base LevelDivisionList
	Noise::LevelDivisionList baseLevelDivisionList = GenerateInitialLevelDivisionList();
	
	// Retry with adjusted LevelDivisionList until all floors are connected
	const int maxRetries = 10;
	for (int attempt = 0; attempt < maxRetries; ++attempt)
	{
		// Initialize field map
		out.Init(static_cast<int>(mapWidth), static_cast<int>(mapHeight), '.');
		out.SetConfig(fieldConfig);

		// Adjust LevelDivisionList to generate more floor tiles if needed
		Noise::LevelDivisionList levelDivisionList = (attempt == 0) 
			? baseLevelDivisionList 
			: AdjustLevelDivisionList(baseLevelDivisionList, attempt);

		// Generate fractalScale using rng (referencing TestMapGridGenerator::RunFieldDemo)
		// x는 7~12 범위, y는 x에서 ±3 범위 (7~12로 클램프), w는 9~12 범위
		Noise::FractalScaleList fractalScaleList;
		fractalScaleList.reserve(4);
		for (size_t i = 0; i < 4; ++i)
		{
			uint8_t x = static_cast<uint8_t>(rng(7, 12));
			int32_t yOffset = rng(-3, 3);
			int32_t y = static_cast<int32_t>(x) + yOffset;
			// y 범위 클램프 (7~12 범위로 제한)
			if (y < 7) y = 7;
			if (y > 12) y = 12;
			uint8_t w = static_cast<uint8_t>(rng(9, 12));
			
			fractalScaleList.emplace_back(Noise::FractalScale{
				x,
				static_cast<uint8_t>(y),
				w
			});
		}

		// NaturalNoise는 내부적으로 (mapWidth+1) x (mapHeight+1) 크기 테이블을 생성합니다.
		// 맵 셀 좌표(0~mapWidth-1)와 NaturalNoise 좌표(0~mapWidth-1)를 1:1로 매핑합니다.
		// 테두리 vertex(mapWidth, mapHeight)는 렌더러에서 처리합니다.
		Noise::NaturalNoise naturalNoise(
			static_cast<uint32_t>(mapWidth),
			static_cast<uint32_t>(mapHeight),
			std::move(levelDivisionList),
			std::move(fractalScaleList),
			fieldConfig.mRandomSeed
		);

		// 맵 셀 전체 처리: 맵 셀 좌표 = NaturalNoise 좌표 (1:1 매핑)
		for (uint32_t y = 0; y < static_cast<uint32_t>(mapHeight); ++y)
		{
			for (uint32_t x = 0; x < static_cast<uint32_t>(mapWidth); ++x)
			{
				const uint8_t level = naturalNoise.GetLevel(x, y);
				
				// Get appropriate ASCII character based on terrain level
				wchar_t asciiChar = registry.GetAsciiByTerrainLevel(static_cast<int>(level));
				out.Set(static_cast<int>(x), static_cast<int>(y), asciiChar);
			}
		}

		// Ensure field connectivity by removing small isolated regions
		EnsureFieldConnectivity(out);

		// Check if all floor tiles are connected
		if (AreAllFloorsConnected(out))
		{
			// Place START and EXIT points on lowest terrain level
			PlaceFieldStartAndExit(out, settings.mapConfig.GetFieldConfig());
			return true;
		}
		
		// Not all floors connected, retry with adjusted LevelDivisionList
		// Continue to next attempt
	}

	// maxRetries reached but still not connected - place START/EXIT anyway
	PlaceFieldStartAndExit(out, settings.mapConfig.GetFieldConfig());
	return true;
}

void FieldGridGenerator::PlaceFieldStartAndExit(AsciiMap& out, const FieldMapConfig& config)
{
	using namespace MmoSync::Map::Special;
	const MmoSync::Map::Unified::MapTypeRegistry registry;
	
	int mapWidth = out.Width();
	int mapHeight = out.Height();
	
	// 1. Find the main connected region (largest one)
	std::vector<std::pair<int, int>> mainRegion;
	const size_t total = static_cast<size_t>(mapWidth) * static_cast<size_t>(mapHeight);
	std::vector<uint8_t> visited(total, 0);
	
	auto toIndex = [&](int x, int y) -> size_t
	{
		return static_cast<size_t>(y) * static_cast<size_t>(mapWidth) + static_cast<size_t>(x);
	};
	
	// Find all connected regions to identify the main one
	std::vector<std::vector<std::pair<int, int>>> allRegions;
	
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			wchar_t tile = out.Get(x, y);
			bool isFloor = false;
			for (int level = 0; level <= 6; ++level)
			{
				if (tile == registry.GetAsciiByTerrainLevel(level))
				{
					isFloor = true;
					break;
				}
			}
			
			if (isFloor && !visited[toIndex(x, y)])
			{
				std::vector<std::pair<int, int>> currentRegion;
				std::queue<std::pair<int, int>> q;
				q.push({ x, y });
				visited[toIndex(x, y)] = 1;
				
				while (!q.empty())
				{
					auto [cx, cy] = q.front();
					q.pop();
					currentRegion.push_back({ cx, cy });
					
				int dirs[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
				for (int i = 0; i < 4; ++i)
				{
					int nx = cx + dirs[i][0];
					int ny = cy + dirs[i][1];
					
				if (nx < 0 || nx >= mapWidth || ny < 0 || ny >= mapHeight)
					continue;
				if (visited[toIndex(nx, ny)])
					continue;
				
				wchar_t neighbor = out.Get(nx, ny);
				bool isNeighborFloor = false;
				for (int level = 0; level <= 6; ++level)
				{
					if (neighbor == registry.GetAsciiByTerrainLevel(level))
					{
						isNeighborFloor = true;
						break;
					}
				}
				
				if (isNeighborFloor)
				{
					visited[toIndex(nx, ny)] = 1;
					q.push({ nx, ny });
				}
				}
			}
			
			allRegions.push_back(std::move(currentRegion));
		}
	}
}

if (allRegions.empty())
	return;  // No floor regions found
	
	// Find the main region (largest one)
	size_t maxSize = 0;
	int mainRegionIndex = -1;
	for (size_t i = 0; i < allRegions.size(); ++i)
	{
		if (allRegions[i].size() > maxSize)
		{
			maxSize = allRegions[i].size();
			mainRegionIndex = static_cast<int>(i);
		}
	}
	
	if (mainRegionIndex == -1)
		return;
	
	mainRegion = allRegions[mainRegionIndex];
	
	// 2. Find tiles with the lowest terrain level in the main region
	std::map<int, std::vector<std::pair<int, int>>> tilesByLevel;
	
	for (const auto& [x, y] : mainRegion)
	{
		wchar_t tile = out.Get(x, y);
		for (int level = 0; level <= 6; ++level)
		{
			if (tile == registry.GetAsciiByTerrainLevel(level))
			{
				tilesByLevel[level].push_back({ x, y });
				break;
			}
		}
	}
	
	if (tilesByLevel.empty())
		return;  // No terrain level tiles found in main region
	
	// Find the lowest level number that actually exists in main region
	int lowestLevel = tilesByLevel.begin()->first;
	
	// Get all tiles with the lowest level in the main region
	std::vector<std::pair<int, int>> candidatePoints = tilesByLevel[lowestLevel];
	
	if (candidatePoints.empty())
		return;  // No suitable tiles found
	
	// 3. Calculate minimum distance between points
	int minDistance = std::min(mapWidth, mapHeight) / 5;
	
	// 4. Select START points (3-5 points)
	std::vector<std::pair<int, int>> selectedPoints;
	const int numStarts = std::min(5, static_cast<int>(candidatePoints.size()));
	
	// Use deterministic random seed from config
	DeterministicRandom32 rng(static_cast<uint32_t>(config.mRandomSeed));
	
	for (int i = 0; i < numStarts; ++i)
	{
		bool found = false;
		const int maxAttempts = 100;  // Limit attempts to avoid infinite loop
		
		for (int attempt = 0; attempt < maxAttempts && !found; ++attempt)
		{
			int index = rng(0, static_cast<int>(candidatePoints.size()) - 1);
			std::pair<int, int> candidate = candidatePoints[index];
			
			// Check distance from all previously selected points
			bool tooClose = false;
			for (const auto& selected : selectedPoints)
			{
				int dx = candidate.first - selected.first;
				int dy = candidate.second - selected.second;
				int dist = std::abs(dx) + std::abs(dy);  // Manhattan distance
				
				if (dist < minDistance)
				{
					tooClose = true;
					break;
				}
			}
			
			if (!tooClose)
			{
				selectedPoints.push_back(candidate);
				out.Set(candidate.first, candidate.second, START);
				candidatePoints.erase(candidatePoints.begin() + index);
				found = true;
			}
		}
	}
	
	if (selectedPoints.empty())
		return;  // Could not place any START points
	
	// 5. Choose EXIT as the point closest to map center
	int centerX = mapWidth / 2;
	int centerY = mapHeight / 2;
	
	int minDistToCenter = INT_MAX;
	std::pair<int, int> exitPos = selectedPoints[0];
	
	for (const auto& pos : selectedPoints)
	{
		int dx = pos.first - centerX;
		int dy = pos.second - centerY;
		int distToCenter = std::abs(dx) + std::abs(dy);  // Manhattan distance
		
		if (distToCenter < minDistToCenter)
		{
			minDistToCenter = distToCenter;
			exitPos = pos;
		}
	}
	
	// Place EXIT (replacing START at that location)
	out.Set(exitPos.first, exitPos.second, EXIT);
}

void FieldGridGenerator::EnsureFieldConnectivity(AsciiMap& out)
{
	const MmoSync::Map::Unified::MapTypeRegistry registry;
	
	int mapWidth = out.Width();
	int mapHeight = out.Height();
	
	// Find all floor tiles (LEVEL0-6)
	std::vector<std::pair<int, int>> floorTiles;
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			wchar_t tile = out.Get(x, y);
			// Check if this is LEVEL0-6 (passable terrain)
			for (int level = 0; level <= 6; ++level)
			{
				if (tile == registry.GetAsciiByTerrainLevel(level))
				{
					floorTiles.push_back({ x, y });
					break;
				}
			}
		}
	}
	
	if (floorTiles.empty())
		return;  // No floor tiles to process
	
	// Find the main region (largest connected component)
	std::vector<std::pair<int, int>> mainRegion;
	const size_t total = static_cast<size_t>(mapWidth) * static_cast<size_t>(mapHeight);
	std::vector<uint8_t> visited(total, 0);
	
	auto toIndex = [&](int x, int y) -> size_t
	{
		return static_cast<size_t>(y) * static_cast<size_t>(mapWidth) + static_cast<size_t>(x);
	};
	
	// Flood fill to find all connected regions
	std::vector<std::vector<std::pair<int, int>>> allRegions;
	
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			wchar_t tile = out.Get(x, y);
			// Check if this is LEVEL0-6 and not visited
			bool isFloor = false;
			for (int level = 0; level <= 6; ++level)
			{
				if (tile == registry.GetAsciiByTerrainLevel(level))
				{
					isFloor = true;
					break;
				}
			}
			
			if (isFloor && !visited[toIndex(x, y)])
			{
				std::vector<std::pair<int, int>> currentRegion;
				
				// Flood fill to find all connected tiles
				std::queue<std::pair<int, int>> q;
				q.push({ x, y });
				visited[toIndex(x, y)] = 1;
				
				while (!q.empty())
				{
					auto [cx, cy] = q.front();
					q.pop();
					currentRegion.push_back({ cx, cy });
					
				// Check 4 neighbors
				int dirs[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
				for (int i = 0; i < 4; ++i)
				{
				int nx = cx + dirs[i][0];
				int ny = cy + dirs[i][1];
				
				if (nx < 0 || nx >= mapWidth || ny < 0 || ny >= mapHeight)
					continue;
				
				if (visited[toIndex(nx, ny)])
					continue;
				
				wchar_t neighbor = out.Get(nx, ny);
				bool isNeighborFloor = false;
				for (int level = 0; level <= 6; ++level)
				{
					if (neighbor == registry.GetAsciiByTerrainLevel(level))
					{
						isNeighborFloor = true;
						break;
					}
				}
				
				if (isNeighborFloor)
				{
					visited[toIndex(nx, ny)] = 1;
					q.push({ nx, ny });
				}
				}
			}
			
			allRegions.push_back(std::move(currentRegion));
		}
	}
}

if (allRegions.empty())
	return;
	
	// Find the main region (largest one)
	size_t maxSize = 0;
	int mainRegionIndex = -1;
	for (size_t i = 0; i < allRegions.size(); ++i)
	{
		if (allRegions[i].size() > maxSize)
		{
			maxSize = allRegions[i].size();
			mainRegionIndex = static_cast<int>(i);
		}
	}
	
	if (mainRegionIndex == -1)
		return;
	
	// Calculate minimum floor size threshold (1% of map area)
	const int minFloorSize = std::max(50, (mapWidth * mapHeight) / 100);
	
	// Remove small isolated regions, keep main region and reasonably sized regions
	for (size_t i = 0; i < allRegions.size(); ++i)
	{
		if (i == static_cast<size_t>(mainRegionIndex))
		{
			// Always keep main region
			mainRegion = allRegions[i];
		}
		else if (static_cast<int>(allRegions[i].size()) < minFloorSize)
		{
			// Remove small isolated regions by converting to tunnel
			for (const auto& [x, y] : allRegions[i])
			{
				out.Set(x, y, LEVEL6);  // Convert to tunnel (obstacle for most)
			}
		}
		// Else: keep reasonably sized regions as-is
	}
}

