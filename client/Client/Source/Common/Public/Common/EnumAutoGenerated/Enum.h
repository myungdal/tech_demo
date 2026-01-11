// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


enum class Result : int64_t
{
	SUCCEEDED = 0,
	APP_JOIN_RETRY_LATER = 1, // 서버간 통신
	APP_JOIN_COUNT_OVERFLOW = 2, // 서버간 통신
	OUT_OF_SIZE = 3, // 서버간 통신
	NEED_HANDSHAKE = 4, // 서버간 통신
	HANDLING_PACKET_FAILED = 5, // 서버간 통신
	NETWORK_DISCONNECTED = 6,
	RETRY_LATER = 7,
	TOO_MANY_CONNECTIONS = 8,
	LOW_NETWORK_SPEED = 9,
	INVALID_REQ = 10,
	INVALID_UUID = 11,
	WRONG_NAME = 12,
	NAME_DUPLICATED = 13,
	AUTH_ERROR = 14,
	DB_ERROR = 15,
	DB_ERROR_SP1 = 16,
	DB_ERROR_SP2 = 17,
	DB_ERROR_SP3 = 18,
	DB_ERROR_SP4 = 19,
	DB_ERROR_SP5 = 20,
	DB_ERROR_SP6 = 21,
	DB_ERROR_SP7 = 22,
	DB_ERROR_SP8 = 23,
	DB_ERROR_SP9 = 24,
	DB_SEQ_ERROR = 25,
	CACHE_ERROR = 26,
	USER_STATUS_ERROR = 27,
	WRONG_VALUE = 28,
	NOT_ENOUGH_ITEM = 29,
	NOT_ENOUGH_LEVEL = 30,
	WRONG_APP_VERSION = 31,
	FATAL_ERROR = 32,
	MAX = 33
};

COMMON_API const wchar_t* GetResultDisplayKey(Result result);
COMMON_API const wchar_t* GetResultProgramKey0(Result result);
COMMON_API const wchar_t* GetResultProgramKey1(Result result);
COMMON_API const wchar_t* GetResultProgramKey2(Result result);
COMMON_API const wchar_t* GetResultString(Result result);
COMMON_API Result GetResultEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<Result, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const Result t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetResultString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class WorldUserCommandType : int64_t
{
	NONE = 0,
	KEY_DOWN = 1, // 키 다운
	KEY_UP = 2, // 키 업
	TEST = 3, // 테스트
	ACTION = 4, // 행동
	MOVE = 5, // 이동
	ATTACK = 6, // 공격
	PATH = 7, // 장거리
	MAX = 8
};

COMMON_API const wchar_t* GetWorldUserCommandTypeDisplayKey(WorldUserCommandType worldUserCommandType);
COMMON_API const wchar_t* GetWorldUserCommandTypeProgramKey0(WorldUserCommandType worldUserCommandType);
COMMON_API const wchar_t* GetWorldUserCommandTypeProgramKey1(WorldUserCommandType worldUserCommandType);
COMMON_API const wchar_t* GetWorldUserCommandTypeProgramKey2(WorldUserCommandType worldUserCommandType);
COMMON_API const wchar_t* GetWorldUserCommandTypeString(WorldUserCommandType worldUserCommandType);
COMMON_API WorldUserCommandType GetWorldUserCommandTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<WorldUserCommandType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const WorldUserCommandType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetWorldUserCommandTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class WorldUserCommandState : int64_t
{
	INIT = 0, // 새로운 명령
	ACTIVE = 1, // 반복중
	COMPLETED = 2 // 수행 완료
};

COMMON_API const wchar_t* GetWorldUserCommandStateDisplayKey(WorldUserCommandState worldUserCommandState);
COMMON_API const wchar_t* GetWorldUserCommandStateProgramKey0(WorldUserCommandState worldUserCommandState);
COMMON_API const wchar_t* GetWorldUserCommandStateProgramKey1(WorldUserCommandState worldUserCommandState);
COMMON_API const wchar_t* GetWorldUserCommandStateProgramKey2(WorldUserCommandState worldUserCommandState);
COMMON_API const wchar_t* GetWorldUserCommandStateString(WorldUserCommandState worldUserCommandState);
COMMON_API WorldUserCommandState GetWorldUserCommandStateEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<WorldUserCommandState, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const WorldUserCommandState t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetWorldUserCommandStateString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class MapKind : int64_t
{
	DUNGEON = 0,
	FIELD = 1,
	MAX = 2,
	NONE = MAX
};

COMMON_API const wchar_t* GetMapKindDisplayKey(MapKind mapKind);
COMMON_API const wchar_t* GetMapKindProgramKey0(MapKind mapKind);
COMMON_API const wchar_t* GetMapKindProgramKey1(MapKind mapKind);
COMMON_API const wchar_t* GetMapKindProgramKey2(MapKind mapKind);
COMMON_API const wchar_t* GetMapKindString(MapKind mapKind);
COMMON_API MapKind GetMapKindEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<MapKind, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const MapKind t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetMapKindString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class CharacterType : int64_t
{
	ROCK = 0,
	BOMB = 1,
	PROJECTILE = 2,
	WARRIOR = 3,
	ARCHER = 4,
	ASSASSIN = 5,
	BAT = 6,
	BEAR = 7,
	SPIDER = 8,
	ZEUS = 9,
	HADES = 10,
	CUPID = 11,
	GOLEM = 12,
	PHOENIX = 13,
	DRAGON = 14,
	MAX = 15,
	NONE = MAX
};

COMMON_API const wchar_t* GetCharacterTypeDisplayKey(CharacterType characterType);
COMMON_API const wchar_t* GetCharacterTypeProgramKey0(CharacterType characterType);
COMMON_API const wchar_t* GetCharacterTypeProgramKey1(CharacterType characterType);
COMMON_API const wchar_t* GetCharacterTypeProgramKey2(CharacterType characterType);
COMMON_API const wchar_t* GetCharacterTypeString(CharacterType characterType);
COMMON_API CharacterType GetCharacterTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<CharacterType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const CharacterType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetCharacterTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class TacticsType : int64_t
{
	AVOIDANT = 0,
	DEFENSIVE = 1,
	OFFENSIVE = 2,
	MAX = 3,
	NONE = MAX
};

