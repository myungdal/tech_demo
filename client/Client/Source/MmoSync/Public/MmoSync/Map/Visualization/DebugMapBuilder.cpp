// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DebugMapBuilder.h"
#include "DebugMapSystem.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"

namespace MmoSync::Map::Unified
{
	std::shared_ptr<MapTypeRegistry> MapTypeFactory::CreateRegistry()
	{
		auto registry = std::make_shared<MapTypeRegistry>();
		RegisterAllStandardMapTypes(registry);
		return registry;
	}


	std::unique_ptr<UnifiedMapAnalyzer> MapTypeFactory::CreateAnalyzer(std::shared_ptr<MapTypeRegistry> registry)
	{
		return std::make_unique<UnifiedMapAnalyzer>(registry);
	}

	std::unique_ptr<UnifiedMapAnalyzer> MapTypeFactory::CreateAnalyzer()
	{
		auto registry = CreateRegistry();
		return CreateAnalyzer(registry);
	}

	std::unique_ptr<UnifiedMapRenderer> MapTypeFactory::CreateRenderer(std::shared_ptr<MapTypeRegistry> registry)
	{
		return std::make_unique<UnifiedMapRenderer>(registry);
	}

	std::unique_ptr<UnifiedMapRenderer> MapTypeFactory::CreateRenderer()
	{
		auto registry = CreateRegistry();
		return CreateRenderer(registry);
	}

	std::unique_ptr<UnifiedMapSystem> MapTypeFactory::CreateCompleteSystem()
	{
		auto registry = CreateRegistry();
		return std::make_unique<UnifiedMapSystem>(std::move(registry));
	}

	bool MapTypeFactory::ValidateSystem(std::shared_ptr<MapTypeRegistry> registry)
	{
		if (!registry)
		{
			return false;
		}

		// Validate that ASCII definitions are registered
		auto asciiDefinitions = registry->GetAsciiDefinitions();

		return !asciiDefinitions.empty();
	}

	void MapTypeFactory::RegisterAllStandardMapTypes(std::shared_ptr<MapTypeRegistry> registry)
	{
		// 모든 ASCII를 한 번에 등록
		registry->RegisterAllAscii();
	}
}
