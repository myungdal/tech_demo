// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemDash.h"

#include "Common/Stat/statContainer/StatContainer.h"


SkillSystemDash::SkillSystemDash(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<SkillComponentDash>();
	IncludeComponent<PhysicsComponent>();
	IncludeComponent<TacticsComponentCombat>();
	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}
void SkillSystemDash::Update()
{
	if (Data().empty())
		return;

	const TimeStepUpdateCycle timeDelta = GetTimeStepUpdateCycle();

	for (EntityIdp entityIdp : Data())
	{
		MAYBE_UNUSED PositionComponent& p0 = GetComponent<PositionComponent>(entityIdp.first);
		MAYBE_UNUSED CharacterComponent& c0 = GetComponent<CharacterComponent>(entityIdp.first);
		SkillComponentDash& s0 = GetComponent<SkillComponentDash>(entityIdp.first);
		PhysicsComponent& physics0 = GetComponent<PhysicsComponent>(entityIdp.first);

		if (StatContainer* statContainer0 = c0.GetStatContainer())
		{
			const StatValue currSp = statContainer0->GetStatValue(StatType::SP);
			const StatValue maxSp = statContainer0->GetStatValue(StatType::SP_MAX);
			const StatValue increaseSp = (StatValue)std::min((maxSp - currSp), (int)timeDelta);

			if (increaseSp > 0)
				statContainer0->IncreaseStatValue(StatType::SP, increaseSp);

			STATIC_CHARACTER_SKILL& staticCharacterSkill = s0.GetStaticCharacterSkill();
			const StatValue costSp = (staticCharacterSkill.Get_c_skill_cost_sp() >> TIME_STEP_INTERVAL_FACTOR);
			const StatValue newSp = statContainer0->GetStatValue(StatType::SP);

			if (newSp < costSp)
				continue;

			Entity* e1 = GetEntity(c0.skillTarget);
			if (!e1)
				continue;

			if (e1->HasComponent(AbilityComponentUntargetrable::GetComponentType()))
				e1 = nullptr;
			else if (e1->HasComponent(MotionComponentDying::GetComponentType()))
				e1 = nullptr;
			else if (e1->HasComponent(MotionComponentDead::GetComponentType()))
				e1 = nullptr;

			if (!e1)
			{
				c0.skillTarget = INVALID_ENTITY_IDP;
				continue;
			}

			CharacterComponent* c1 = TryGetComponent<CharacterComponent>(*e1);
			if (nullptr == c1)
				continue;

		PositionComponent* p1 = TryGetComponent<PositionComponent>(*e1);
		if (nullptr == p1)
			continue;

		// 키보드 입력 중일 때: 입력 방향과 타겟 방향이 일치하는지 체크 (내적 사용)
		if (physics0.IsKeyDown())
		{
			// 키 입력에 따른 방향 벡터 결정
			Scalar keyDirX = 0;
			Scalar keyDirY = 0;
			bool hasDirectionInput = false;

			const bool w = physics0.IsKeyDownW();
			const bool a = physics0.IsKeyDownA();
			const bool s = physics0.IsKeyDownS();
			const bool d = physics0.IsKeyDownD();

			// 8방향 벡터 계산 (정규화된 1024 단위)
			if (w && !s)
			{
				if (a && !d)
				{
					keyDirX = -724; keyDirY = -724; // 위+왼쪽 (1024/√2 ≈ 724)
				}
				else if (d && !a)
				{
					keyDirX = 724; keyDirY = -724;  // 위+오른쪽
				}
				else
				{
					keyDirX = 0; keyDirY = -1024;   // 위
				}
				hasDirectionInput = true;
			}
			else if (s && !w)
			{
				if (a && !d)
				{
					keyDirX = -724; keyDirY = 724;  // 아래+왼쪽
				}
				else if (d && !a)
				{
					keyDirX = 724; keyDirY = 724;   // 아래+오른쪽
				}
				else
				{
					keyDirX = 0; keyDirY = 1024;    // 아래
				}
				hasDirectionInput = true;
			}
			else if (a && !d)
			{
				keyDirX = -1024; keyDirY = 0;       // 왼쪽
				hasDirectionInput = true;
			}
			else if (d && !a)
			{
				keyDirX = 1024; keyDirY = 0;        // 오른쪽
				hasDirectionInput = true;
			}

			// 키 입력 방향이 있을 때, 타겟 방향과 내적으로 비교
			if (hasDirectionInput)
			{
				// 타겟 방향 벡터 (정규화 필요 없음, 내적의 부호만 확인)
				const Scalar targetDirX = p1->x - p0.x;
				const Scalar targetDirY = p1->y - p0.y;

				// 내적 계산: dot = keyDir · targetDir
				const int64_t dot = MathUtil::GetInnerProduct(keyDirX, keyDirY, targetDirX, targetDirY);

				// 내적이 0 이하이면 90도 이상 차이 → 대시하지 않음
				// (내적 > 0: 전방 180도 범위 내)
				if (dot <= 0)
					continue;
			}
		}

		const Scalar d = p0.CalcSquaredLength(*p1);

		if (d < MathUtil::Pow(staticCharacterSkill.Get_c_skill_distance_min() << CELL_SIZE_DESIGN_FACTOR))
			continue;

			if (d > MathUtil::Pow(staticCharacterSkill.Get_c_skill_distance_max() << CELL_SIZE_DESIGN_FACTOR))
				continue;

			statContainer0->DecreaseStatValue(StatType::SP, costSp);

			const Span skillMotionDuration = (staticCharacterSkill.Get_c_skill_motion_duration() >> TIME_STEP_INTERVAL_FACTOR);

			Entity& e0 = *GetEntity(entityIdp);

			// 스킬 모션
			{
				MotionComponentDash& c = AttachComponent<MotionComponentDash>(e0);
				c.now = 0;
				c.duration = skillMotionDuration;
				c.speedRate = DebugInfo::SKILL_DASH_SPEED_RATE;
			}
		}
	}
}
