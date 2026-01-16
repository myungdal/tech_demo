// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class STATIC_GAME;
struct MapDoc;

struct GameDoc
{
	const STATIC_GAME* mStaticGame = nullptr;
	const MapDoc* mMapDoc = nullptr;
};
