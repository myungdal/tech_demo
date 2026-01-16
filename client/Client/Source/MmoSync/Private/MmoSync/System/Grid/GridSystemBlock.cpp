// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Grid/GridSystemBlock.h"

#include "Common/Math/MathUtil.h"

#include "MmoSync/Grid/MultiGrid.h"
#include "Packet/StaticData/Character/CharacterDoc.h"


GridSystemBlock::GridSystemBlock(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<PositionComponent>();
	IncludeComponent<CharacterComponent>();
	IncludeComponent<AbilityComponentGridBlock>();
	ExcludeComponent<AbilityComponentGridApplied>();
}
// 블록 능력의 엔터티가 위치한 셀에 SetBlocked 해준다.
void GridSystemBlock::Update()
{
	if (Data().empty())
		return;

	MultiGrid& grid = RefMultiGrid();

	for (EntityIdp entityIdp : Data())
	{
		PositionComponent& position = GetComponent<PositionComponent>(entityIdp.first);
		CharacterComponent& character = GetComponent<CharacterComponent>(entityIdp.first);
		Entity& entity = *GetEntity(entityIdp);
		
		if (entity.IsDestroyReserved())
			continue;

		const GridMoverType moverType = character.moverType;

		ColRowPair cell = grid.PickCell(position.x, position.y);
		if (grid.IsBlockedAny(cell.first, cell.second))
		{
			const GridCellDir firstDir = static_cast<GridCellDir>(GetRandom(0, 3));
			cell = grid.GetExitCell(cell.first, cell.second, firstDir, moverType);

			position.x = GridUtil::GetCenterX(cell.first);
			position.y = GridUtil::GetCenterY(cell.second);
		}

		grid.SetBlocked(cell.first, cell.second, GridCellBlockType::HILL);
		AbilityComponentGridApplied& c = AttachComponent<AbilityComponentGridApplied>(entity);
		c.isBlocked = true;
		c.x = position.x;
		c.y = position.y;
		c.c = cell.first;
		c.r = cell.second;
	}
}
