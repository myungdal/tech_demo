// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class StaticDataCatalog;

class StaticDbLoader
{
public:
	static bool Load(SQLHENV env, OUT StaticDataCatalog& staticDataCatalog);
};
