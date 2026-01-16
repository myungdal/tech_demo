// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Stat/StatNodeBase/StatNodeBase.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스탯 노드 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// StatType별로 특수화된 스탯 노드 템플릿 클래스.
// - 책임: 각 StatType에 대한 스탯 값 계산 로직 구현
template <StatType statType>
class StatNode : public StatNodeBase {};

template <>
// STR: passthrough
class StatNode<StatType::STR> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override 
	{ 
		return GetStatValue(c, StatType::STR);
	}
};

template <>
// DEX: passthrough
class StatNode<StatType::DEX> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEX);
	}
};

template <>
// INT: passthrough
class StatNode<StatType::INT> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::INT);
	}
};

template <>
// WIS: passthrough
class StatNode<StatType::WIS> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::WIS);
	}
};

template <>
// CON: passthrough
class StatNode<StatType::CON> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::CON);
	}
};

template <>
// AGI: passthrough
class StatNode<StatType::AGI> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::AGI);
	}
};

template <>
// END: passthrough
class StatNode<StatType::END> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::END);
	}
};

template <>
// HP: passthrough
class StatNode<StatType::HP> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::HP);
	}
};

template <>
// SP: passthrough
class StatNode<StatType::SP> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::SP);
	}
};

template <>
// HP_MAX_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::HP_MAX_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::HP_MAX_MULTIPLIER);
	}
};

template <>
// HP_MAX: base value with HP_MAX_MULTIPLIER (bp) applied
class StatNode<StatType::HP_MAX> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::HP_MAX_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::HP_MAX), 10000 + GetStatValue(c, StatType::HP_MAX_MULTIPLIER));
	}
};

template <>
// HP_RECOVERY_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::HP_RECOVERY_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::HP_RECOVERY_MULTIPLIER);
	}
};

template <>
// HP_RECOVERY: base value with HP_RECOVERY_MULTIPLIER (bp) applied
class StatNode<StatType::HP_RECOVERY> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::HP_RECOVERY_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::HP_RECOVERY), 10000 + GetStatValue(c, StatType::HP_RECOVERY_MULTIPLIER));
	}
};

template <>
// SP_MAX_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::SP_MAX_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::SP_MAX_MULTIPLIER);
	}
};

template <>
// SP_MAX: base value with SP_MAX_MULTIPLIER (bp) applied
class StatNode<StatType::SP_MAX> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::SP_MAX_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::SP_MAX), 10000 + GetStatValue(c, StatType::SP_MAX_MULTIPLIER));
	}
};

template <>
// SP_RECOVERY_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::SP_RECOVERY_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::SP_RECOVERY_MULTIPLIER);
	}
};

template <>
// SP_RECOVERY: base value with SP_RECOVERY_MULTIPLIER (bp) applied
class StatNode<StatType::SP_RECOVERY> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::SP_RECOVERY_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::SP_RECOVERY), 10000 + GetStatValue(c, StatType::SP_RECOVERY_MULTIPLIER));
	}
};

template <>
// SHIELD_MAX_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::SHIELD_MAX_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::SHIELD_MAX_MULTIPLIER);
	}
};

template <>
// SHIELD_MAX: base value with SHIELD_MAX_MULTIPLIER (bp) applied
class StatNode<StatType::SHIELD_MAX> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::SHIELD_MAX_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::SHIELD_MAX), 10000 + GetStatValue(c, StatType::SHIELD_MAX_MULTIPLIER));
	}
};

template <>
// LUCK: passthrough
class StatNode<StatType::LUCK> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::LUCK);
	}
};

template <>
// MOVE_SPEED_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::MOVE_SPEED_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::MOVE_SPEED_MULTIPLIER);
	}
};

template <>
// MOVE_SPEED: base value with MOVE_SPEED_MULTIPLIER (bp) applied
class StatNode<StatType::MOVE_SPEED> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::MOVE_SPEED_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::MOVE_SPEED), 10000 + GetStatValue(c, StatType::MOVE_SPEED_MULTIPLIER));
	}
};

template <>
// SKILL_COOLDOWN_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::SKILL_COOLDOWN_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::SKILL_COOLDOWN_MULTIPLIER);
	}
};

template <>
// DETECT_DISTANCE_MULTIFLIER: passthrough
class StatNode<StatType::DETECT_DISTANCE_MULTIFLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DETECT_DISTANCE_MULTIFLIER);
	}
};

template <>
// SKILL_DISTANCE_MULTIFLIER: passthrough
class StatNode<StatType::SKILL_DISTANCE_MULTIFLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::SKILL_DISTANCE_MULTIFLIER);
	}
};

