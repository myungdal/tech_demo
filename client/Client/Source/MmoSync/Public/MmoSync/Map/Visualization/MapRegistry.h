// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "AsciiDefinition.h"
#include <map>
#include <vector>
#include <string>

namespace MmoSync::Map::Unified
{
	// Registry for managing map ASCII definitions (unified, no MapKind separation)
	class MMOSYNC_API MapTypeRegistry
	{
	private:
		std::vector<MapAsciiDefinition> mAsciiDefinitions;
		std::wstring mLegend;
		std::map<wchar_t, MapAsciiDefinition> mAsciiLookup;

	public:
		MapTypeRegistry();
		~MapTypeRegistry() = default;

		// ASCII registration
		void RegisterAscii(wchar_t ascii, const std::wstring& name,
			AsciiCategory category, int terrainLevel = 0,
			GridCellBlockType blockType = GridCellBlockType::TUNNEL);

		// Legend management
		void SetLegend(const std::wstring& legend);
		std::wstring GetLegend() const;
		std::wstring GenerateLegend() const; // 동적 생성

		// ASCII lookup
		MapAsciiDefinition GetAsciiDefinition(wchar_t ascii) const;
		std::vector<MapAsciiDefinition> GetAsciiDefinitions() const;
		std::vector<MapAsciiDefinition> GetAsciiByCategory(AsciiCategory category) const;
		wchar_t GetAsciiByTerrainLevel(int level) const;

		// GridCellBlockType mapping
		GridCellBlockType GetBlockTypeByAscii(wchar_t ascii) const;

		// Validation
		bool IsAsciiValid(wchar_t ascii) const;
		AsciiCategory GetAsciiCategory(wchar_t ascii) const;

		// Unified ASCII registration
		void RegisterAllAscii();

		// Utility
		void Clear();

	private:
		void BuildAsciiLookup();
	};
}
