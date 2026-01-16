// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

Register<SearchSystem>(SystemType::SEARCH_SYSTEM, 1, systemManager, engine, game);
Register<GridSystemBlock>(SystemType::GRID_SYSTEM_BLOCK, 1, systemManager, engine, game);
Register<GridSystemUnblock>(SystemType::GRID_SYSTEM_UNBLOCK, 1, systemManager, engine, game);
Register<DetectionSystemEnemy>(SystemType::DETECTION_SYSTEM_ENEMY, 1, systemManager, engine, game);
Register<DetectionSystemFlock>(SystemType::DETECTION_SYSTEM_FLOCK, 1, systemManager, engine, game);
Register<DetectionSystemPartner>(SystemType::DETECTION_SYSTEM_PARTNER, 1, systemManager, engine, game);
Register<PhysicsSystemInit>(SystemType::PHYSICS_SYSTEM_INIT, 1, systemManager, engine, game);
Register<CharacterSystem>(SystemType::CHARACTER_SYSTEM, 1, systemManager, engine, game);
Register<PcSystem>(SystemType::PC_SYSTEM, 1, systemManager, engine, game);
Register<NpcSystem>(SystemType::NPC_SYSTEM, 1, systemManager, engine, game);
Register<RecoverySystem>(SystemType::RECOVERY_SYSTEM, 1, systemManager, engine, game);
Register<DamageSystem>(SystemType::DAMAGE_SYSTEM, 1, systemManager, engine, game);
Register<FormationSystemFlock>(SystemType::FORMATION_SYSTEM_FLOCK, 1, systemManager, engine, game);
Register<FormationSystemCrowd>(SystemType::FORMATION_SYSTEM_CROWD, 1, systemManager, engine, game);
Register<FormationSystemCrowdEnemy>(SystemType::FORMATION_SYSTEM_CROWD_ENEMY, 1, systemManager, engine, game);
Register<TacticsSystemCombat>(SystemType::TACTICS_SYSTEM_COMBAT, 1, systemManager, engine, game);
Register<TacticsSystemReturn>(SystemType::TACTICS_SYSTEM_RETURN, 1, systemManager, engine, game);
Register<TacticsSystemIdle>(SystemType::TACTICS_SYSTEM_IDLE, 1, systemManager, engine, game);
Register<TacticsSystemRoaming>(SystemType::TACTICS_SYSTEM_ROAMING, 1, systemManager, engine, game);
Register<TacticsSystemFlee>(SystemType::TACTICS_SYSTEM_FLEE, 1, systemManager, engine, game);
Register<PhysicsSystemVelocityAir>(SystemType::PHYSICS_SYSTEM_VELOCITY_AIR, 1, systemManager, engine, game);
Register<PhysicsSystemVelocityGround>(SystemType::PHYSICS_SYSTEM_VELOCITY_GROUND, 1, systemManager, engine, game);
Register<PhysicsSystemMovementAir>(SystemType::PHYSICS_SYSTEM_MOVEMENT_AIR, 1, systemManager, engine, game);
Register<PhysicsSystemMovementGround>(SystemType::PHYSICS_SYSTEM_MOVEMENT_GROUND, 1, systemManager, engine, game);
Register<SkillSystemAxe>(SystemType::SKILL_SYSTEM_AXE, 1, systemManager, engine, game);
Register<SkillSystemBazooka>(SystemType::SKILL_SYSTEM_BAZOOKA, 1, systemManager, engine, game);
Register<SkillSystemBlocking>(SystemType::SKILL_SYSTEM_BLOCKING, 1, systemManager, engine, game);
Register<SkillSystemBomb>(SystemType::SKILL_SYSTEM_BOMB, 1, systemManager, engine, game);
Register<SkillSystemBow>(SystemType::SKILL_SYSTEM_BOW, 1, systemManager, engine, game);
Register<SkillSystemDash>(SystemType::SKILL_SYSTEM_DASH, 1, systemManager, engine, game);
Register<SkillSystemBlink>(SystemType::SKILL_SYSTEM_BLINK, 1, systemManager, engine, game);
Register<SkillSystemElectric>(SystemType::SKILL_SYSTEM_ELECTRIC, 1, systemManager, engine, game);
Register<SkillSystemHammer>(SystemType::SKILL_SYSTEM_HAMMER, 1, systemManager, engine, game);
Register<SkillSystemHeal>(SystemType::SKILL_SYSTEM_HEAL, 1, systemManager, engine, game);
Register<SkillSystemLaser>(SystemType::SKILL_SYSTEM_LASER, 1, systemManager, engine, game);
Register<SkillSystemParasite>(SystemType::SKILL_SYSTEM_PARASITE, 1, systemManager, engine, game);
Register<SkillSystemPoison>(SystemType::SKILL_SYSTEM_POISON, 1, systemManager, engine, game);
Register<SkillSystemSpear>(SystemType::SKILL_SYSTEM_SPEAR, 1, systemManager, engine, game);
Register<SkillSystemSword>(SystemType::SKILL_SYSTEM_SWORD, 1, systemManager, engine, game);
Register<SkillSystemVampire>(SystemType::SKILL_SYSTEM_VAMPIRE, 1, systemManager, engine, game);
Register<SkillSystemVirus>(SystemType::SKILL_SYSTEM_VIRUS, 1, systemManager, engine, game);
Register<SkillSystemWeb>(SystemType::SKILL_SYSTEM_WEB, 1, systemManager, engine, game);
Register<SkillSystemProjectile>(SystemType::SKILL_SYSTEM_PROJECTILE, 1, systemManager, engine, game);
Register<MotionSystemBleeding>(SystemType::MOTION_SYSTEM_BLEEDING, 1, systemManager, engine, game);
Register<MotionSystemDying>(SystemType::MOTION_SYSTEM_DYING, 1, systemManager, engine, game);
Register<MotionSystemJuggle>(SystemType::MOTION_SYSTEM_JUGGLE, 1, systemManager, engine, game);
Register<MotionSystemShock>(SystemType::MOTION_SYSTEM_SHOCK, 1, systemManager, engine, game);
Register<MotionSystemStun>(SystemType::MOTION_SYSTEM_STUN, 1, systemManager, engine, game);
Register<MotionSystemWeb>(SystemType::MOTION_SYSTEM_WEB, 1, systemManager, engine, game);
Register<MotionSystemSkillA>(SystemType::MOTION_SYSTEM_SKILL_A, 1, systemManager, engine, game);
Register<MotionSystemSkillB>(SystemType::MOTION_SYSTEM_SKILL_B, 1, systemManager, engine, game);
Register<MotionSystemDamage>(SystemType::MOTION_SYSTEM_DAMAGE, 1, systemManager, engine, game);
Register<MotionSystemDead>(SystemType::MOTION_SYSTEM_DEAD, 1, systemManager, engine, game);
Register<MotionSystemDash>(SystemType::MOTION_SYSTEM_DASH, 1, systemManager, engine, game);
Register<MotionSystemArrival>(SystemType::MOTION_SYSTEM_ARRIVAL, 1, systemManager, engine, game);
Register<MotionSystemDeparture>(SystemType::MOTION_SYSTEM_DEPARTURE, 1, systemManager, engine, game);
Register<DebuffSystemExplosion>(SystemType::DEBUFF_SYSTEM_EXPLOSION, 1, systemManager, engine, game);
Register<DebuffSystemHealing>(SystemType::DEBUFF_SYSTEM_HEALING, 1, systemManager, engine, game);
Register<DebuffSystemParasite>(SystemType::DEBUFF_SYSTEM_PARASITE, 1, systemManager, engine, game);
Register<DebuffSystemPoison>(SystemType::DEBUFF_SYSTEM_POISON, 1, systemManager, engine, game);
Register<DebuffSystemVirus>(SystemType::DEBUFF_SYSTEM_VIRUS, 1, systemManager, engine, game);
Register<BuffSystemHeal>(SystemType::BUFF_SYSTEM_HEAL, 1, systemManager, engine, game);
Register<SpawneeSystem>(SystemType::SPAWNEE_SYSTEM, 1, systemManager, engine, game);
Register<SpawnerSystem>(SystemType::SPAWNER_SYSTEM, 1, systemManager, engine, game);
Register<GridSystemBlockOnDead>(SystemType::GRID_SYSTEM_BLOCK_ON_DEAD, 1, systemManager, engine, game);
Register<GridSystemUnblockOnDead>(SystemType::GRID_SYSTEM_UNBLOCK_ON_DEAD, 1, systemManager, engine, game);
