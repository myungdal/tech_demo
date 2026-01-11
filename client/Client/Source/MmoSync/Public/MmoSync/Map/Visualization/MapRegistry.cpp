// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MapRegistry.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"
#include "MmoSync/Grid/MultiGridDef.h"
#include <algorithm>
#include <stdexcept>

namespace MmoSync::Map::Unified
{
	MapTypeRegistry::MapTypeRegistry()
	{
		RegisterAllAscii();
		BuildAsciiLookup();
		mLegend = GenerateLegend();
	}

	void MapTypeRegistry::RegisterAscii(wchar_t ascii, const std::wstring& name,
		AsciiCategory category, int terrainLevel,
		GridCellBlockType blockType)
	{
		MapAsciiDefinition definition(ascii, name, category, terrainLevel,
			blockType);
		mAsciiDefinitions.push_back(definition);
		mAsciiLookup[definition.ascii] = definition;
	}

	void MapTypeRegistry::SetLegend(const std::wstring& legend)
	{
		mLegend = legend;
	}

	std::wstring MapTypeRegistry::GetLegend() const
	{
		return mLegend;
	}

	std::wstring MapTypeRegistry::GenerateLegend() const
	{
		if (mAsciiDefinitions.empty())
			return L"";

		// 카테고리별, 레벨별로 정렬
		auto sortedAscii = mAsciiDefinitions;
		std::sort(sortedAscii.begin(), sortedAscii.end(),
			[](const MapAsciiDefinition& a, const MapAsciiDefinition& b) {
				if (a.category != b.category) {
					return static_cast<int>(a.category) < static_cast<int>(b.category);
				}
				return a.terrainLevel < b.terrainLevel;
			});

		std::wstring legend;
		for (size_t i = 0; i < sortedAscii.size(); ++i) {
			if (i > 0) legend += L", ";
			legend += std::wstring(1, sortedAscii[i].ascii) + L" = " + sortedAscii[i].name;
		}

		return legend;
	}

	MapAsciiDefinition MapTypeRegistry::GetAsciiDefinition(wchar_t ascii) const
	{
		auto it = mAsciiLookup.find(ascii);
		if (it != mAsciiLookup.end()) {
			return it->second;
		}

		// Return default unknown definition
		return MapAsciiDefinition(ascii, L"Unknown", AsciiCategory::TerrainLevel, 0);
	}

	std::vector<MapAsciiDefinition> MapTypeRegistry::GetAsciiDefinitions() const
	{
		return mAsciiDefinitions;
	}

	std::vector<MapAsciiDefinition> MapTypeRegistry::GetAsciiByCategory(AsciiCategory category) const
	{
		std::vector<MapAsciiDefinition> result;
		for (const auto& def : mAsciiDefinitions) {
			if (def.category == category) {
				result.push_back(def);
			}
		}
		return result;
	}

	wchar_t MapTypeRegistry::GetAsciiByTerrainLevel(int level) const
	{
		// Find the ASCII character that matches the given terrain level (unified terrain levels)
		for (const auto& def : mAsciiDefinitions) {
			if (def.category == AsciiCategory::TerrainLevel && def.terrainLevel == level) {
				return def.ascii;
			}
		}

		return L' ';
	}

	bool MapTypeRegistry::IsAsciiValid(wchar_t ascii) const
	{
		return mAsciiLookup.find(ascii) != mAsciiLookup.end();
	}

	AsciiCategory MapTypeRegistry::GetAsciiCategory(wchar_t ascii) const
	{
		auto it = mAsciiLookup.find(ascii);
		return it != mAsciiLookup.end() ? it->second.category : AsciiCategory::TerrainLevel;
	}

	GridCellBlockType MapTypeRegistry::GetBlockTypeByAscii(wchar_t ascii) const
	{
		auto it = mAsciiLookup.find(ascii);
		return it != mAsciiLookup.end() ? it->second.blockType : GridCellBlockType::TUNNEL;
	}

	void MapTypeRegistry::RegisterAllAscii()
	{
		using namespace MmoSync::Map::TerrainLevel;
		using namespace MmoSync::Map::Special;
		using namespace MmoSync::Map::Npc;

		// === 통합 지형 레벨 (Field & Dungeon 공용) ===
		// Level 0-3: 낮은 난이도 (Nest 배치 적합)
		RegisterAscii(LEVEL0, L"0", AsciiCategory::TerrainLevel, 0, GridCellBlockType::GROUND);
		RegisterAscii(LEVEL1, L"1", AsciiCategory::TerrainLevel, 1, GridCellBlockType::GROUND);
		RegisterAscii(LEVEL2, L"2", AsciiCategory::TerrainLevel, 2, GridCellBlockType::GROUND);
		RegisterAscii(LEVEL3, L"3", AsciiCategory::TerrainLevel, 3, GridCellBlockType::GROUND);

		// Level 4-7: 특수 지형
		RegisterAscii(LEVEL4, L"4", AsciiCategory::TerrainLevel, 4, GridCellBlockType::HILL);
		RegisterAscii(LEVEL5, L"5", AsciiCategory::TerrainLevel, 5, GridCellBlockType::WATER);
		RegisterAscii(LEVEL6, L"6", AsciiCategory::TerrainLevel, 6, GridCellBlockType::TUNNEL);
		RegisterAscii(LEVEL7, L"7", AsciiCategory::TerrainLevel, 7, GridCellBlockType::WALL);

		// === 특수 ASCII ===
		RegisterAscii(START, L"Start", AsciiCategory::Special, 0, GridCellBlockType::GROUND);
		RegisterAscii(EXIT, L"Exit", AsciiCategory::Special, 0, GridCellBlockType::GROUND);
		RegisterAscii(DOOR, L"Door", AsciiCategory::Special, 0, GridCellBlockType::GROUND);

		// === NPC ASCII ===
		RegisterAscii(CASTLE, L"Castle", AsciiCategory::Npc, 0, GridCellBlockType::GROUND);
		RegisterAscii(NEST0, L"Nest0", AsciiCategory::Npc, 0, GridCellBlockType::GROUND);
		RegisterAscii(NEST1, L"Nest1", AsciiCategory::Npc, 1, GridCellBlockType::GROUND);
		RegisterAscii(NEST2, L"Nest2", AsciiCategory::Npc, 2, GridCellBlockType::GROUND);
		RegisterAscii(NEST3, L"Nest3", AsciiCategory::Npc, 3, GridCellBlockType::GROUND);
		RegisterAscii(NEST4, L"Nest4", AsciiCategory::Npc, 4, GridCellBlockType::GROUND);
	}

	void MapTypeRegistry::Clear()
	{
		mAsciiDefinitions.clear();
		mAsciiLookup.clear();
		mLegend.clear();
	}

	void MapTypeRegistry::BuildAsciiLookup()
	{
		mAsciiLookup.clear();
		for (const auto& def : mAsciiDefinitions) {
			mAsciiLookup[def.ascii] = def;
		}
	}
}