COMMON_API const wchar_t* GetTacticsTypeDisplayKey(TacticsType tacticsType);
COMMON_API const wchar_t* GetTacticsTypeProgramKey0(TacticsType tacticsType);
COMMON_API const wchar_t* GetTacticsTypeProgramKey1(TacticsType tacticsType);
COMMON_API const wchar_t* GetTacticsTypeProgramKey2(TacticsType tacticsType);
COMMON_API const wchar_t* GetTacticsTypeString(TacticsType tacticsType);
COMMON_API TacticsType GetTacticsTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<TacticsType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const TacticsType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetTacticsTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class GridMoverType : int64_t
{
	WALKER = 0,
	FLYER = 1,
	SWIMMER = 2,
	UNDERWALKER = 3,
	ONLY_SWIMMER = 4,
	ONLY_UNDERWALKER = 5,
	MAX = 6,
	NONE = MAX
};

COMMON_API const wchar_t* GetGridMoverTypeDisplayKey(GridMoverType gridMoverType);
COMMON_API const wchar_t* GetGridMoverTypeProgramKey0(GridMoverType gridMoverType);
COMMON_API const wchar_t* GetGridMoverTypeProgramKey1(GridMoverType gridMoverType);
COMMON_API const wchar_t* GetGridMoverTypeProgramKey2(GridMoverType gridMoverType);
COMMON_API const wchar_t* GetGridMoverTypeString(GridMoverType gridMoverType);
COMMON_API GridMoverType GetGridMoverTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<GridMoverType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const GridMoverType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetGridMoverTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class GridCollisionBit : int64_t
{
	NONE = 0,
	CAN_WALK = 1,
	CAN_FLY = 2,
	CAN_SWIM = 4,
	CAN_BURROW = 8
};

COMMON_API const wchar_t* GetGridCollisionBitDisplayKey(GridCollisionBit gridCollisionBit);
COMMON_API const wchar_t* GetGridCollisionBitProgramKey0(GridCollisionBit gridCollisionBit);
COMMON_API const wchar_t* GetGridCollisionBitProgramKey1(GridCollisionBit gridCollisionBit);
COMMON_API const wchar_t* GetGridCollisionBitProgramKey2(GridCollisionBit gridCollisionBit);
COMMON_API const wchar_t* GetGridCollisionBitString(GridCollisionBit gridCollisionBit);
COMMON_API GridCollisionBit GetGridCollisionBitEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<GridCollisionBit, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const GridCollisionBit t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetGridCollisionBitString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class GridCellBlockType : int64_t
{
	GROUND = 0,
	HILL = 1,
	WATER = 2,
	TUNNEL = 3,
	WALL = 4,
	MAX = 5,
	NONE = MAX
};

COMMON_API const wchar_t* GetGridCellBlockTypeDisplayKey(GridCellBlockType gridCellBlockType);
COMMON_API const wchar_t* GetGridCellBlockTypeProgramKey0(GridCellBlockType gridCellBlockType);
COMMON_API const wchar_t* GetGridCellBlockTypeProgramKey1(GridCellBlockType gridCellBlockType);
COMMON_API const wchar_t* GetGridCellBlockTypeProgramKey2(GridCellBlockType gridCellBlockType);
COMMON_API const wchar_t* GetGridCellBlockTypeString(GridCellBlockType gridCellBlockType);
COMMON_API GridCellBlockType GetGridCellBlockTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<GridCellBlockType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const GridCellBlockType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetGridCellBlockTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class BuildingType : int64_t
{
	CASTLE = 0,
	MAX = 1,
	NONE = MAX
};

COMMON_API const wchar_t* GetBuildingTypeDisplayKey(BuildingType buildingType);
COMMON_API const wchar_t* GetBuildingTypeProgramKey0(BuildingType buildingType);
COMMON_API const wchar_t* GetBuildingTypeProgramKey1(BuildingType buildingType);
COMMON_API const wchar_t* GetBuildingTypeProgramKey2(BuildingType buildingType);
COMMON_API const wchar_t* GetBuildingTypeString(BuildingType buildingType);
COMMON_API BuildingType GetBuildingTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<BuildingType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const BuildingType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetBuildingTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class CharacterColor : int64_t
{
	RED = 0,
	BLUE = 1,
	MAX = 2
};

