// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/System/Skill/SkillSystemHeal.h"

#include "Common/Stat/statContainer/StatContainer.h"
#include "MmoSync/Util/WorldIffChecker/WorldIffChecker.h"

#include <deque>
#include <vector>


SkillSystemHeal::SkillSystemHeal(Engine& engine, TimeStepUpdateCycle timeStepUpdateCycle, GameInstance& game)
	:
	SkillSystemBase(engine, timeStepUpdateCycle, game)
{
	IncludeComponent<SkillComponentHeal>();
	// 힐 스킬은 TacticsComponentCombat 불필요 (아군 대상이므로)
	// IncludeComponent<TacticsComponentCombat>();

	ExcludeComponent<MotionComponentGroupA>();
	ExcludeComponent<MotionComponentGroupB>();
	ExcludeComponent<MotionComponentGroupC>();
}

void SkillSystemHeal::Update()
{
	if (Data().empty())
		return;

	// 프레임 시작 시 Ray 캐시 초기화
	mRayCache.Clear();

	const TimeStepUpdateCycle timeDelta = GetTimeStepUpdateCycle();

	for (EntityIdp entityIdp : Data())
	{
		PositionComponent& p0 = GetComponent<PositionComponent>(entityIdp.first);
		CharacterComponent& c0 = GetComponent<CharacterComponent>(entityIdp.first);
		SkillComponentHeal& s0 = GetComponent<SkillComponentHeal>(entityIdp.first);
		Entity& e0 = *GetEntity(entityIdp);

#if defined(UE_EDITOR) || defined(_DEBUG)
		if (entityIdp.first == DebugInfo::DEBUG_ENTITY_ID)
			__nop();
#endif

		// ========== 1단계: 캐스팅 딜레이 처리 (힐 시전 중) ==========
		if (s0.castingDelay > 0)
		{
			if (s0.castingDelay > timeDelta)
				s0.castingDelay -= timeDelta;
			else
				s0.castingDelay = 0;

			if (s0.castingDelay > 0)
				continue;

			// 힐 타겟 엔티티 확인
			Entity* e1 = GetEntity(s0.skillTargetIdp);

			// 타겟이 힐 불가 상태인지 체크
			if (e1)
				if (e1->HasComponent(MotionComponentDying::GetComponentType()))
					e1 = nullptr;
				else if (e1->HasComponent(MotionComponentDead::GetComponentType()))
					e1 = nullptr;

			// 타겟이 유효하면 HP 회복 적용
			if (e1)
			{
				CharacterComponent* c1 = TryGetComponent<CharacterComponent>(*e1);
				if (c1)
				{
					if (StatContainer* statContainer1 = c1->GetStatContainer())
					{
						STATIC_CHARACTER_SKILL& staticCharacterSkill = s0.GetStaticCharacterSkill();
						const StatValue healAmount = (StatValue)staticCharacterSkill.Get_c_skill_power();
						const StatValue currentHp = statContainer1->GetStatValue(StatType::HP);
						const StatValue maxHp = statContainer1->GetStatValue(StatType::HP_MAX);
						const StatValue actualHeal = std::min(healAmount, maxHp - currentHp);

						if (actualHeal > 0)
							statContainer1->IncreaseStatValue(StatType::HP, actualHeal);
					}
				}
			}

			c0.skillTarget = INVALID_ENTITY_IDP;
			continue;
		}

		// 쿨타임 감소
		if (s0.cooltime > 0)
		{
			if (s0.cooltime > timeDelta)
				s0.cooltime -= timeDelta;
			else
				s0.cooltime = 0;
		}

		if (s0.cooltime > 0)
			continue;

		// ========== 2단계: SP 회복 및 스킬 시전 조건 검사 ==========
		StatContainer* statContainer0 = c0.GetStatContainer();
		if (!statContainer0)
			continue;

		// SP 자연 회복
		const StatValue currSp = statContainer0->GetStatValue(StatType::SP);
		const StatValue maxSp = statContainer0->GetStatValue(StatType::SP_MAX);
		const StatValue increaseSp = (StatValue)std::min((maxSp - currSp), (int)timeDelta);

		if (increaseSp > 0)
			statContainer0->IncreaseStatValue(StatType::SP, increaseSp);

		// 스킬 비용 확인
		STATIC_CHARACTER_SKILL& staticCharacterSkill = s0.GetStaticCharacterSkill();
		const StatValue costSp = (staticCharacterSkill.Get_c_skill_cost_sp() >> TIME_STEP_INTERVAL_FACTOR);
		const StatValue newSp = statContainer0->GetStatValue(StatType::SP);

		if (newSp < costSp)
			continue;

		// ========== 3단계: 아군 중 HP가 가장 낮은 대상 찾기 ==========
		const Scalar maxDistance = (staticCharacterSkill.Get_c_skill_distance_max() << CELL_SIZE_DESIGN_FACTOR);

		// 사거리 내 모든 엔티티 검색
		std::deque<EntityIdp> entityList;
		GetWorldManager().PickEntityInCircle(p0.x, p0.y, MathUtil::Pow(maxDistance), entityList);

		// 현재 엔티티의 PC/NPC 컴포넌트 확인 (아군 판별용)
		const PcComponent* pc0 = TryGetComponent<PcComponent>(entityIdp);
		const NpcComponent* npc0 = TryGetComponent<NpcComponent>(entityIdp);

		EntityIdp bestTargetIdp = INVALID_ENTITY_IDP;
		Scalar lowestHpRatio1024 = 1024; // HP 비율 * 1024 (1024 = 100%), 정수 연산으로 결정론적 보장
		PositionComponent* bestTargetPos = nullptr;

		MultiGrid& grid = RefMultiGrid();
		ColRowPair cell0 = grid.PickCell(p0.x, p0.y);

		for (EntityIdp idp1 : entityList)
		{
			// 자기 자신은 힐 대상에서 제외
			if (idp1 == entityIdp)
				continue;

			Entity* e1 = GetEntity(idp1);
			if (!e1)
				continue;

			// 죽은 대상은 힐 불가
			if (e1->HasComponent(MotionComponentDying::GetComponentType()) ||
				e1->HasComponent(MotionComponentDead::GetComponentType()))
				continue;

			// 아군인지 확인
			const PcComponent* pc1 = TryGetComponent<PcComponent>(idp1);
			const NpcComponent* npc1 = TryGetComponent<NpcComponent>(idp1);

			if (!WorldIffChecker::IsFriend(*this, entityIdp, idp1, pc0, npc0, pc1, npc1))
				continue;

			// HP 비율 확인
			CharacterComponent* c1 = TryGetComponent<CharacterComponent>(idp1);
			if (!c1)
				continue;

			StatContainer* statContainer1 = c1->GetStatContainer();
			if (!statContainer1)
				continue;

			const StatValue hp = statContainer1->GetStatValue(StatType::HP);
			const StatValue hpMax = statContainer1->GetStatValue(StatType::HP_MAX);

			if (hpMax <= 0)
				continue;

			// 이미 풀피면 힐 불필요
			if (hp >= hpMax)
				continue;

			const Scalar hpRatio1024 = (hp << 10) / hpMax; // HP 비율 * 1024

			// 시야 차단 체크 (캐시 사용)
			PositionComponent* p1 = TryGetComponent<PositionComponent>(idp1);
			if (p1)
			{
				ColRowPair cell1 = grid.PickCell(p1->x, p1->y);
				const bool isBlocked = mRayCache.IsRayBlockedCached(
					grid.GetColCount() - 1, grid.GetRowCount() - 1,
					cell0.first, cell0.second,
					cell1.first, cell1.second,
					-1,
					[&grid](GridCol c, GridRow r) { return grid.IsBlocked(c, r, GridMoverType::FLYER); }
				);
				if (isBlocked)
					continue;
			}

			// HP 비율이 더 낮은 아군 발견
			if (hpRatio1024 < lowestHpRatio1024)
			{
				lowestHpRatio1024 = hpRatio1024;
				bestTargetIdp = idp1;
				bestTargetPos = p1;
			}
		}

		// ========== 4단계: 힐 대상이 있으면 스킬 시전 ==========
		if (bestTargetIdp == INVALID_ENTITY_IDP || !bestTargetPos)
			continue;

		// SP 소모
		statContainer0->DecreaseStatValue(StatType::SP, costSp);

		s0.renderSkillX = bestTargetPos->x;
		s0.renderSkillY = bestTargetPos->y;

		// 쿨타임 시작
		s0.cooltime = (staticCharacterSkill.Get_c_skill_cooltime() >> TIME_STEP_INTERVAL_FACTOR);

		// 타겟 저장
		s0.skillTargetIdp = bestTargetIdp;
		c0.skillTarget = bestTargetIdp;

		// 캐스팅 딜레이 시작
		const Span skillMotionDuration = (staticCharacterSkill.Get_c_skill_motion_duration() >> TIME_STEP_INTERVAL_FACTOR);
		s0.castingDelay = skillMotionDuration;

		// 힐 대상 방향으로 회전
		if (PhysicsComponent* physics0 = TryGetComponent<PhysicsComponent>(e0))
			if (!physics0->IsKeyDown())
				physics0->UpdateAngleToTarget(bestTargetPos->x, bestTargetPos->y, p0.x, p0.y);

		// 스킬 모션 컴포넌트 추가
		MotionComponentSkillA& motion = AttachComponent<MotionComponentSkillA>(e0);
		motion.skillComponentType = staticCharacterSkill.Get_c_skill_component_type();
		motion.now = 0;
		motion.duration = skillMotionDuration;

		c0.clientCharacterState |= CLIENT_CHARACTER_STATE_SMOOTH_RADIAN_RESET;
	}
}
