// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Grid/GridSystemUnblockOnDead.h"

#include "MmoSync/Grid/MultiGrid.h"
#include "Packet/StaticData/Character/CharacterDoc.h"


GridSystemUnblockOnDead::GridSystemUnblockOnDead(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<PositionComponent>();
	IncludeComponent<CharacterComponent>();
	IncludeComponent<AbilityComponentGridUnblock>();
}
void GridSystemUnblockOnDead::Update()
{
	if (Data().empty())
		return;

	MultiGrid& grid = RefMultiGrid();

	for (EntityIdp entityIdp : Data())
	{
		Entity& entity = *GetEntity(entityIdp);

		if (entity.IsDestroyReserved())
		{
			PositionComponent& position = GetComponent<PositionComponent>(entityIdp.first);
			//CharacterComponent& character = GetComponent<CharacterComponent>(entityIdp.first);

			// 블록 상태로 되돌린다
			ColRowPair cell = grid.PickCell(position.x, position.y);
			grid.SetBlocked(cell.first, cell.second, GridCellBlockType::HILL);
		}
#if defined(UE_EDITOR) || defined(_DEBUG)
		else
		{
			AbilityComponentGridApplied* applied = TryGetComponent<AbilityComponentGridApplied>(entity);
			PositionComponent* position = TryGetComponent<PositionComponent>(entity);
			PhysicsComponent* physics = TryGetComponent<PhysicsComponent>(entity);
			//PcComponent* pc = TryGetComponent<PcComponent>(entity);

			_ASSERT_CRASH(applied);
			_ASSERT_CRASH(position);
			_ASSERT_CRASH(!physics);
			_ASSERT_CRASH(applied->isBlocked == true);
			_ASSERT_CRASH(applied->x == position->x);
			_ASSERT_CRASH(applied->y == position->y);
		}
#endif
	}
}
