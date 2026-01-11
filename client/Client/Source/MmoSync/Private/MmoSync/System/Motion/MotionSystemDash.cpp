// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Motion/MotionSystemDash.h"

#include "MmoSync/DebugControlTool/DebugInfo.h"


MotionSystemDash::MotionSystemDash(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	DurationSystemBase<MotionSystemDash, MotionComponentDash>(engine, timeStepUpdateCycle, game)
{
}
void MotionSystemDash::OnMotionStarted(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{
	// 컴포넌트에 저장된 speedRate 사용 (PC Roll, Follower Charge, Skill Dash 등 컨텍스트별 속도 조절)
	MotionComponentDash& dash = GetComponent<MotionComponentDash>(entity.GetEntityIdp().first);
	SetSpeed(entity, dash.speedRate);
}

bool MotionSystemDash::OnMotionCompleted(Entity& entity, MAYBE_UNUSED CharacterComponent& character)
{	
	SetSpeed(entity, 0);

	// [FIX] Dash 종료 시 ACTION 레이어 클리어
	// Dash 중 방향 변경으로 설정된 ACTION 명령이 Dash 종료 후에도 남아
	// 해당 목표로 계속 이동하는 문제 방지
	PhysicsComponent* physics = TryGetComponent<PhysicsComponent>(entity);
	if (physics)
	{
		physics->ClearCommand(MoveCommandLayer::ACTION);

		// [FIX] PC Leader Roll 후 키 입력 중이면 바로 USER 이동 명령 설정
		// 시스템 실행 순서로 인해 PcSystem::Update가 먼저 실행되어
		// 이번 프레임에서 이동 명령이 없어 잠깐 멈추는 현상 방지
		if (HasComponent<LeaderComponent>(entity.GetEntityIdp()) && physics->IsKeyDown())
		{
			PositionComponent* position = TryGetComponent<PositionComponent>(entity);
			if (position)
			{
				// 바라보는 방향으로 이동 명령 설정
				const Angle angle = physics->GetAngle();
				const Scalar cosAngle = MathUtil::Cos(angle);
				const Scalar sinAngle = MathUtil::Sin(angle);
				const Scalar tx = position->x + ((cosAngle * DebugInfo::MOVE_TO_DISTANCE_BY_KEY) >> 10);
				const Scalar ty = position->y + ((sinAngle * DebugInfo::MOVE_TO_DISTANCE_BY_KEY) >> 10);
				physics->SetCommand(true, position->x, position->y, MoveCommandLayer::USER, tx, ty, DebugInfo::MOVE_LAYER_USER_MAX_LIFETIME, DebugInfo::MOVE_LAYER_USER_MIN_HOLD);
			}
		}
	}

	return true;
}
