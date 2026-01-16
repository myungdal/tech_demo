// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Motion/MotionSystemArrival.h"

#include "MmoSync/Component/AbilityComponent.h"
#include "MmoSync/Component/PhysicsComponent.h"
#include "MmoSync/Component/PositionComponent.h"
#include "MmoSync/Component/SpawneeComponent.h"


MotionSystemArrival::MotionSystemArrival(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	DurationSystemBase<MotionSystemArrival, MotionComponentArrival>(engine, timeStepUpdateCycle, game)
{
}
void MotionSystemArrival::OnMotionStarted(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	// Spawnee는 스폰 모션(Arrival) 동안 이동/속도 누적이 발생하지 않도록 "정지" 상태로 고정한다.
	// - DescriptCharacterBase에서 초기 TACTICS 이동 명령(homeX/homeY)이 미리 걸려있을 수 있는데,
	//   Arrival 동안 갱신/유지되다가 모션 종료 직후 급격히 실행되는 느낌이 날 수 있다.
	// - 따라서 Arrival 시작 시점에 속도/힘을 0으로 만들고, TACTICS 명령은 잠시 제거한다.
	if (!TryGetComponent<SpawneeComponent>(entity))
		return;

	PhysicsComponent* physics = TryGetComponent<PhysicsComponent>(entity);
	PositionComponent* position = TryGetComponent<PositionComponent>(entity);
	if (!physics || !position)
		return;

	physics->vx = 0;
	physics->vy = 0;
	physics->fx = 0;
	physics->fy = 0;
	physics->ex = 0;
	physics->ey = 0;

	// 모든 commandSlots 클리어 (다른 레이어에 RUNNING 명령이 남아있으면 일관성 문제 발생)
	physics->ClearAllCommands();
}

bool MotionSystemArrival::OnMotionCompleted(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{	
	DetachComponent<AbilityComponentUntargetrable>(entity);

	// Spawnee는 Arrival 종료 시점부터 이동을 시작하도록, homeX/homeY로 TACTICS 이동 명령을 복원한다.
	if (TryGetComponent<SpawneeComponent>(entity))
	{
		PhysicsComponent* physics = TryGetComponent<PhysicsComponent>(entity);
		PositionComponent* position = TryGetComponent<PositionComponent>(entity);
		if (physics && position)
		{
			// Arrival 동안 누적될 수 있는 물리 잔여값 정리
			physics->vx = 0;
			physics->vy = 0;
			physics->fx = 0;
			physics->fy = 0;
			physics->ex = 0;
			physics->ey = 0;

			// 모든 commandSlots 클리어 후 TACTICS만 새로 설정
			physics->ClearAllCommands();

			// 정상 속도로 설정 (Arrival 동안 속도가 변경되었을 수 있음)
			SetSpeed(entity, 0);

			// homeX/homeY로 이동 시작 (스폰 시점 초기 명령을 Arrival 이후로 지연 적용)
			physics->SetCommand(false, position->x, position->y, MoveCommandLayer::TACTICS,
				physics->homeX, physics->homeY,
				DebugInfo::MOVE_LAYER_TACTICS_MAX_LIFETIME, DebugInfo::MOVE_LAYER_TACTICS_MIN_HOLD);
		}
	}

	return true;
}