template <>
// SPLASH_DISTANCE_MULTIFLIER: passthrough
class StatNode<StatType::SPLASH_DISTANCE_MULTIFLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::SPLASH_DISTANCE_MULTIFLIER);
	}
};

template <>
// SPLASH_DAMAGE_MULTIFLIER: passthrough
class StatNode<StatType::SPLASH_DAMAGE_MULTIFLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::SPLASH_DAMAGE_MULTIFLIER);
	}
};

template <>
// ATTACK_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::ATTACK_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::ATTACK_MULTIPLIER);
	}
};

template <>
// ATTACK: base value with ATTACK_MULTIPLIER (bp) applied
class StatNode<StatType::ATTACK> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::ATTACK_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::ATTACK), 10000 + GetStatValue(c, StatType::ATTACK_MULTIPLIER));
	}
};

template <>
// ATTACK_MELEE_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::ATTACK_MELEE_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::ATTACK_MELEE_MULTIPLIER);
	}
};

template <>
// ATTACK_MELEE: base value with ATTACK_MELEE_MULTIPLIER (bp) applied
class StatNode<StatType::ATTACK_MELEE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::ATTACK_MELEE_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::ATTACK_MELEE), 10000 + GetStatValue(c, StatType::ATTACK_MELEE_MULTIPLIER));
	}
};

template <>
// ATTACK_RANGED_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::ATTACK_RANGED_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::ATTACK_RANGED_MULTIPLIER);
	}
};

template <>
// ATTACK_RANGED: base value with ATTACK_RANGED_MULTIPLIER (bp) applied
class StatNode<StatType::ATTACK_RANGED> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::ATTACK_RANGED_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::ATTACK_RANGED), 10000 + GetStatValue(c, StatType::ATTACK_RANGED_MULTIPLIER));
	}
};

template <>
// ATTACK_PHYSIC_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::ATTACK_PHYSIC_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::ATTACK_PHYSIC_MULTIPLIER);
	}
};

template <>
// ATTACK_PHYSIC: base value with ATTACK_PHYSIC_MULTIPLIER (bp) applied
class StatNode<StatType::ATTACK_PHYSIC> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::ATTACK_PHYSIC_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::ATTACK_PHYSIC), 10000 + GetStatValue(c, StatType::ATTACK_PHYSIC_MULTIPLIER));
	}
};

template <>
// ATTACK_MAGIC_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::ATTACK_MAGIC_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::ATTACK_MAGIC_MULTIPLIER);
	}
};

template <>
// ATTACK_MAGIC: base value with ATTACK_MAGIC_MULTIPLIER (bp) applied
class StatNode<StatType::ATTACK_MAGIC> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::ATTACK_MAGIC_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::ATTACK_MAGIC), 10000 + GetStatValue(c, StatType::ATTACK_MAGIC_MULTIPLIER));
	}
};

template <>
// DEFENSE_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::DEFENSE_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEFENSE_MULTIPLIER);
	}
};

template <>
// DEFENSE: base value with DEFENSE_MULTIPLIER (bp) applied
class StatNode<StatType::DEFENSE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::DEFENSE_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::DEFENSE), 10000 + GetStatValue(c, StatType::DEFENSE_MULTIPLIER));
	}
};

template <>
// DEFENSE_MELEE_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::DEFENSE_MELEE_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEFENSE_MELEE_MULTIPLIER);
	}
};

template <>
// DEFENSE_MELEE: base value with DEFENSE_MELEE_MULTIPLIER (bp) applied
class StatNode<StatType::DEFENSE_MELEE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::DEFENSE_MELEE_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::DEFENSE_MELEE), 10000 + GetStatValue(c, StatType::DEFENSE_MELEE_MULTIPLIER));
	}
};

template <>
// DEFENSE_RANGED_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::DEFENSE_RANGED_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEFENSE_RANGED_MULTIPLIER);
	}
};

template <>
// DEFENSE_RANGED: base value with DEFENSE_RANGED_MULTIPLIER (bp) applied
class StatNode<StatType::DEFENSE_RANGED> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::DEFENSE_RANGED_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::DEFENSE_RANGED), 10000 + GetStatValue(c, StatType::DEFENSE_RANGED_MULTIPLIER));
	}
};

template <>
// DEFENSE_PHYSIC_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::DEFENSE_PHYSIC_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEFENSE_PHYSIC_MULTIPLIER);
	}
};

