// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


enum class AStarResult : uint32_t
{
	NOT_INITIALISED,
	SEARCHING,
	SUCCEEDED,
	FAILED,
	OUT_OF_MEMORY,
	INVALID
};
