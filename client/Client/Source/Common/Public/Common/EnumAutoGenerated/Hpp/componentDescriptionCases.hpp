// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

case ComponentType::ABILITY_COMPONENT_GRID_BLOCK:
{
   AttachComponent<AbilityComponentGridBlock>(entity) = *static_cast<const AbilityComponentGridBlock*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::ABILITY_COMPONENT_GRID_UNBLOCK:
{
   AttachComponent<AbilityComponentGridUnblock>(entity) = *static_cast<const AbilityComponentGridUnblock*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::ABILITY_COMPONENT_GRID_APPLIED:
{
   AttachComponent<AbilityComponentGridApplied>(entity) = *static_cast<const AbilityComponentGridApplied*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::ABILITY_COMPONENT_RECOVERY:
{
   AttachComponent<AbilityComponentRecovery>(entity) = *static_cast<const AbilityComponentRecovery*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::ABILITY_COMPONENT_UNTARGETABLE:
{
   AttachComponent<AbilityComponentUntargetrable>(entity) = *static_cast<const AbilityComponentUntargetrable*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::FORMATION_COMPONENT_GROUP:
{
   AttachComponent<FormationComponentGroup>(entity) = *static_cast<const FormationComponentGroup*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::FORMATION_COMPONENT_FLOCK:
{
   AttachComponent<FormationComponentFlock>(entity) = *static_cast<const FormationComponentFlock*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::FORMATION_COMPONENT_CROWD:
{
   AttachComponent<FormationComponentCrowd>(entity) = *static_cast<const FormationComponentCrowd*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_GROUP_A:
{
   AttachComponent<MotionComponentGroupA>(entity) = *static_cast<const MotionComponentGroupA*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_SKILL_A:
{
   AttachComponent<MotionComponentSkillA>(entity) = *static_cast<const MotionComponentSkillA*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_BLEEDING:
{
   AttachComponent<MotionComponentBleeding>(entity) = *static_cast<const MotionComponentBleeding*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_JUGGLE:
{
   AttachComponent<MotionComponentJuggle>(entity) = *static_cast<const MotionComponentJuggle*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_DASH:
{
   AttachComponent<MotionComponentDash>(entity) = *static_cast<const MotionComponentDash*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_WEB:
{
   AttachComponent<MotionComponentWeb>(entity) = *static_cast<const MotionComponentWeb*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_DAMAGE:
{
   AttachComponent<MotionComponentDamage>(entity) = *static_cast<const MotionComponentDamage*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_GROUP_B:
{
   AttachComponent<MotionComponentGroupB>(entity) = *static_cast<const MotionComponentGroupB*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_SKILL_B:
{
   AttachComponent<MotionComponentSkillB>(entity) = *static_cast<const MotionComponentSkillB*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_SHOCK:
{
   AttachComponent<MotionComponentShock>(entity) = *static_cast<const MotionComponentShock*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_STUN:
{
   AttachComponent<MotionComponentStun>(entity) = *static_cast<const MotionComponentStun*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_GROUP_C:
{
   AttachComponent<MotionComponentGroupC>(entity) = *static_cast<const MotionComponentGroupC*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_DYING:
{
   AttachComponent<MotionComponentDying>(entity) = *static_cast<const MotionComponentDying*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_DEAD:
{
   AttachComponent<MotionComponentDead>(entity) = *static_cast<const MotionComponentDead*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_DEPARTURE:
{
   AttachComponent<MotionComponentDeparture>(entity) = *static_cast<const MotionComponentDeparture*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::MOTION_COMPONENT_ARRIVAL:
{
   AttachComponent<MotionComponentArrival>(entity) = *static_cast<const MotionComponentArrival*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::BUFF_COMPONENT_GROUP:
{
   AttachComponent<BuffComponentGroup>(entity) = *static_cast<const BuffComponentGroup*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::BUFF_COMPONENT_HEAL:
{
   AttachComponent<BuffComponentHeal>(entity) = *static_cast<const BuffComponentHeal*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::CHARACTER_COMPONENT:
{
   AttachComponent<CharacterComponent>(entity) = *static_cast<const CharacterComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::PC_COMPONENT:
{
   AttachComponent<PcComponent>(entity) = *static_cast<const PcComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::LEADER_COMPONENT:
{
   AttachComponent<LeaderComponent>(entity) = *static_cast<const LeaderComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::FOLLOWER_COMPONENT:
{
   AttachComponent<FollowerComponent>(entity) = *static_cast<const FollowerComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::NPC_COMPONENT:
{
   AttachComponent<NpcComponent>(entity) = *static_cast<const NpcComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DAMAGE_COMPONENT:
{
   AttachComponent<DamageComponent>(entity) = *static_cast<const DamageComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DEBUFF_COMPONENT_GROUP:
{
   AttachComponent<DebuffComponentGroup>(entity) = *static_cast<const DebuffComponentGroup*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DEBUFF_COMPONENT_EXPLOSION:
{
   AttachComponent<DebuffComponentExplosion>(entity) = *static_cast<const DebuffComponentExplosion*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DEBUFF_COMPONENT_HEALING:
{
   AttachComponent<DebuffComponentHealing>(entity) = *static_cast<const DebuffComponentHealing*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DEBUFF_COMPONENT_PARASITE:
{
   AttachComponent<DebuffComponentParasite>(entity) = *static_cast<const DebuffComponentParasite*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DEBUFF_COMPONENT_POISON:
{
   AttachComponent<DebuffComponentPoison>(entity) = *static_cast<const DebuffComponentPoison*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DEBUFF_COMPONENT_VIRUS:
{
   AttachComponent<DebuffComponentVirus>(entity) = *static_cast<const DebuffComponentVirus*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DETECTION_COMPONENT_ENEMY:
{
   AttachComponent<DetectionComponentEnemy>(entity) = *static_cast<const DetectionComponentEnemy*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DETECTION_COMPONENT_FLOCK:
{
   AttachComponent<DetectionComponentFlock>(entity) = *static_cast<const DetectionComponentFlock*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::DETECTION_COMPONENT_PARTNER:
{
   AttachComponent<DetectionComponentPartner>(entity) = *static_cast<const DetectionComponentPartner*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::IMPULSE_COMPONENT:
{
   AttachComponent<ImpulseComponent>(entity) = *static_cast<const ImpulseComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::PHYSICS_COMPONENT:
{
   AttachComponent<PhysicsComponent>(entity) = *static_cast<const PhysicsComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::POSITION_COMPONENT:
{
   AttachComponent<PositionComponent>(entity) = *static_cast<const PositionComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_AXE:
{
   AttachComponent<SkillComponentAxe>(entity) = *static_cast<const SkillComponentAxe*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_BOW:
{
   AttachComponent<SkillComponentBow>(entity) = *static_cast<const SkillComponentBow*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_SPEAR:
{
   AttachComponent<SkillComponentSpear>(entity) = *static_cast<const SkillComponentSpear*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_BAZOOKA:
{
   AttachComponent<SkillComponentBazooka>(entity) = *static_cast<const SkillComponentBazooka*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_BOMB:
{
   AttachComponent<SkillComponentBomb>(entity) = *static_cast<const SkillComponentBomb*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_HAMMER:
{
   AttachComponent<SkillComponentHammer>(entity) = *static_cast<const SkillComponentHammer*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_HEAL:
{
   AttachComponent<SkillComponentHeal>(entity) = *static_cast<const SkillComponentHeal*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_PARASITE:
{
   AttachComponent<SkillComponentParasite>(entity) = *static_cast<const SkillComponentParasite*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_POISON:
{
   AttachComponent<SkillComponentPoison>(entity) = *static_cast<const SkillComponentPoison*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_WEB:
{
   AttachComponent<SkillComponentWeb>(entity) = *static_cast<const SkillComponentWeb*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_SPAWN:
{
   AttachComponent<SkillComponentSpawn>(entity) = *static_cast<const SkillComponentSpawn*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_BLOCKING:
{
   AttachComponent<SkillComponentBlocking>(entity) = *static_cast<const SkillComponentBlocking*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_LASER:
{
   AttachComponent<SkillComponentLaser>(entity) = *static_cast<const SkillComponentLaser*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_SWORD:
{
   AttachComponent<SkillComponentSword>(entity) = *static_cast<const SkillComponentSword*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_DASH:
{
   AttachComponent<SkillComponentDash>(entity) = *static_cast<const SkillComponentDash*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_ELECTRIC:
{
   AttachComponent<SkillComponentElectric>(entity) = *static_cast<const SkillComponentElectric*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_VIRUS:
{
   AttachComponent<SkillComponentVirus>(entity) = *static_cast<const SkillComponentVirus*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_VAMPIRE:
{
   AttachComponent<SkillComponentVampire>(entity) = *static_cast<const SkillComponentVampire*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_BLINK:
{
   AttachComponent<SkillComponentBlink>(entity) = *static_cast<const SkillComponentBlink*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SKILL_COMPONENT_PROJECTILE:
{
   AttachComponent<SkillComponentProjectile>(entity) = *static_cast<const SkillComponentProjectile*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SPAWNEE_COMPONENT:
{
   AttachComponent<SpawneeComponent>(entity) = *static_cast<const SpawneeComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::SPAWNER_COMPONENT:
{
   AttachComponent<SpawnerComponent>(entity) = *static_cast<const SpawnerComponent*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::TACTICS_COMPONENT_GROUP:
{
   AttachComponent<TacticsComponentGroup>(entity) = *static_cast<const TacticsComponentGroup*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::TACTICS_COMPONENT_COMBAT:
{
   AttachComponent<TacticsComponentCombat>(entity) = *static_cast<const TacticsComponentCombat*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::TACTICS_COMPONENT_RETURN:
{
   AttachComponent<TacticsComponentReturn>(entity) = *static_cast<const TacticsComponentReturn*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::TACTICS_COMPONENT_IDLE:
{
   AttachComponent<TacticsComponentIdle>(entity) = *static_cast<const TacticsComponentIdle*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::TACTICS_COMPONENT_ROAMING:
{
   AttachComponent<TacticsComponentRoaming>(entity) = *static_cast<const TacticsComponentRoaming*>(componentDescription.mDescription.get());
   break; 
}
case ComponentType::TACTICS_COMPONENT_FLEE:
{
   AttachComponent<TacticsComponentFlee>(entity) = *static_cast<const TacticsComponentFlee*>(componentDescription.mDescription.get());
   break; 
}