COMMON_API const wchar_t* GetCharacterColorDisplayKey(CharacterColor characterColor);
COMMON_API const wchar_t* GetCharacterColorProgramKey0(CharacterColor characterColor);
COMMON_API const wchar_t* GetCharacterColorProgramKey1(CharacterColor characterColor);
COMMON_API const wchar_t* GetCharacterColorProgramKey2(CharacterColor characterColor);
COMMON_API const wchar_t* GetCharacterColorString(CharacterColor characterColor);
COMMON_API CharacterColor GetCharacterColorEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<CharacterColor, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const CharacterColor t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetCharacterColorString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class SystemType : int64_t
{
	SEARCH_SYSTEM = 0, // 위치를 기반으로 검색
	GRID_SYSTEM_BLOCK = 1, // 블록
	GRID_SYSTEM_UNBLOCK = 2, // 언블록
	DETECTION_SYSTEM_ENEMY = 3, // 감지
	DETECTION_SYSTEM_FLOCK = 4, // 감지
	DETECTION_SYSTEM_PARTNER = 5, // 감지
	PHYSICS_SYSTEM_INIT = 6,
	CHARACTER_SYSTEM = 7, // 캐릭터
	PC_SYSTEM = 8,
	NPC_SYSTEM = 9,
	RECOVERY_SYSTEM = 10, // 회복
	DAMAGE_SYSTEM = 11, // 데미지
	FORMATION_SYSTEM_FLOCK = 12,
	FORMATION_SYSTEM_CROWD = 13,
	FORMATION_SYSTEM_CROWD_ENEMY = 14,
	TACTICS_SYSTEM_COMBAT = 15, // 전투 상태 – 적을 추격하며, 체력이 50% 이하가 되면 도망 상태로. 적이 없거나 영역 밖으로 나가면 회귀 상태로 전환.
	TACTICS_SYSTEM_RETURN = 16, // 회귀 상태 – 거점으로 돌아가는 중. 쿨다운이 완료되고 적이 있으면 전투 상태로 전환.
	TACTICS_SYSTEM_IDLE = 17, // 대기 상태 – 가만히 있음. 적을 만나면 전투 상태로 전환, 시간이 초과되면 탐색 상태로 전환.
	TACTICS_SYSTEM_ROAMING = 18, // 탐색 상태 – 거점 주변을 탐색. 도착 시 IDLE 상태로, 적을 만나면 전투 상태로 전환.
	TACTICS_SYSTEM_FLEE = 19, // 도망 상태 – 적 반대 방향으로 이동하며 회복. 체력이 15% 이하로 떨어지면 회귀 상태로, 75% 이상이면 전투 상태로.
	PHYSICS_SYSTEM_VELOCITY_AIR = 20,
	PHYSICS_SYSTEM_VELOCITY_GROUND = 21,
	PHYSICS_SYSTEM_MOVEMENT_AIR = 22,
	PHYSICS_SYSTEM_MOVEMENT_GROUND = 23,
	SKILL_SYSTEM_AXE = 24, // 스킬
	SKILL_SYSTEM_BAZOOKA = 25,
	SKILL_SYSTEM_BLOCKING = 26,
	SKILL_SYSTEM_BOMB = 27,
	SKILL_SYSTEM_BOW = 28,
	SKILL_SYSTEM_DASH = 29,
	SKILL_SYSTEM_BLINK = 30, // 피하면서 나를 공격한 사람 뒤로 이동하여 일격
	SKILL_SYSTEM_ELECTRIC = 31,
	SKILL_SYSTEM_HAMMER = 32,
	SKILL_SYSTEM_HEAL = 33,
	SKILL_SYSTEM_LASER = 34,
	SKILL_SYSTEM_PARASITE = 35,
	SKILL_SYSTEM_POISON = 36,
	SKILL_SYSTEM_SPEAR = 37, // 관통 공격
	SKILL_SYSTEM_SWORD = 38,
	SKILL_SYSTEM_VAMPIRE = 39,
	SKILL_SYSTEM_VIRUS = 40,
	SKILL_SYSTEM_WEB = 41,
	SKILL_SYSTEM_PROJECTILE = 42,
	MOTION_SYSTEM_BLEEDING = 43, // 모션
	MOTION_SYSTEM_DYING = 44,
	MOTION_SYSTEM_JUGGLE = 45,
	MOTION_SYSTEM_SHOCK = 46,
	MOTION_SYSTEM_STUN = 47,
	MOTION_SYSTEM_WEB = 48,
	MOTION_SYSTEM_SKILL_A = 49,
	MOTION_SYSTEM_SKILL_B = 50,
	MOTION_SYSTEM_DAMAGE = 51,
	MOTION_SYSTEM_DEAD = 52,
	MOTION_SYSTEM_DASH = 53,
	MOTION_SYSTEM_ARRIVAL = 54,
	MOTION_SYSTEM_DEPARTURE = 55,
	DEBUFF_SYSTEM_EXPLOSION = 56, // 디버프
	DEBUFF_SYSTEM_HEALING = 57,
	DEBUFF_SYSTEM_PARASITE = 58,
	DEBUFF_SYSTEM_POISON = 59,
	DEBUFF_SYSTEM_VIRUS = 60,
	BUFF_SYSTEM_HEAL = 61, // 버프
	SPAWNEE_SYSTEM = 62, // 스포니
	SPAWNER_SYSTEM = 63, // 스포너
	GRID_SYSTEM_BLOCK_ON_DEAD = 64, // 죽을 때 블록
	GRID_SYSTEM_UNBLOCK_ON_DEAD = 65, // 죽을 때 언블록
	MAX = 66,
	NONE = MAX
};

COMMON_API const wchar_t* GetSystemTypeDisplayKey(SystemType systemType);
COMMON_API const wchar_t* GetSystemTypeProgramKey0(SystemType systemType);
COMMON_API const wchar_t* GetSystemTypeProgramKey1(SystemType systemType);
COMMON_API const wchar_t* GetSystemTypeProgramKey2(SystemType systemType);
COMMON_API const wchar_t* GetSystemTypeString(SystemType systemType);
COMMON_API SystemType GetSystemTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<SystemType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const SystemType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetSystemTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ComponentType : int64_t
{
	ABILITY_COMPONENT_GRID_BLOCK = 0, // 능력 컴포넌트
	ABILITY_COMPONENT_GRID_UNBLOCK = 1,
	ABILITY_COMPONENT_GRID_APPLIED = 2,
	ABILITY_COMPONENT_RECOVERY = 3,
	ABILITY_COMPONENT_UNTARGETABLE = 4,
	FORMATION_COMPONENT_GROUP = 5,
	FORMATION_COMPONENT_FLOCK = 6,
	FORMATION_COMPONENT_CROWD = 7,
	MOTION_COMPONENT_GROUP_A = 8, // [그룹] 모션
	MOTION_COMPONENT_SKILL_A = 9,
	MOTION_COMPONENT_BLEEDING = 10,
	MOTION_COMPONENT_JUGGLE = 11,
	MOTION_COMPONENT_DASH = 12,
	MOTION_COMPONENT_WEB = 13,
	MOTION_COMPONENT_DAMAGE = 14,
	MOTION_COMPONENT_GROUP_B = 15,
	MOTION_COMPONENT_SKILL_B = 16,
	MOTION_COMPONENT_SHOCK = 17,
	MOTION_COMPONENT_STUN = 18,
	MOTION_COMPONENT_GROUP_C = 19,
	MOTION_COMPONENT_DYING = 20,
	MOTION_COMPONENT_DEAD = 21,
	MOTION_COMPONENT_DEPARTURE = 22,
	MOTION_COMPONENT_ARRIVAL = 23,
	BUFF_COMPONENT_GROUP = 24, // [그룹] 버프
	BUFF_COMPONENT_HEAL = 25,
	CHARACTER_COMPONENT = 26, // 캐릭터
	PC_COMPONENT = 27,
	LEADER_COMPONENT = 28,
	FOLLOWER_COMPONENT = 29,
	NPC_COMPONENT = 30,
	DAMAGE_COMPONENT = 31, // 대미지
	DEBUFF_COMPONENT_GROUP = 32, // [그룹] 디버프
	DEBUFF_COMPONENT_EXPLOSION = 33,
	DEBUFF_COMPONENT_HEALING = 34,
	DEBUFF_COMPONENT_PARASITE = 35,
	DEBUFF_COMPONENT_POISON = 36,
	DEBUFF_COMPONENT_VIRUS = 37,
	DETECTION_COMPONENT_ENEMY = 38,
	DETECTION_COMPONENT_FLOCK = 39,
	DETECTION_COMPONENT_PARTNER = 40,
	IMPULSE_COMPONENT = 41, // 충격
	PHYSICS_COMPONENT = 42, // 물리
	POSITION_COMPONENT = 43, // 위치
	SKILL_COMPONENT_AXE = 44,
	SKILL_COMPONENT_BOW = 45,
	SKILL_COMPONENT_SPEAR = 46,
	SKILL_COMPONENT_BAZOOKA = 47,
	SKILL_COMPONENT_BOMB = 48,
	SKILL_COMPONENT_HAMMER = 49,
	SKILL_COMPONENT_HEAL = 50,
	SKILL_COMPONENT_PARASITE = 51,
	SKILL_COMPONENT_POISON = 52,
	SKILL_COMPONENT_WEB = 53,
	SKILL_COMPONENT_SPAWN = 54,
	SKILL_COMPONENT_BLOCKING = 55,
	SKILL_COMPONENT_LASER = 56,
	SKILL_COMPONENT_SWORD = 57,
	SKILL_COMPONENT_DASH = 58,
	SKILL_COMPONENT_ELECTRIC = 59,
	SKILL_COMPONENT_VIRUS = 60,
	SKILL_COMPONENT_VAMPIRE = 61,
	SKILL_COMPONENT_BLINK = 62,
	SKILL_COMPONENT_PROJECTILE = 63,
	SPAWNEE_COMPONENT = 64, // 스폰
	SPAWNER_COMPONENT = 65,
	TACTICS_COMPONENT_GROUP = 66, // [그룹] 상태
	TACTICS_COMPONENT_COMBAT = 67,
	TACTICS_COMPONENT_RETURN = 68,
	TACTICS_COMPONENT_IDLE = 69,
	TACTICS_COMPONENT_ROAMING = 70,
	TACTICS_COMPONENT_FLEE = 71,
	MAX = 72,
	NONE = MAX
};

