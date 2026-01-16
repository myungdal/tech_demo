// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


struct FUiMsgBase
{
};

template<typename _Owner>
struct FUiMsg : FUiMsgBase
{
	using OwnerType = _Owner;
};
