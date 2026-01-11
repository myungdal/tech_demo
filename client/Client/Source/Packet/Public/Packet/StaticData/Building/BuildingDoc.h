// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class STATIC_BUILDING;
class STATIC_BUILDING_LEVEL;

struct BuildingDoc
{
	const STATIC_BUILDING* mStaticBuilding = nullptr;
	std::vector<const STATIC_BUILDING_LEVEL*> mStaticBuildingLevelList;
};
