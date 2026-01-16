// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Stat/StatNodeBuilder.h"

#include "Common/Stat/StatNode.h"


namespace StatNodeBuilderUtil
{
	// StatType별 노드를 생성한다.
	template <StatType statType>
	void CreateNode(StatNodeArray& nodes)
	{
		std::unique_ptr<StatNode<statType>> node = std::make_unique<StatNode<statType>>(statType);
		nodes.at(static_cast<size_t>(statType)) = std::move(node);
	}
}

StatNodeBuilder::StatNodeBuilder()
{
	// [WHY] 모든 StatType에 대해 노드 생성
	StatNodeBuilderUtil::CreateNode<StatType::STR>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEX>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::INT>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::WIS>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::CON>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::AGI>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::END>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::HP>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SP>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::HP_MAX_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::HP_MAX>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::HP_RECOVERY_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::HP_RECOVERY>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SP_MAX_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SP_MAX>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SP_RECOVERY_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SP_RECOVERY>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SHIELD_MAX_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SHIELD_MAX>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::LUCK>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::MOVE_SPEED_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::MOVE_SPEED>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SKILL_COOLDOWN_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DETECT_DISTANCE_MULTIFLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SKILL_DISTANCE_MULTIFLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SPLASH_DISTANCE_MULTIFLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::SPLASH_DAMAGE_MULTIFLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_MELEE_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_MELEE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_RANGED_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_RANGED>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_PHYSIC_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_PHYSIC>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_MAGIC_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::ATTACK_MAGIC>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_MELEE_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_MELEE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_RANGED_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_RANGED>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_PHYSIC_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_PHYSIC>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_MAGIC_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEFENSE_MAGIC>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::HIT_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::HIT>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DODGE_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DODGE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DAMAGE_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DAMAGE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::REDUNTION_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::REDUNTION>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::CRITICAL_HIT>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::CRITICAL_DODGE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::CRITICAL_MULTIPLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::CRITICAL_RESISTANCE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEBUFF_HIT>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEBUFF_DODGE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEBUFF_MULTIFLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::DEBUFF_RESISTANCE>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::BUFF_APPLY_MULTIFLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::BUFF_RECEIVE_MULTIFLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::EXP_MULTIFLIER>(mNodes);
	StatNodeBuilderUtil::CreateNode<StatType::GOLD_MULTIPLIER>(mNodes);

	BuildDependent();
}

void StatNodeBuilder::BuildDependent()
{
	// [WHY] 각 노드의 의존성을 역방향으로 설정 (의존하는 노드에 의존받는 노드 추가)
	for (std::unique_ptr<StatNodeBase>& node : mNodes)
	{
		if (!node)
			continue;

		const std::set<StatType>& dependencySet = node->GetDependencySet();
		for (StatType dependency : dependencySet)
		{
			std::unique_ptr<StatNodeBase>& dependencyNode = mNodes.at(static_cast<size_t>(dependency));
			dependencyNode->InsertDependent(node->GetStatType());
		}
	}

	// [WHY] 각 노드의 간접 의존성도 설정 (의존성 체인 전체에 의존받는 노드 추가)
	for (std::unique_ptr<StatNodeBase>& node : mNodes)
	{
		if (node)
		{
			InsertDependentTo(node->GetStatType(), node->GetStatType());
		}
	}
}

void StatNodeBuilder::InsertDependentTo(StatType from, StatType to)
{
	// [WHY] DFS로 의존성 그래프를 순회하여 간접 의존성 설정
	std::stack<StatType> stack;
	stack.push(from);

	while (!stack.empty()) 
	{
		StatType current = stack.top();
		stack.pop();

		GetStatNode(to).InsertDependent(current);

		const std::set<StatType>& dependencySet = GetStatNode(current).GetDependencySet();
		for (StatType dependency : dependencySet) 
		{
			// [WHY] 자기 자신으로의 순환 참조 방지
			if (dependency != to) 
				stack.push(dependency);
		}
	}
}


StatNodeBase& StatNodeBuilder::GetStatNode(StatType statType)
{
	return *mNodes.at(static_cast<size_t>(statType));
}
