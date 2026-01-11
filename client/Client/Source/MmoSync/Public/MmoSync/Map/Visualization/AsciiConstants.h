// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <wchar.h>
#include <cstdint> // For uint8_t

namespace MmoSync::Map
{
	// === 통합 지형 레벨 (Field & Dungeon 공용) ===
	// Level 0-3: 높은 난이도 (어려운 Nest 배치)
	// Level 4-7: 벽/장애물 (Nest 배치 불가)
	namespace TerrainLevel
	{
		constexpr wchar_t LEVEL0 = L' ';  // 가장 높은 난이도 (매우 어려운 Nest 배치)
		constexpr wchar_t LEVEL1 = L'.';  // 높은 난이도 (어려운 Nest 배치)
		constexpr wchar_t LEVEL2 = L',';  // 중간 난이도 (보통 Nest 배치)
		constexpr wchar_t LEVEL3 = L';';  // 낮은 난이도 (쉬운 Nest 배치)
		constexpr wchar_t LEVEL4 = L'$';  // HILL (Nest 배치 불가)
		constexpr wchar_t LEVEL5 = L'#';  // WALL (Nest 배치 불가)
		constexpr wchar_t LEVEL6 = L'@';  // WATER (Nest 배치 불가)
		constexpr wchar_t LEVEL7 = L'%';  // TUNNEL (Nest 배치 불가)
	}

	// === 특수 문자 ===
	namespace Special
	{
		constexpr wchar_t START = L'S';   // 시작점
		constexpr wchar_t EXIT = L'E';    // 출구
		constexpr wchar_t DOOR = L'+';    // 문
		constexpr wchar_t UNKNOWN = L'?'; // 알 수 없음
	}

	// === NPC 문자 ===
	namespace Npc
	{
		constexpr wchar_t CASTLE = L'C';  // 성
		constexpr wchar_t NEST0 = L'0';    // 둥지
		constexpr wchar_t NEST1 = L'1';    // 둥지
		constexpr wchar_t NEST2 = L'2';    // 둥지
		constexpr wchar_t NEST3 = L'3';    // 둥지
		constexpr wchar_t NEST4 = L'4';    // 둥지
	}


}
