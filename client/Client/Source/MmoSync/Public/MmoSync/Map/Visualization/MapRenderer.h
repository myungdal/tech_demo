// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MapRegistry.h"
#include "MmoSync/Map/Types/AsciiMap.h"
#include <memory>
#include <vector>
#include <string>

namespace MmoSync::Map::Unified
{

	// Entity structure for rendering
	struct MapEntity
	{
		int x, y;
		std::wstring type;
		wchar_t character;

		MapEntity(int posX, int posY, const std::wstring& entityType, wchar_t ch)
			: x(posX), y(posY), type(entityType), character(ch)
		{
		}
	};

	// Unified map renderer for all map types
	class UnifiedMapRenderer
	{
	private:
		std::shared_ptr<MapTypeRegistry> mRegistry;

	public:
		explicit UnifiedMapRenderer(std::shared_ptr<MapTypeRegistry> registry);
		~UnifiedMapRenderer() = default;

		// Map rendering
		template<typename MapType>
		std::wstring RenderMap(const MapType& map,
			const std::vector<MapEntity>& entities = {});

		// Map rendering with header
		template<typename MapType>
		std::wstring RenderMapWithHeader(const MapType& map,
			const std::vector<MapEntity>& entities = {});

		// Legend generation
		std::wstring GenerateLegend() const;

		// Map header generation
		std::wstring GenerateMapHeader(int width, int height) const;

		// Border generation
		std::wstring GenerateBorder(int width) const;

		// Entity overlay rendering
		template<typename MapType>
		std::wstring RenderEntityOverlay(const MapType& map,
			const std::vector<MapEntity>& entities);

		// Registry access
		std::shared_ptr<MapTypeRegistry> GetRegistry() const { return mRegistry; }

	private:
		// Helper methods
		template<typename MapType>
		std::wstring RenderMapInternal(const MapType& map,
			const std::vector<MapEntity>& entities);

		std::wstring GetEntityCharacter(const std::wstring& entityType) const;
		bool IsEntityPosition(const std::vector<MapEntity>& entities, int x, int y) const;
		MapEntity GetEntityAtPosition(const std::vector<MapEntity>& entities, int x, int y) const;
	};

	// Template implementations (must be in header)
	template<typename MapType>
	inline std::wstring UnifiedMapRenderer::RenderMap(const MapType& map,
		const std::vector<MapEntity>& entities)
	{
		return RenderMapInternal(map, entities);
	}

	template<typename MapType>
	inline std::wstring UnifiedMapRenderer::RenderMapWithHeader(const MapType& map,
		const std::vector<MapEntity>& entities)
	{
		std::wstring result;

		// Add header
		result += GenerateMapHeader(map.Width(), map.Height()) + L"\n";

		// Add legend
		result += GenerateLegend() + L"\n";

		// Add border
		result += GenerateBorder(map.Width()) + L"\n";

		// Add map content
		result += RenderMapInternal(map, entities);

		// Add closing border
		result += GenerateBorder(map.Width()) + L"\n";

		return result;
	}

	template<typename MapType>
	inline std::wstring UnifiedMapRenderer::RenderEntityOverlay(const MapType& map,
		const std::vector<MapEntity>& entities)
	{
		std::wstring result;

		for (int y = 0; y < map.Height(); ++y)
		{
			result += L"|";
			for (int x = 0; x < map.Width(); ++x)
			{
				if (IsEntityPosition(entities, x, y))
				{
					auto entity = GetEntityAtPosition(entities, x, y);
					result += entity.character;
				}
				else
				{
					result += map.Get(x, y);
				}
			}
			result += L"|\n";
		}

		return result;
	}

	template<typename MapType>
	inline std::wstring UnifiedMapRenderer::RenderMapInternal(const MapType& map,
		const std::vector<MapEntity>& entities)
	{
		std::wstring result;

		for (int y = 0; y < map.Height(); ++y)
		{
			result += L"|";
			for (int x = 0; x < map.Width(); ++x)
			{
				if (IsEntityPosition(entities, x, y))
				{
					auto entity = GetEntityAtPosition(entities, x, y);
					result += entity.character;
				}
				else
				{
					result += map.Get(x, y);
				}
			}
			result += L"|\n";
		}

		return result;
	}
}