COMMON_API const wchar_t* GetComponentTypeDisplayKey(ComponentType componentType);
COMMON_API const wchar_t* GetComponentTypeProgramKey0(ComponentType componentType);
COMMON_API const wchar_t* GetComponentTypeProgramKey1(ComponentType componentType);
COMMON_API const wchar_t* GetComponentTypeProgramKey2(ComponentType componentType);
COMMON_API const wchar_t* GetComponentTypeString(ComponentType componentType);
COMMON_API ComponentType GetComponentTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ComponentType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ComponentType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetComponentTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class StatType : int64_t
{
	STR = 0, // Strength (힘): 물리 공격력 및 무게를 다루는 능력
	DEX = 1, // Dexterity (민첩): 회피율, 명중률, 속도에 영향을 미치는 능력
	INT = 2, // Intelligence (지능): 마법 공격력 및 마나 활용 능력
	WIS = 3, // Wisdom (지혜): 마법 저항력 및 마법 치유 능력
	CON = 4, // Constitution (체력): 체력, 내구력 및 생명력 회복력
	AGI = 5, // Agility (기민함): 행동 순서 및 이동 속도에 영향을 미침
	END = 6, // Endurance (지구력): 상태 이상 저항 및 전투 지속력
	HP = 7,
	SP = 8,
	HP_MAX_MULTIPLIER = 9, // 최대 체력에 대한 증가 배율
	HP_MAX = 10, // 최대 체력
	HP_RECOVERY_MULTIPLIER = 11, // 체력 회복 속도 증가 배율
	HP_RECOVERY = 12, // 체력 회복 속도
	SP_MAX_MULTIPLIER = 13, // 최대 스태미너 증가 배율
	SP_MAX = 14, // 최대 스태미너
	SP_RECOVERY_MULTIPLIER = 15, // 스태미너 회복 속도 증가 배율
	SP_RECOVERY = 16, // 스태미너 회복 속도
	SHIELD_MAX_MULTIPLIER = 17, // 최대 실드(보호막) 수치 증가 배율: 실드의 상한값 증가 배율
	SHIELD_MAX = 18, // 최대 실드(보호막) 수치: 실드의 상한값.
	LUCK = 19, // 행운: 치명타 확률, 아이템 드랍률 등에 영향을 미침
	MOVE_SPEED_MULTIPLIER = 20, // 이동 속도 증가 배율
	MOVE_SPEED = 21, // 이동 속도
	SKILL_COOLDOWN_MULTIPLIER = 22, // 공격 속도 증가 배율
	DETECT_DISTANCE_MULTIFLIER = 23, // 탐지 거리 증가 배율
	SKILL_DISTANCE_MULTIFLIER = 24, // 공격 거리 증가 배율
	SPLASH_DISTANCE_MULTIFLIER = 25, // 범위 데미지 거리 증가 배율
	SPLASH_DAMAGE_MULTIFLIER = 26, // 범위 데미지 증가 배율
	ATTACK_MULTIPLIER = 27, // 모든 공격력 증가 배율
	ATTACK = 28, // 모든 공격력
	ATTACK_MELEE_MULTIPLIER = 29, // 근접 공격력 증가 배율
	ATTACK_MELEE = 30, // 근접 공격력
	ATTACK_RANGED_MULTIPLIER = 31, // 원거리 공격력 증가 배율
	ATTACK_RANGED = 32, // 원거리 공격력
	ATTACK_PHYSIC_MULTIPLIER = 33, // 물리 공격력 증가 배율
	ATTACK_PHYSIC = 34, // 물리 공격력
	ATTACK_MAGIC_MULTIPLIER = 35, // 마법 공격력 증가 배율
	ATTACK_MAGIC = 36, // 마법 공격력
	DEFENSE_MULTIPLIER = 37, // 모든 방어력 증가 배율
	DEFENSE = 38, // 모든 방어력
	DEFENSE_MELEE_MULTIPLIER = 39, // 근접 방어력 증가 배율
	DEFENSE_MELEE = 40, // 근접 방어력
	DEFENSE_RANGED_MULTIPLIER = 41, // 원거리 방어력 증가 배율
	DEFENSE_RANGED = 42, // 원거리 방어력
	DEFENSE_PHYSIC_MULTIPLIER = 43, // 물리 방어력 증가 배율
	DEFENSE_PHYSIC = 44, // 물리 방어력
	DEFENSE_MAGIC_MULTIPLIER = 45, // 마법 방어력 증가 배율
	DEFENSE_MAGIC = 46, // 마법 방어력
	HIT_MULTIPLIER = 47, // 명중률 증가 배율
	HIT = 48, // 명중률
	DODGE_MULTIPLIER = 49, // 회피율 증가 배율
	DODGE = 50, // 회피율
	DAMAGE_MULTIPLIER = 51, // 기본 데미지 증가 배율
	DAMAGE = 52, // 기본 데미지
	REDUNTION_MULTIPLIER = 53, // 피해 감소율 증가 배율
	REDUNTION = 54, // 피해 감소율
	CRITICAL_HIT = 55, // 치명타 적중률
	CRITICAL_DODGE = 56, // 치명타 회피율
	CRITICAL_MULTIPLIER = 57, // 치명타 효과 배율
	CRITICAL_RESISTANCE = 58, // 치명타 저항력
	DEBUFF_HIT = 59, // 디버프 적중률
	DEBUFF_DODGE = 60, // 디버프 회피율
	DEBUFF_MULTIFLIER = 61, // 디버프 효과 증폭
	DEBUFF_RESISTANCE = 62, // 디버프 저항력
	BUFF_APPLY_MULTIFLIER = 63, // 버프 적용력 증폭
	BUFF_RECEIVE_MULTIFLIER = 64, // 버프 수신력 증폭
	EXP_MULTIFLIER = 65, // 경험치 획득량 증가 배율
	GOLD_MULTIPLIER = 66, // 골드 획득량 증가 배율
	MAX = 67,
	NONE = MAX
};

