// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Grid/GridSystemUnblock.h"

#include "MmoSync/Grid/MultiGrid.h"
#include "Packet/StaticData/Character/CharacterDoc.h"


GridSystemUnblock::GridSystemUnblock(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<PositionComponent>();
	IncludeComponent<CharacterComponent>();
	IncludeComponent<AbilityComponentGridUnblock>();
	ExcludeComponent<AbilityComponentGridApplied>();
}
// 언블록 능력의 엔터티가 위치한 셀에 SetUnblocked 해준다.
void GridSystemUnblock::Update()
{
	if (Data().empty())
		return;

	MultiGrid& grid = RefMultiGrid();

	for (EntityIdp entityIdp : Data())
	{
		PositionComponent& position = GetComponent<PositionComponent>(entityIdp.first);
		//CharacterComponent& character = GetComponent<CharacterComponent>(entityIdp.first);
		Entity& entity = *GetEntity(entityIdp);

		if (entity.IsDestroyReserved())
			continue;

		ColRowPair cell = grid.PickCell(position.x, position.y);
		if (!grid.IsBlockedAny(cell.first, cell.second))
			continue;

		grid.SetUnblocked(cell.first, cell.second, GridCellBlockType::HILL);

		AttachComponent<AbilityComponentGridApplied>(entity);
		AbilityComponentGridApplied& c = AttachComponent<AbilityComponentGridApplied>(entity);
		c.isBlocked = false;
		c.x = position.x;
		c.y = position.y;
		c.c = cell.first;
		c.r = cell.second;
	}
}
