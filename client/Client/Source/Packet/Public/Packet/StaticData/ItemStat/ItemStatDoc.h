// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class STATIC_ITEM_STAT;
class StatContainer;

struct ItemStatDoc
{
	std::vector<const STATIC_ITEM_STAT*> mStaticItemStatList;
	std::shared_ptr<StatContainer> mStatContainer = nullptr;
};
