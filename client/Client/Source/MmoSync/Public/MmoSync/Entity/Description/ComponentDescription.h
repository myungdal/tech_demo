// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


struct ComponentBase;

struct ComponentDescription
{
	ComponentType mComponentType = ComponentType::NONE;
	std::shared_ptr<ComponentBase> mDescription = nullptr;

	explicit ComponentDescription(ComponentType componentType, std::shared_ptr<ComponentBase> description)
		:
		mComponentType(componentType),
		mDescription(description)
	{
	}
};

using EntityDescription = std::vector<ComponentDescription>;
using EntityDescriptionList = std::vector<EntityDescription>;
