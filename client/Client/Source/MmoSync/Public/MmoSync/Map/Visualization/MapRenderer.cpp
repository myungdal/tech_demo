// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MapRenderer.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"
#include <format>
#include <algorithm>

using namespace MmoSync::Map::Special;

namespace MmoSync::Map::Unified
{
	UnifiedMapRenderer::UnifiedMapRenderer(std::shared_ptr<MapTypeRegistry> registry)
		: mRegistry(registry)
	{
	}


	std::wstring UnifiedMapRenderer::GenerateLegend() const
	{
		return mRegistry->GetLegend();
	}

	std::wstring UnifiedMapRenderer::GenerateMapHeader(int width, int height) const
	{
		return std::format(L"Map (Terrain + Entities)\nMap Size: {}x{}", width, height);
	}

	std::wstring UnifiedMapRenderer::GenerateBorder(int width) const
	{
		return L"+" + std::wstring(width, L'-') + L"+";
	}


	std::wstring UnifiedMapRenderer::GetEntityCharacter(const std::wstring& entityType) const
	{
		auto entities = mRegistry->GetAsciiByCategory(AsciiCategory::Npc);

		for (const auto& entity : entities)
		{
			if (entity.name == entityType)
			{
				return std::wstring(1, entity.ascii);
			}
		}

		return L"?"; // Default unknown entity
	}

	bool UnifiedMapRenderer::IsEntityPosition(const std::vector<MapEntity>& entities, int x, int y) const
	{
		return std::any_of(entities.begin(), entities.end(),
			[x, y](const MapEntity& entity) {
				return entity.x == x && entity.y == y;
			});
	}

	MapEntity UnifiedMapRenderer::GetEntityAtPosition(const std::vector<MapEntity>& entities, int x, int y) const
	{
		auto it = std::find_if(entities.begin(), entities.end(),
			[x, y](const MapEntity& entity) {
				return entity.x == x && entity.y == y;
			});

		if (it != entities.end())
			return *it;

		_DEBUG_RED;
		return MapEntity(x, y, L"Unknown", Special::UNKNOWN);
	}
}
