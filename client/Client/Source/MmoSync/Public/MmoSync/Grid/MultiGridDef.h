// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <bitset>
#include <array>
#include "Common/PCH/pch_world.h"
#include "Common/Math/MathUtil.h"


// GridCollisionMap 타입 정의
using GridCollisionMap = std::bitset<MathUtil::AlignBitCountTo64(MAX_GRID_COL * MAX_GRID_ROW)>;

// GridCollisionBit 비트플래그 연산자 오버로드
constexpr GridCollisionBit operator|(GridCollisionBit a, GridCollisionBit b) { return static_cast<GridCollisionBit>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b)); }
constexpr GridCollisionBit operator&(GridCollisionBit a, GridCollisionBit b) { return static_cast<GridCollisionBit>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b)); }
constexpr GridCollisionBit operator~(GridCollisionBit a) { return static_cast<GridCollisionBit>(~static_cast<uint8_t>(a)); }
constexpr bool operator!(GridCollisionBit a) { return static_cast<uint8_t>(a) == 0; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 구현 의도:
// 
// 1. GridMoverType별 능력 정의 (gMoverTypeToCollisionBit)
//    - 각 캐릭터/몬스터 타입이 "가지고 있는 이동 능력"을 정의
//    - 예: FLYER는 모든 능력을 가짐 (날 수 있으니 모든 지형 통과 가능)
//    - 예: WALKER는 걷기만 가능
// 
// 2. GridCellBlockType별 필요 능력 정의 (gGridCellBlockCollisions)
//    - 각 지형을 "통과하는데 필요한 능력"을 정의
//    - 예: WATER 지형은 CAN_SWIM 능력이 필요
//    - 예: WALL은 NONE (아무도 통과 불가)
// 
// 3. 충돌 판정 로직:
//    - 능력이 있으면 (moverAbilities & requiredAbility != 0) → 통과 가능 (언블록)
//    - 능력이 없으면 (moverAbilities & requiredAbility == 0) → 통과 불가 (블록)
//    - WALL (requiredAbility == NONE)은 모든 타입 블록
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GridMoverType별로 가지고 있는 이동 능력 매핑
constexpr std::array<size_t, static_cast<size_t>(GridMoverType::MAX)> gMoverTypeToCollisionBit = {
	(size_t)GridCollisionBit::CAN_WALK,																// WALKER: 걷기만 가능
	(size_t)GridCollisionBit::CAN_WALK | (size_t)GridCollisionBit::CAN_FLY,									// FLYER: 걷기+비행 (날 수 있어서 언덕 통과)
	(size_t)GridCollisionBit::CAN_WALK | (size_t)GridCollisionBit::CAN_FLY | (size_t)GridCollisionBit::CAN_SWIM,	// SWIMMER: 걷기+수영
	(size_t)GridCollisionBit::CAN_WALK | (size_t)GridCollisionBit::CAN_FLY | (size_t)GridCollisionBit::CAN_BURROW,	// UNDERWALKER: 걷기+땅굴
	(size_t)GridCollisionBit::CAN_SWIM,																// ONLY_SWIMMER: 수영만 가능
	(size_t)GridCollisionBit::CAN_BURROW,															// ONLY_UNDERWALKER: 땅굴만 가능
};

// GridCellBlockType별로 통과하는데 필요한 능력 매핑
constexpr std::array<size_t, static_cast<size_t>(GridCellBlockType::MAX)> gGridCellBlockCollisions = {
	(size_t)GridCollisionBit::CAN_WALK,   // GROUND: 평지 - 걷기 능력 필요
	(size_t)GridCollisionBit::CAN_FLY,    // HILL: 언덕 - 비행 능력 필요
	(size_t)GridCollisionBit::CAN_SWIM,   // WATER: 물 - 수영 능력 필요
	(size_t)GridCollisionBit::CAN_BURROW, // TUNNEL: 터널 - 땅굴 능력 필요
	(size_t)GridCollisionBit::NONE        // WALL: 벽 - 아무도 통과 불가
};
