// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MapRegistry.h"
#include "MapAnalyzer.h"
#include "MapRenderer.h"
#include <memory>

namespace MmoSync::Map::Unified
{
	// Forward declaration
	class UnifiedMapSystem;
	// Factory class for creating unified map system components
	class MapTypeFactory
	{
	public:
		// Registry creation
		static std::shared_ptr<MapTypeRegistry> CreateRegistry();

		// Analyzer creation
		static std::unique_ptr<UnifiedMapAnalyzer> CreateAnalyzer(std::shared_ptr<MapTypeRegistry> registry);
		static std::unique_ptr<UnifiedMapAnalyzer> CreateAnalyzer();

		// Renderer creation
		static std::unique_ptr<UnifiedMapRenderer> CreateRenderer(std::shared_ptr<MapTypeRegistry> registry);
		static std::unique_ptr<UnifiedMapRenderer> CreateRenderer();

		// Complete system creation
		static std::unique_ptr<UnifiedMapSystem> CreateCompleteSystem();

		static bool ValidateSystem(std::shared_ptr<MapTypeRegistry> registry);

	private:
		// Helper methods
		static void RegisterAllStandardMapTypes(std::shared_ptr<MapTypeRegistry> registry);
	};
}