COMMON_API const wchar_t* GetStatTypeDisplayKey(StatType statType);
COMMON_API const wchar_t* GetStatTypeProgramKey0(StatType statType);
COMMON_API const wchar_t* GetStatTypeProgramKey1(StatType statType);
COMMON_API const wchar_t* GetStatTypeProgramKey2(StatType statType);
COMMON_API const wchar_t* GetStatTypeString(StatType statType);
COMMON_API StatType GetStatTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<StatType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const StatType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetStatTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class LanguageType : int64_t
{
	EN = 0,
	KO = 1,
	JA = 2,
	ZH_CHS = 3,
	ZH_CHT = 4,
	DE = 5,
	FI = 6,
	MAX = 7,
	NONE = MAX
};

COMMON_API const wchar_t* GetLanguageTypeDisplayKey(LanguageType languageType);
COMMON_API const wchar_t* GetLanguageTypeProgramKey0(LanguageType languageType);
COMMON_API const wchar_t* GetLanguageTypeProgramKey1(LanguageType languageType);
COMMON_API const wchar_t* GetLanguageTypeProgramKey2(LanguageType languageType);
COMMON_API const wchar_t* GetLanguageTypeString(LanguageType languageType);
COMMON_API LanguageType GetLanguageTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<LanguageType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const LanguageType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetLanguageTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class CoefficientType : int64_t
{
	AAA = 0,
	BBB = 1,
	MAX = 2,
	NONE = MAX
};

COMMON_API const wchar_t* GetCoefficientTypeDisplayKey(CoefficientType coefficientType);
COMMON_API const wchar_t* GetCoefficientTypeProgramKey0(CoefficientType coefficientType);
COMMON_API const wchar_t* GetCoefficientTypeProgramKey1(CoefficientType coefficientType);
COMMON_API const wchar_t* GetCoefficientTypeProgramKey2(CoefficientType coefficientType);
COMMON_API const wchar_t* GetCoefficientTypeString(CoefficientType coefficientType);
COMMON_API CoefficientType GetCoefficientTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<CoefficientType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const CoefficientType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetCoefficientTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class KickReason : int64_t
{
	MAINTENANCE = 0,
	MAX = 1,
	NONE = MAX
};

COMMON_API const wchar_t* GetKickReasonDisplayKey(KickReason kickReason);
COMMON_API const wchar_t* GetKickReasonProgramKey0(KickReason kickReason);
COMMON_API const wchar_t* GetKickReasonProgramKey1(KickReason kickReason);
COMMON_API const wchar_t* GetKickReasonProgramKey2(KickReason kickReason);
COMMON_API const wchar_t* GetKickReasonString(KickReason kickReason);
COMMON_API KickReason GetKickReasonEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<KickReason, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const KickReason t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetKickReasonString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class NotifyReason : int64_t
{
	MAINTAINANCE = 0,
	MAX = 1,
	NONE = MAX
};

COMMON_API const wchar_t* GetNotifyReasonDisplayKey(NotifyReason notifyReason);
COMMON_API const wchar_t* GetNotifyReasonProgramKey0(NotifyReason notifyReason);
COMMON_API const wchar_t* GetNotifyReasonProgramKey1(NotifyReason notifyReason);
COMMON_API const wchar_t* GetNotifyReasonProgramKey2(NotifyReason notifyReason);
COMMON_API const wchar_t* GetNotifyReasonString(NotifyReason notifyReason);
COMMON_API NotifyReason GetNotifyReasonEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<NotifyReason, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const NotifyReason t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetNotifyReasonString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class RoomType : int64_t
{
	Guild = 0,
	Party = 1,
	Channel = 2,
	Server = 3,
	MAX = 4,
	NONE = MAX
};

COMMON_API const wchar_t* GetRoomTypeDisplayKey(RoomType roomType);
COMMON_API const wchar_t* GetRoomTypeProgramKey0(RoomType roomType);
COMMON_API const wchar_t* GetRoomTypeProgramKey1(RoomType roomType);
COMMON_API const wchar_t* GetRoomTypeProgramKey2(RoomType roomType);
COMMON_API const wchar_t* GetRoomTypeString(RoomType roomType);
COMMON_API RoomType GetRoomTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<RoomType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const RoomType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetRoomTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ServerState : int64_t
{
	OPEN = 0,
	RECOMMENDED = 1,
	BUSY = 2,
	FULL = 3,
	MAINTENANCE = 4,
	CLOSED = 5
};

COMMON_API const wchar_t* GetServerStateDisplayKey(ServerState serverState);
COMMON_API const wchar_t* GetServerStateProgramKey0(ServerState serverState);
COMMON_API const wchar_t* GetServerStateProgramKey1(ServerState serverState);
COMMON_API const wchar_t* GetServerStateProgramKey2(ServerState serverState);
COMMON_API const wchar_t* GetServerStateString(ServerState serverState);
COMMON_API ServerState GetServerStateEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ServerState, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ServerState t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetServerStateString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class AccountState : int64_t
{
	NORMAL = 0,
	BANNED = 1,
	MAX = 2,
	NONE = MAX
};

COMMON_API const wchar_t* GetAccountStateDisplayKey(AccountState accountState);
COMMON_API const wchar_t* GetAccountStateProgramKey0(AccountState accountState);
COMMON_API const wchar_t* GetAccountStateProgramKey1(AccountState accountState);
COMMON_API const wchar_t* GetAccountStateProgramKey2(AccountState accountState);
COMMON_API const wchar_t* GetAccountStateString(AccountState accountState);
COMMON_API AccountState GetAccountStateEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<AccountState, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const AccountState t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetAccountStateString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class UserState : int64_t
{
	NORMAL = 0,
	UNDER_INSPECTION = 1,
	BANNED = 2,
	QA = 3,
	GM = 4,
	ADMIN = 5,
	MAX = 6,
	NONE = MAX
};