template <>
// DEFENSE_PHYSIC: base value with DEFENSE_PHYSIC_MULTIPLIER (bp) applied
class StatNode<StatType::DEFENSE_PHYSIC> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::DEFENSE_PHYSIC_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::DEFENSE_PHYSIC), 10000 + GetStatValue(c, StatType::DEFENSE_PHYSIC_MULTIPLIER));
	}
};

template <>
// DEFENSE_MAGIC_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::DEFENSE_MAGIC_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEFENSE_MAGIC_MULTIPLIER);
	}
};

template <>
// DEFENSE_MAGIC: base value with DEFENSE_MAGIC_MULTIPLIER (bp) applied
class StatNode<StatType::DEFENSE_MAGIC> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::DEFENSE_MAGIC_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::DEFENSE_MAGIC), 10000 + GetStatValue(c, StatType::DEFENSE_MAGIC_MULTIPLIER));
	}
};

template <>
// HIT_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::HIT_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::HIT_MULTIPLIER);
	}
};

template <>
// HIT: base value with HIT_MULTIPLIER (bp) applied
class StatNode<StatType::HIT> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::HIT_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::HIT), 10000 + GetStatValue(c, StatType::HIT_MULTIPLIER));
	}
};

template <>
// DODGE_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::DODGE_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DODGE_MULTIPLIER);
	}
};

template <>
// DODGE: base value with DODGE_MULTIPLIER (bp) applied
class StatNode<StatType::DODGE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::DODGE_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::DODGE), 10000 + GetStatValue(c, StatType::DODGE_MULTIPLIER));
	}
};

template <>
// DAMAGE_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::DAMAGE_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DAMAGE_MULTIPLIER);
	}
};

template <>
// DAMAGE: base value with DAMAGE_MULTIPLIER (bp) applied
class StatNode<StatType::DAMAGE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::DAMAGE_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::DAMAGE), 10000 + GetStatValue(c, StatType::DAMAGE_MULTIPLIER));
	}
};

template <>
// REDUNTION_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::REDUNTION_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::REDUNTION_MULTIPLIER);
	}
};

template <>
// REDUNTION: base value with REDUNTION_MULTIPLIER (bp) applied
class StatNode<StatType::REDUNTION> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, { StatType::REDUNTION_MULTIPLIER }) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return MathUtil::MultiplyBp<StatValue>(GetStatValue(c, StatType::REDUNTION), 10000 + GetStatValue(c, StatType::REDUNTION_MULTIPLIER));
	}
};

template <>
// CRITICAL_HIT: passthrough
class StatNode<StatType::CRITICAL_HIT> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::CRITICAL_HIT);
	}
};

template <>
// CRITICAL_DODGE: passthrough
class StatNode<StatType::CRITICAL_DODGE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::CRITICAL_DODGE);
	}
};

template <>
// CRITICAL_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::CRITICAL_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::CRITICAL_MULTIPLIER);
	}
};

template <>
// CRITICAL_RESISTANCE: passthrough
class StatNode<StatType::CRITICAL_RESISTANCE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::CRITICAL_RESISTANCE);
	}
};

template <>
// DEBUFF_HIT: passthrough
class StatNode<StatType::DEBUFF_HIT> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEBUFF_HIT);
	}
};

template <>
// DEBUFF_DODGE: passthrough
class StatNode<StatType::DEBUFF_DODGE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEBUFF_DODGE);
	}
};

template <>
// DEBUFF_MULTIFLIER: passthrough
class StatNode<StatType::DEBUFF_MULTIFLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEBUFF_MULTIFLIER);
	}
};

template <>
// DEBUFF_RESISTANCE: passthrough
class StatNode<StatType::DEBUFF_RESISTANCE> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::DEBUFF_RESISTANCE);
	}
};

template <>
// BUFF_APPLY_MULTIFLIER: passthrough
class StatNode<StatType::BUFF_APPLY_MULTIFLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::BUFF_APPLY_MULTIFLIER);
	}
};

template <>
// BUFF_RECEIVE_MULTIFLIER: passthrough
class StatNode<StatType::BUFF_RECEIVE_MULTIFLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::BUFF_RECEIVE_MULTIFLIER);
	}
};

template <>
// EXP_MULTIFLIER: passthrough
class StatNode<StatType::EXP_MULTIFLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::EXP_MULTIFLIER);
	}
};

template <>
// GOLD_MULTIPLIER: multiplier (bp, 10000=1.0)
class StatNode<StatType::GOLD_MULTIPLIER> : public StatNodeBase
{
public:
	explicit StatNode(StatType statType) : StatNodeBase(statType, {}) {}
	StatValue UpdateStatValue(StatContainer& c) const override
	{
		return GetStatValue(c, StatType::GOLD_MULTIPLIER);
	}
};
