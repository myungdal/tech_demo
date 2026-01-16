// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class STATIC_ITEM;
struct ItemStatDoc;

struct ItemDoc
{
	const STATIC_ITEM* mStaticItem = nullptr;
	const ItemStatDoc* mItemStatDoc = nullptr;
};