COMMON_API const wchar_t* GetUserStateDisplayKey(UserState userState);
COMMON_API const wchar_t* GetUserStateProgramKey0(UserState userState);
COMMON_API const wchar_t* GetUserStateProgramKey1(UserState userState);
COMMON_API const wchar_t* GetUserStateProgramKey2(UserState userState);
COMMON_API const wchar_t* GetUserStateString(UserState userState);
COMMON_API UserState GetUserStateEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<UserState, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const UserState t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetUserStateString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class UserType : int64_t
{
	NORMAL = 0,
	GM = 1,
	ADMIN = 2,
	MAX = 3,
	NONE = MAX
};

COMMON_API const wchar_t* GetUserTypeDisplayKey(UserType userType);
COMMON_API const wchar_t* GetUserTypeProgramKey0(UserType userType);
COMMON_API const wchar_t* GetUserTypeProgramKey1(UserType userType);
COMMON_API const wchar_t* GetUserTypeProgramKey2(UserType userType);
COMMON_API const wchar_t* GetUserTypeString(UserType userType);
COMMON_API UserType GetUserTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<UserType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const UserType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetUserTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class PurchaseState : int64_t
{
	VALIDATED = 0, // 구매 가능 여부가 검증됨.
	VERIFIED = 1, // 결제 확인 및 유효성 검사가 완료됨.
	DELIVERED = 2, // 관련 상품이나 서비스가 사용자에게 제공됨.
	REFUNDED = 3, // 결제가 환불됨.
	RECLAIMED = 4, // 물건이나 서비스가 회수됨.
	MAX = 5,
	NONE = MAX
};

COMMON_API const wchar_t* GetPurchaseStateDisplayKey(PurchaseState purchaseState);
COMMON_API const wchar_t* GetPurchaseStateProgramKey0(PurchaseState purchaseState);
COMMON_API const wchar_t* GetPurchaseStateProgramKey1(PurchaseState purchaseState);
COMMON_API const wchar_t* GetPurchaseStateProgramKey2(PurchaseState purchaseState);
COMMON_API const wchar_t* GetPurchaseStateString(PurchaseState purchaseState);
COMMON_API PurchaseState GetPurchaseStateEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<PurchaseState, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const PurchaseState t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetPurchaseStateString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ActivityType : int64_t
{
	ITEM_USE = 0,
	ITEM_INCREASE = 1,
	ITEM_DECREASE = 2,
	CHARACTER_COLLECT = 3,
	BUILDING_UPGRADE = 4,
	QUEST_COMPLETE = 5,
	MISSION_COMPLETE = 6,
	STOCK_BUY = 7,
	PRODUCT_BUY = 8,
	MAX = 9,
	NONE = MAX
};

COMMON_API const wchar_t* GetActivityTypeDisplayKey(ActivityType activityType);
COMMON_API const wchar_t* GetActivityTypeProgramKey0(ActivityType activityType);
COMMON_API const wchar_t* GetActivityTypeProgramKey1(ActivityType activityType);
COMMON_API const wchar_t* GetActivityTypeProgramKey2(ActivityType activityType);
COMMON_API const wchar_t* GetActivityTypeString(ActivityType activityType);
COMMON_API ActivityType GetActivityTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ActivityType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ActivityType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetActivityTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ItemType : int64_t
{
	CURRENCY = 0,
	REWARD_BOX = 1,
	LAUNCH_GAME = 2,
	CHARACTER = 3,
	BUILDING = 4,
	STONE = 5,
	MAX = 6,
	NONE = MAX
};

COMMON_API const wchar_t* GetItemTypeDisplayKey(ItemType itemType);
COMMON_API const wchar_t* GetItemTypeProgramKey0(ItemType itemType);
COMMON_API const wchar_t* GetItemTypeProgramKey1(ItemType itemType);
COMMON_API const wchar_t* GetItemTypeProgramKey2(ItemType itemType);
COMMON_API const wchar_t* GetItemTypeString(ItemType itemType);
COMMON_API ItemType GetItemTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ItemType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ItemType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetItemTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ItemGrade : int64_t
{
	BASIC = 0, // 검은색
	RARE = 1, // 노란색
	EPIC = 2, // 파란색
	LEGENDARY = 3, // 빨간색
	MYTHIC = 4, // 흰색
	MAX = 5,
	NONE = MAX
};

COMMON_API const wchar_t* GetItemGradeDisplayKey(ItemGrade itemGrade);
COMMON_API const wchar_t* GetItemGradeProgramKey0(ItemGrade itemGrade);
COMMON_API const wchar_t* GetItemGradeProgramKey1(ItemGrade itemGrade);
COMMON_API const wchar_t* GetItemGradeProgramKey2(ItemGrade itemGrade);
COMMON_API const wchar_t* GetItemGradeString(ItemGrade itemGrade);
COMMON_API ItemGrade GetItemGradeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ItemGrade, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ItemGrade t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetItemGradeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ItemColor : int64_t
{
	ORANGE = 0, // 오렌지색
	GREEN = 1, // 녹색
	VIOLET = 2, // 보라색
	MAX = 3,
	NONE = MAX
};

COMMON_API const wchar_t* GetItemColorDisplayKey(ItemColor itemColor);
COMMON_API const wchar_t* GetItemColorProgramKey0(ItemColor itemColor);
COMMON_API const wchar_t* GetItemColorProgramKey1(ItemColor itemColor);
COMMON_API const wchar_t* GetItemColorProgramKey2(ItemColor itemColor);
COMMON_API const wchar_t* GetItemColorString(ItemColor itemColor);
COMMON_API ItemColor GetItemColorEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ItemColor, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ItemColor t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetItemColorString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ItemEnchant : int64_t
{
	NORMAL = 0, // 일반
	EXCELLENT = 1, // 탁월
	MAX = 2,
	NONE = MAX
};

