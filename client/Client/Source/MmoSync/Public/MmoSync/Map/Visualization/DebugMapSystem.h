// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "DebugMapBuilder.h"
#include "MapStats.h"
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace MmoSync::Map::Unified
{
	// Main unified map system class that combines all components
	class UnifiedMapSystem
	{
	private:
		std::shared_ptr<MapTypeRegistry> mRegistry;
		std::unique_ptr<UnifiedMapAnalyzer> mAnalyzer;
		std::unique_ptr<UnifiedMapRenderer> mRenderer;

	public:
		// Constructor
		explicit UnifiedMapSystem(std::shared_ptr<MapTypeRegistry> registry);
		~UnifiedMapSystem() = default;

		// Map analysis
		template<typename MapType>
		std::map<std::wstring, int> AnalyzeEntities(const MapType& map);

		// Map rendering
		template<typename MapType>
		std::wstring RenderMap(const MapType& map,
			const std::vector<MapEntity>& entities = {});

		template<typename MapType>
		std::wstring RenderMapWithHeader(const MapType& map,
			const std::vector<MapEntity>& entities = {});

		// File operations
		template<typename MapType>
		bool SaveMapToFile(const MapType& map,
			const std::wstring& filename,
			const std::vector<MapEntity>& entities = {});

		// Utility methods
		std::wstring GetLegend() const;

		// Component access
		std::shared_ptr<MapTypeRegistry> GetRegistry() const { return mRegistry; }
		UnifiedMapAnalyzer* GetAnalyzer() const { return mAnalyzer.get(); }
		UnifiedMapRenderer* GetRenderer() const { return mRenderer.get(); }

		// System validation
		bool IsValid() const;

	private:
		void InitializeComponents();
	};

	// Template implementations (must be in header)
	template<typename MapType>
	inline std::map<std::wstring, int> UnifiedMapSystem::AnalyzeEntities(const MapType& map)
	{
		return mAnalyzer->AnalyzeEntities(map);
	}

	template<typename MapType>
	inline std::wstring UnifiedMapSystem::RenderMap(const MapType& map,
		const std::vector<MapEntity>& entities)
	{
		return mRenderer->RenderMap(map, entities);
	}

	template<typename MapType>
	inline std::wstring UnifiedMapSystem::RenderMapWithHeader(const MapType& map,
		const std::vector<MapEntity>& entities)
	{
		return mRenderer->RenderMapWithHeader(map, entities);
	}

	template<typename MapType>
	inline bool UnifiedMapSystem::SaveMapToFile(const MapType& map,
		const std::wstring& filename,
		const std::vector<MapEntity>& entities)
	{
		try
		{
			std::wofstream file(filename);
			if (!file.is_open())
			{
				return false;
			}

			std::wstring content = RenderMapWithHeader(map, entities);
			file << content;
			file.close();

			return true;
		}
		catch (...)
		{
			return false;
		}
	}
}
