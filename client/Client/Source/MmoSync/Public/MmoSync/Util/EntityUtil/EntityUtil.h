// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Entity/DescriptorBase/EntityDescriptorArg.h"
#include "MmoSync/Entity/Description/EntityDescription.h"


namespace EntityUtil
{
	MMOSYNC_API EntityDescription DescriptSpawner(const EntityDescriptorArg& arg);
	MMOSYNC_API EntityDescription DescriptSpawnee(const EntityDescriptorArg& arg);
	MMOSYNC_API EntityDescription DescriptEntityPcNpc(const EntityDescriptorArg& arg);
	MMOSYNC_API EntityDescription DescriptEntityNormal(const EntityDescriptorArg& arg);

	template <typename _Component>
	std::shared_ptr<_Component> TakeComponent(EntityDescription& entityDescription)
	{
		const ComponentType componentType = _Component::GetComponentType();
		auto it = std::find_if(entityDescription.begin(), entityDescription.end(),
			[&componentType](ComponentDescription& cd) {
				return (componentType == cd.mComponentType);
			});
		if (it != entityDescription.end())
			return std::static_pointer_cast<_Component>(it->mDescription);

		std::shared_ptr<_Component> component = std::make_shared<_Component>();
		entityDescription.emplace_back(_Component::GetComponentType(), component);

		return component;
	}
}