COMMON_API const wchar_t* GetItemEnchantDisplayKey(ItemEnchant itemEnchant);
COMMON_API const wchar_t* GetItemEnchantProgramKey0(ItemEnchant itemEnchant);
COMMON_API const wchar_t* GetItemEnchantProgramKey1(ItemEnchant itemEnchant);
COMMON_API const wchar_t* GetItemEnchantProgramKey2(ItemEnchant itemEnchant);
COMMON_API const wchar_t* GetItemEnchantString(ItemEnchant itemEnchant);
COMMON_API ItemEnchant GetItemEnchantEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ItemEnchant, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ItemEnchant t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetItemEnchantString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ItemTag : int64_t
{
	NONE = 0,
	CAN_STORAGE = 1,
	CAN_TRADE = 2,
	CAN_SELL = 4,
	CAN_DELETE = 8
};

COMMON_API const wchar_t* GetItemTagDisplayKey(ItemTag itemTag);
COMMON_API const wchar_t* GetItemTagProgramKey0(ItemTag itemTag);
COMMON_API const wchar_t* GetItemTagProgramKey1(ItemTag itemTag);
COMMON_API const wchar_t* GetItemTagProgramKey2(ItemTag itemTag);
COMMON_API const wchar_t* GetItemTagString(ItemTag itemTag);
COMMON_API ItemTag GetItemTagEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ItemTag, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ItemTag t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetItemTagString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ItemStateFlag : int64_t
{
	NONE = 0,
	LOCKED = 1,
	AUTO_USE = 2
};

COMMON_API const wchar_t* GetItemStateFlagDisplayKey(ItemStateFlag itemStateFlag);
COMMON_API const wchar_t* GetItemStateFlagProgramKey0(ItemStateFlag itemStateFlag);
COMMON_API const wchar_t* GetItemStateFlagProgramKey1(ItemStateFlag itemStateFlag);
COMMON_API const wchar_t* GetItemStateFlagProgramKey2(ItemStateFlag itemStateFlag);
COMMON_API const wchar_t* GetItemStateFlagString(ItemStateFlag itemStateFlag);
COMMON_API ItemStateFlag GetItemStateFlagEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ItemStateFlag, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ItemStateFlag t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetItemStateFlagString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class RewardType : int64_t
{
	ALL = 0, // 모두
	PICK = 1, // 선택
	RATIO = 2, // 비율
	PROBABILITY = 3, // 개별 확률
	MAX = 4,
	NONE = MAX
};

COMMON_API const wchar_t* GetRewardTypeDisplayKey(RewardType rewardType);
COMMON_API const wchar_t* GetRewardTypeProgramKey0(RewardType rewardType);
COMMON_API const wchar_t* GetRewardTypeProgramKey1(RewardType rewardType);
COMMON_API const wchar_t* GetRewardTypeProgramKey2(RewardType rewardType);
COMMON_API const wchar_t* GetRewardTypeString(RewardType rewardType);
COMMON_API RewardType GetRewardTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<RewardType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const RewardType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetRewardTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ChatType : int64_t
{
	ALL = 0,
	GAME = 1,
	MAX = 2,
	NONE = MAX
};

COMMON_API const wchar_t* GetChatTypeDisplayKey(ChatType chatType);
COMMON_API const wchar_t* GetChatTypeProgramKey0(ChatType chatType);
COMMON_API const wchar_t* GetChatTypeProgramKey1(ChatType chatType);
COMMON_API const wchar_t* GetChatTypeProgramKey2(ChatType chatType);
COMMON_API const wchar_t* GetChatTypeString(ChatType chatType);
COMMON_API ChatType GetChatTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ChatType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ChatType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetChatTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class MailType : int64_t
{
	PRODUCT = 0,
	RANK = 1,
	MAX = 2,
	NONE = MAX
};

COMMON_API const wchar_t* GetMailTypeDisplayKey(MailType mailType);
COMMON_API const wchar_t* GetMailTypeProgramKey0(MailType mailType);
COMMON_API const wchar_t* GetMailTypeProgramKey1(MailType mailType);
COMMON_API const wchar_t* GetMailTypeProgramKey2(MailType mailType);
COMMON_API const wchar_t* GetMailTypeString(MailType mailType);
COMMON_API MailType GetMailTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<MailType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const MailType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetMailTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class GameType : int64_t
{
	FIELD = 0,
	CONQUEST = 1, // 정복, 기여도 순위, 막타
	DEFENSE = 2, // 방어, 참여 시간
	HUNTING = 3, // 사냥, 점수
	SURVIVAL = 4, // 각자 생존, 시간 순위
	MAX = 5,
	NONE = MAX
};

COMMON_API const wchar_t* GetGameTypeDisplayKey(GameType gameType);
COMMON_API const wchar_t* GetGameTypeProgramKey0(GameType gameType);
COMMON_API const wchar_t* GetGameTypeProgramKey1(GameType gameType);
COMMON_API const wchar_t* GetGameTypeProgramKey2(GameType gameType);
COMMON_API const wchar_t* GetGameTypeString(GameType gameType);
COMMON_API GameType GetGameTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<GameType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const GameType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetGameTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class GameGrade : int64_t
{
	BASIC = 0,
	RARE = 1,
	EPIC = 2,
	LEGENDARY = 3,
	MYTHIC = 4,
	MAX = 5,
	NONE = MAX
};

COMMON_API const wchar_t* GetGameGradeDisplayKey(GameGrade gameGrade);
COMMON_API const wchar_t* GetGameGradeProgramKey0(GameGrade gameGrade);
COMMON_API const wchar_t* GetGameGradeProgramKey1(GameGrade gameGrade);
COMMON_API const wchar_t* GetGameGradeProgramKey2(GameGrade gameGrade);
COMMON_API const wchar_t* GetGameGradeString(GameGrade gameGrade);
COMMON_API GameGrade GetGameGradeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<GameGrade, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const GameGrade t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetGameGradeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class AppType : int64_t
{
	BRIDGE_SERVER = 0,
	DB_SERVER = 1,
	FRONT_SERVER = 2,
	GAME_SERVER = 3,
	LOG_SERVER = 4,
	MAIN_SERVER = 5,
	SHELL = 6,
	BOT = 7,
	TEST = 8,
	MAX = 9,
	NONE = MAX
};

COMMON_API const wchar_t* GetAppTypeDisplayKey(AppType appType);
COMMON_API const wchar_t* GetAppTypeProgramKey0(AppType appType);
COMMON_API const wchar_t* GetAppTypeProgramKey1(AppType appType);
COMMON_API const wchar_t* GetAppTypeProgramKey2(AppType appType);
COMMON_API const wchar_t* GetAppTypeString(AppType appType);
COMMON_API AppType GetAppTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<AppType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const AppType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetAppTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class DbType : int64_t
{
	STATIC = 0,
	LOG = 1,
	MAIN = 2,
	USER = 3,
	MAX = 4,
	NONE = MAX
};

