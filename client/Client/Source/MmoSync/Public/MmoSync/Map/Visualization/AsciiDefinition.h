// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/MultiGridDef.h"

namespace MmoSync::Map::Unified
{
	// ASCII categories for unified map system
	enum class AsciiCategory
	{
		TerrainLevel,   // Terrain levels (0-7) for both Field and Dungeon
		Npc,            // Castles, nests, NPCs, etc.
		Special         // Start, exit, doors, etc.
	};

	// Map ASCII definition structure
	struct MapAsciiDefinition
	{
		wchar_t ascii;              // Actual ASCII character ('#', '.', '?', '?')
		std::wstring name;          // Human-readable name ("Wall", "Floor", "Castle")
		AsciiCategory category;     // TERRAIN, ENTITY, SPECIAL
		int terrainLevel;           // Terrain level (0=lowest, 6=highest)
		GridCellBlockType blockType; // GridCellBlockType for MultiGrid integration

		MapAsciiDefinition() = default;

		MapAsciiDefinition(wchar_t ch, const std::wstring& n,
			AsciiCategory cat, int level = 0,
			GridCellBlockType blockType = GridCellBlockType::TUNNEL)
			: ascii(ch), name(n), category(cat),
			terrainLevel(level), blockType(blockType)
		{
		}
	};
}
