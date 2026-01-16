// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemBlink.h"

#include "Common/Stat/statContainer/StatContainer.h"
#include "MmoSync/Grid/MultiGrid.h"


SkillSystemBlink::SkillSystemBlink(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<SkillComponentBlink>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}
void SkillSystemBlink::Update()
{
	UpdateSkillRandomTarget<SkillComponentBlink, MotionComponentSkillA>();
}

void SkillSystemBlink::OnGiveDamage(
	MAYBE_UNUSED Entity& e0,
	MAYBE_UNUSED PositionComponent& p0,
	MAYBE_UNUSED Entity& e1,
	MAYBE_UNUSED PositionComponent& p1,
	MAYBE_UNUSED DamageInfo& damageInfo
)
{
	damageInfo.stun = true;

	CharacterComponent* character = TryGetComponent<CharacterComponent>(e0.GetEntityIdp());
	if (!character)
		return;

	const GridMoverType moverType = character->moverType;

	MultiGrid& grid = RefMultiGrid();

	const Scalar dx = (p1.x - p0.x);
	const Scalar dy = (p1.y - p0.y);
	auto [nx, ny] = MathUtil::GetVec1024(dx, dy);
	if (dx == 0 && dy == 0)
	{
		nx = 1024;
		ny = 0;
	}

	Scalar x1 = p1.x - ((nx * CELL_SIZE) >> 10);
	Scalar y1 = p1.y - ((ny * CELL_SIZE) >> 10);

	ColRowPair targetCell = grid.PickCell(x1, y1);
	if (grid.IsBlocked(targetCell.first, targetCell.second, moverType))
	{
		const GridCellDir firstDir = static_cast<GridCellDir>(GetRandom(0, 3));
		ColRowPair exitCell = grid.GetExitCell(targetCell.first, targetCell.second, firstDir, moverType);
		x1 = GridUtil::GetCenterX(exitCell.first);
		y1 = GridUtil::GetCenterY(exitCell.second);
	}

	PositionComponent& pos0 = p0;
	pos0.x = x1;
	pos0.y = y1;

	character->clientCharacterState |= CLIENT_CHARACTER_STATE_SMOOTH_RESET;

}
