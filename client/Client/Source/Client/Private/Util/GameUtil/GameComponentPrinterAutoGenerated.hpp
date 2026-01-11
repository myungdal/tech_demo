// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

case ComponentType::ABILITY_COMPONENT_GRID_BLOCK:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const AbilityComponentGridBlock&>(base)).data());
case ComponentType::ABILITY_COMPONENT_GRID_UNBLOCK:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const AbilityComponentGridUnblock&>(base)).data());
case ComponentType::ABILITY_COMPONENT_GRID_APPLIED:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const AbilityComponentGridApplied&>(base)).data());
case ComponentType::ABILITY_COMPONENT_RECOVERY:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const AbilityComponentRecovery&>(base)).data());
case ComponentType::ABILITY_COMPONENT_UNTARGETABLE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const AbilityComponentUntargetrable&>(base)).data());
case ComponentType::FORMATION_COMPONENT_GROUP:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const FormationComponentGroup&>(base)).data());
case ComponentType::FORMATION_COMPONENT_FLOCK:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const FormationComponentFlock&>(base)).data());
case ComponentType::FORMATION_COMPONENT_CROWD:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const FormationComponentCrowd&>(base)).data());
case ComponentType::MOTION_COMPONENT_GROUP_A:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentGroupA&>(base)).data());
case ComponentType::MOTION_COMPONENT_SKILL_A:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentSkillA&>(base)).data());
case ComponentType::MOTION_COMPONENT_BLEEDING:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentBleeding&>(base)).data());
case ComponentType::MOTION_COMPONENT_JUGGLE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentJuggle&>(base)).data());
case ComponentType::MOTION_COMPONENT_DASH:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentDash&>(base)).data());
case ComponentType::MOTION_COMPONENT_WEB:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentWeb&>(base)).data());
case ComponentType::MOTION_COMPONENT_DAMAGE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentDamage&>(base)).data());
case ComponentType::MOTION_COMPONENT_GROUP_B:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentGroupB&>(base)).data());
case ComponentType::MOTION_COMPONENT_SKILL_B:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentSkillB&>(base)).data());
case ComponentType::MOTION_COMPONENT_SHOCK:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentShock&>(base)).data());
case ComponentType::MOTION_COMPONENT_STUN:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentStun&>(base)).data());
case ComponentType::MOTION_COMPONENT_GROUP_C:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentGroupC&>(base)).data());
case ComponentType::MOTION_COMPONENT_DYING:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentDying&>(base)).data());
case ComponentType::MOTION_COMPONENT_DEAD:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentDead&>(base)).data());
case ComponentType::MOTION_COMPONENT_DEPARTURE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentDeparture&>(base)).data());
case ComponentType::MOTION_COMPONENT_ARRIVAL:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const MotionComponentArrival&>(base)).data());
case ComponentType::BUFF_COMPONENT_GROUP:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const BuffComponentGroup&>(base)).data());
case ComponentType::BUFF_COMPONENT_HEAL:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const BuffComponentHeal&>(base)).data());
case ComponentType::CHARACTER_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const CharacterComponent&>(base)).data());
case ComponentType::PC_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const PcComponent&>(base)).data());
case ComponentType::LEADER_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const LeaderComponent&>(base)).data());
case ComponentType::FOLLOWER_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const FollowerComponent&>(base)).data());
case ComponentType::NPC_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const NpcComponent&>(base)).data());
case ComponentType::DAMAGE_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DamageComponent&>(base)).data());
case ComponentType::DEBUFF_COMPONENT_GROUP:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DebuffComponentGroup&>(base)).data());
case ComponentType::DEBUFF_COMPONENT_EXPLOSION:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DebuffComponentExplosion&>(base)).data());
case ComponentType::DEBUFF_COMPONENT_HEALING:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DebuffComponentHealing&>(base)).data());
case ComponentType::DEBUFF_COMPONENT_PARASITE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DebuffComponentParasite&>(base)).data());
case ComponentType::DEBUFF_COMPONENT_POISON:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DebuffComponentPoison&>(base)).data());
case ComponentType::DEBUFF_COMPONENT_VIRUS:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DebuffComponentVirus&>(base)).data());
case ComponentType::DETECTION_COMPONENT_ENEMY:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DetectionComponentEnemy&>(base)).data());
case ComponentType::DETECTION_COMPONENT_FLOCK:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DetectionComponentFlock&>(base)).data());
case ComponentType::DETECTION_COMPONENT_PARTNER:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const DetectionComponentPartner&>(base)).data());
case ComponentType::IMPULSE_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const ImpulseComponent&>(base)).data());
case ComponentType::PHYSICS_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const PhysicsComponent&>(base)).data());
case ComponentType::POSITION_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const PositionComponent&>(base)).data());
case ComponentType::SKILL_COMPONENT_AXE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentAxe&>(base)).data());
case ComponentType::SKILL_COMPONENT_BOW:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentBow&>(base)).data());
case ComponentType::SKILL_COMPONENT_SPEAR:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentSpear&>(base)).data());
case ComponentType::SKILL_COMPONENT_BAZOOKA:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentBazooka&>(base)).data());
case ComponentType::SKILL_COMPONENT_BOMB:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentBomb&>(base)).data());
case ComponentType::SKILL_COMPONENT_HAMMER:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentHammer&>(base)).data());
case ComponentType::SKILL_COMPONENT_HEAL:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentHeal&>(base)).data());
case ComponentType::SKILL_COMPONENT_PARASITE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentParasite&>(base)).data());
case ComponentType::SKILL_COMPONENT_POISON:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentPoison&>(base)).data());
case ComponentType::SKILL_COMPONENT_WEB:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentWeb&>(base)).data());
case ComponentType::SKILL_COMPONENT_SPAWN:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentSpawn&>(base)).data());
case ComponentType::SKILL_COMPONENT_BLOCKING:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentBlocking&>(base)).data());
case ComponentType::SKILL_COMPONENT_LASER:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentLaser&>(base)).data());
case ComponentType::SKILL_COMPONENT_SWORD:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentSword&>(base)).data());
case ComponentType::SKILL_COMPONENT_DASH:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentDash&>(base)).data());
case ComponentType::SKILL_COMPONENT_ELECTRIC:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentElectric&>(base)).data());
case ComponentType::SKILL_COMPONENT_VIRUS:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentVirus&>(base)).data());
case ComponentType::SKILL_COMPONENT_VAMPIRE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentVampire&>(base)).data());
case ComponentType::SKILL_COMPONENT_BLINK:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentBlink&>(base)).data());
case ComponentType::SKILL_COMPONENT_PROJECTILE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SkillComponentProjectile&>(base)).data());
case ComponentType::SPAWNEE_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SpawneeComponent&>(base)).data());
case ComponentType::SPAWNER_COMPONENT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const SpawnerComponent&>(base)).data());
case ComponentType::TACTICS_COMPONENT_GROUP:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const TacticsComponentGroup&>(base)).data());
case ComponentType::TACTICS_COMPONENT_COMBAT:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const TacticsComponentCombat&>(base)).data());
case ComponentType::TACTICS_COMPONENT_RETURN:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const TacticsComponentReturn&>(base)).data());
case ComponentType::TACTICS_COMPONENT_IDLE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const TacticsComponentIdle&>(base)).data());
case ComponentType::TACTICS_COMPONENT_ROAMING:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const TacticsComponentRoaming&>(base)).data());
case ComponentType::TACTICS_COMPONENT_FLEE:
	return FString::Printf(TEXT("%s"), std::format(L"{}", reinterpret_cast<const TacticsComponentFlee&>(base)).data());