COMMON_API const wchar_t* GetDbTypeDisplayKey(DbType dbType);
COMMON_API const wchar_t* GetDbTypeProgramKey0(DbType dbType);
COMMON_API const wchar_t* GetDbTypeProgramKey1(DbType dbType);
COMMON_API const wchar_t* GetDbTypeProgramKey2(DbType dbType);
COMMON_API const wchar_t* GetDbTypeString(DbType dbType);
COMMON_API DbType GetDbTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<DbType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const DbType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetDbTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class BotEventType : int64_t
{
	CREATED = 0,
	DESTROYED = 1,
	CONNECT_FRONT = 2,
	CONNECT_GAME = 3,
	DISCONNECT_FRONT = 4,
	DISCONNECT_GAME = 5,
	SEQ_RETRIED = 6,
	CONNECTED_FRONT = 7,
	DISCONNECTED_FRONT = 8,
	CONNECTED_GAME = 9,
	DISCONNECTED_GAME = 10,
	SCENARIO_STARTED = 11,
	SCENARIO_FINISHED = 12,
	SCENE_STARTED = 13,
	SCENE_FINISHED = 14,
	SEQ_STARTED = 15,
	SEQ_FINISHED = 16,
	RECONNECT_SUCCEEDED = 17,
	RELOGIN_SUCCEEDED = 18,
	RECONNECT_FAILED = 19,
	MAX = 20,
	NONE = MAX
};

COMMON_API const wchar_t* GetBotEventTypeDisplayKey(BotEventType botEventType);
COMMON_API const wchar_t* GetBotEventTypeProgramKey0(BotEventType botEventType);
COMMON_API const wchar_t* GetBotEventTypeProgramKey1(BotEventType botEventType);
COMMON_API const wchar_t* GetBotEventTypeProgramKey2(BotEventType botEventType);
COMMON_API const wchar_t* GetBotEventTypeString(BotEventType botEventType);
COMMON_API BotEventType GetBotEventTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<BotEventType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const BotEventType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetBotEventTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class CharacterAnimType : int64_t
{
	IDLE = 0,
	RUN = 1,
	SUMMON = 2,
	DASH = 3,
	HIT = 4,
	DIE = 5,
	TOMBSTONE = 6,
	ATTACK = 7,
	MAX = 8,
	NONE = MAX
};

COMMON_API const wchar_t* GetCharacterAnimTypeDisplayKey(CharacterAnimType characterAnimType);
COMMON_API const wchar_t* GetCharacterAnimTypeProgramKey0(CharacterAnimType characterAnimType);
COMMON_API const wchar_t* GetCharacterAnimTypeProgramKey1(CharacterAnimType characterAnimType);
COMMON_API const wchar_t* GetCharacterAnimTypeProgramKey2(CharacterAnimType characterAnimType);
COMMON_API const wchar_t* GetCharacterAnimTypeString(CharacterAnimType characterAnimType);
COMMON_API CharacterAnimType GetCharacterAnimTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<CharacterAnimType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const CharacterAnimType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetCharacterAnimTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class EffectAnimType : int64_t
{
	FIRE_BLAST = 0,
	LIGHT_BEAM = 1,
	ARC_ENERGY = 2,
	POISON_CLOUD = 3,
	ELECTRIC_SHOCK = 4,
	ICE_SPIKE = 5,
	WIND_SLASH = 6,
	EARTH_CRACK = 7,
	WATER_SPLASH = 8,
	DARK_PULSE = 9,
	HOLY_FLASH = 10,
	STONE_SHIELD = 11,
	BLOOD_STRIKE = 12,
	NATURE_GROWTH = 13,
	SHADOW_STEP = 14,
	METAL_IMPACT = 15,
	SAND_STORM = 16,
	MAX = 17,
	NONE = 18
};

COMMON_API const wchar_t* GetEffectAnimTypeDisplayKey(EffectAnimType effectAnimType);
COMMON_API const wchar_t* GetEffectAnimTypeProgramKey0(EffectAnimType effectAnimType);
COMMON_API const wchar_t* GetEffectAnimTypeProgramKey1(EffectAnimType effectAnimType);
COMMON_API const wchar_t* GetEffectAnimTypeProgramKey2(EffectAnimType effectAnimType);
COMMON_API const wchar_t* GetEffectAnimTypeString(EffectAnimType effectAnimType);
COMMON_API EffectAnimType GetEffectAnimTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<EffectAnimType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const EffectAnimType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetEffectAnimTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class RedDotType : int64_t
{
	MENU = 0,
	INVENTORY = 1,
	SHOP = 2,
	QUEST = 3,
	MAIL = 4,
	MISSION = 5,
	ACHIEVEMENT = 6,
	RANKING = 7,
	MAX = 8,
	NONE = MAX
};

COMMON_API const wchar_t* GetRedDotTypeDisplayKey(RedDotType redDotType);
COMMON_API const wchar_t* GetRedDotTypeProgramKey0(RedDotType redDotType);
COMMON_API const wchar_t* GetRedDotTypeProgramKey1(RedDotType redDotType);
COMMON_API const wchar_t* GetRedDotTypeProgramKey2(RedDotType redDotType);
COMMON_API const wchar_t* GetRedDotTypeString(RedDotType redDotType);
COMMON_API RedDotType GetRedDotTypeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<RedDotType, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const RedDotType t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetRedDotTypeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};

enum class ClientMode : int64_t
{
	GAME = 0,
	TEST = 1,
	MAX = 2,
	NONE = MAX
};

COMMON_API const wchar_t* GetClientModeDisplayKey(ClientMode clientMode);
COMMON_API const wchar_t* GetClientModeProgramKey0(ClientMode clientMode);
COMMON_API const wchar_t* GetClientModeProgramKey1(ClientMode clientMode);
COMMON_API const wchar_t* GetClientModeProgramKey2(ClientMode clientMode);
COMMON_API const wchar_t* GetClientModeString(ClientMode clientMode);
COMMON_API ClientMode GetClientModeEnum(const wchar_t* str);

template<typename _CharType>
struct std::formatter<ClientMode, _CharType>
{
    constexpr auto parse(auto & ctx)
    {
        return ctx.end();
    }
    
    auto format(MAYBE_UNUSED const ClientMode t, auto& ctx) const
    {
        std::wstring result = std::format(L"{}", GetClientModeString(t));

        return std::ranges::copy(result, ctx.out()).out;
    }
};
