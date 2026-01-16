// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Physics/PhysicsSystemInit.h"


PhysicsSystemInit::PhysicsSystemInit(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<PhysicsComponent>();
}
void PhysicsSystemInit::Update()
{
	if (Data().empty())
		return;

	for (EntityIdp entityIdp : Data())
	{
		PhysicsComponent& physics = GetComponent<PhysicsComponent>(entityIdp.first);

		physics.ex = 0;
		physics.ey = 0;
	}
}
