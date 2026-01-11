// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_STATIC_ACHIEVEMENT_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_achievement_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_ACHIEVEMENT_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_achievement_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_achievement_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_ACHIEVEMENT_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

	}
};

class SP_STATIC_ACHIEVEMENT_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_achievement_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_ACHIEVEMENT_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_achievement_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_achievement_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_ACHIEVEMENT_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

	}
};

class SP_STATIC_ACHIEVEMENT_STEP_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_achievement_sid = 0; // type: 8, flags: 4097, charset: 63 
           int64_t c_achievement_step = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_ACHIEVEMENT_STEP_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_achievement_step_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_achievement_sid);
		BindOut(mOut.c_achievement_step);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_ACHIEVEMENT_STEP_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_ACHIEVEMENT_STEP_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_achievement_sid = 0; // type: 8, flags: 4097, charset: 63 
           int64_t c_achievement_step = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_ACHIEVEMENT_STEP_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_achievement_step_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOut(mOut.c_achievement_sid);
		BindOut(mOut.c_achievement_step);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_ACHIEVEMENT_STEP_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_BUILDING_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_building_item_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_building_type = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_BUILDING_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_building_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_building_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_building_item_sid);
		BindOutStr(mOut.c_building_type, 51 * sizeof(wchar_t));
	}
	~SP_STATIC_BUILDING_SELECT()
	{
     if (mOut.c_building_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_building_type));

	}
};

class SP_STATIC_BUILDING_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_building_item_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_building_type = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_BUILDING_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_building_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_building_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_building_item_sid);
		BindOutStr(mOut.c_building_type, 51 * sizeof(wchar_t));
	}
	~SP_STATIC_BUILDING_SELECT_ALL()
	{
     if (mOut.c_building_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_building_type));

	}
};

class SP_STATIC_CHARACTER_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_character_item_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_character_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_character_color = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_tactics_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_grid_mover_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_physics_size = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_distance_best_min = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_distance_best_max = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_hp_threshold_flee = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_hp_threshold_combat = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_hp_threshold_last_stand = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_CHARACTER_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_character_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_character_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_character_color = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_tactics_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_grid_mover_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_character_item_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_character_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_character_color, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_tactics_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_grid_mover_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_physics_size);
		BindOut(mOut.c_skill_distance_best_min);
		BindOut(mOut.c_skill_distance_best_max);
		BindOut(mOut.c_hp_threshold_flee);
		BindOut(mOut.c_hp_threshold_combat);
		BindOut(mOut.c_hp_threshold_last_stand);
	}
	~SP_STATIC_CHARACTER_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_character_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_character_type));

     if (mOut.c_character_color)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_character_color));

     if (mOut.c_tactics_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_tactics_type));

     if (mOut.c_grid_mover_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_grid_mover_type));

	}
};

class SP_STATIC_CHARACTER_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_character_item_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_character_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_character_color = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_tactics_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_grid_mover_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_physics_size = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_distance_best_min = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_distance_best_max = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_hp_threshold_flee = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_hp_threshold_combat = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_hp_threshold_last_stand = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_CHARACTER_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_character_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_character_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_character_color = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_tactics_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_grid_mover_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_character_item_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_character_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_character_color, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_tactics_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_grid_mover_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_physics_size);
		BindOut(mOut.c_skill_distance_best_min);
		BindOut(mOut.c_skill_distance_best_max);
		BindOut(mOut.c_hp_threshold_flee);
		BindOut(mOut.c_hp_threshold_combat);
		BindOut(mOut.c_hp_threshold_last_stand);
	}
	~SP_STATIC_CHARACTER_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_character_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_character_type));

     if (mOut.c_character_color)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_character_color));

     if (mOut.c_tactics_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_tactics_type));

     if (mOut.c_grid_mover_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_grid_mover_type));

	}
};

class SP_STATIC_CHARACTER_SKILL_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_character_item_sid = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_skill_component_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_skill_cost_sp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_cooltime = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_distance_min = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_distance_max = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_motion_duration = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_power = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_CHARACTER_SKILL_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_character_skill_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_skill_component_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_character_item_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_skill_component_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_skill_cost_sp);
		BindOut(mOut.c_skill_cooltime);
		BindOut(mOut.c_skill_distance_min);
		BindOut(mOut.c_skill_distance_max);
		BindOut(mOut.c_skill_motion_duration);
		BindOut(mOut.c_skill_power);
	}
	~SP_STATIC_CHARACTER_SKILL_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_skill_component_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_skill_component_type));

	}
};

class SP_STATIC_CHARACTER_SKILL_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_character_item_sid = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_skill_component_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_skill_cost_sp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_cooltime = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_distance_min = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_distance_max = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_motion_duration = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_skill_power = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_CHARACTER_SKILL_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_character_skill_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_skill_component_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOut(mOut.c_character_item_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_skill_component_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_skill_cost_sp);
		BindOut(mOut.c_skill_cooltime);
		BindOut(mOut.c_skill_distance_min);
		BindOut(mOut.c_skill_distance_max);
		BindOut(mOut.c_skill_motion_duration);
		BindOut(mOut.c_skill_power);
	}
	~SP_STATIC_CHARACTER_SKILL_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_skill_component_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_skill_component_type));

	}
};

class SP_STATIC_GAME_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_game_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_game_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_map_sid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_GAME_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_game_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_game_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_game_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_game_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_map_sid);
	}
	~SP_STATIC_GAME_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_game_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_game_type));

	}
};

class SP_STATIC_GAME_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_game_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_game_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_map_sid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_GAME_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_game_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_game_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_game_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_game_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_map_sid);
	}
	~SP_STATIC_GAME_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_game_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_game_type));

	}
};

class SP_STATIC_ITEM_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_item_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_item_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_item_grade = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_max_quantity = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_tag = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_use_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_use_action_script = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_uasset_icon_img = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_ITEM_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_item_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_item_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_item_grade = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_use_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_use_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_item_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_item_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_item_grade, 51 * sizeof(wchar_t));
		BindOut(mOut.c_max_quantity);
		BindOut(mOut.c_item_tag);
		BindOutStr(mOut.c_use_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_use_action_script, 251 * sizeof(wchar_t));
		BindOut(mOut.c_uasset_icon_img);
	}
	~SP_STATIC_ITEM_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_item_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_item_type));

     if (mOut.c_item_grade)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_item_grade));

     if (mOut.c_use_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_use_condition_script));

     if (mOut.c_use_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_use_action_script));

	}
};

class SP_STATIC_ITEM_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_item_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_item_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_item_grade = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_max_quantity = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_tag = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_use_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_use_action_script = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_uasset_icon_img = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_ITEM_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_item_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_item_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_item_grade = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_use_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_use_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_item_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_item_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_item_grade, 51 * sizeof(wchar_t));
		BindOut(mOut.c_max_quantity);
		BindOut(mOut.c_item_tag);
		BindOutStr(mOut.c_use_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_use_action_script, 251 * sizeof(wchar_t));
		BindOut(mOut.c_uasset_icon_img);
	}
	~SP_STATIC_ITEM_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_item_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_item_type));

     if (mOut.c_item_grade)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_item_grade));

     if (mOut.c_use_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_use_condition_script));

     if (mOut.c_use_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_use_action_script));

	}
};

class SP_STATIC_ITEM_STAT_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 4097, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_stat_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_stat_value = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_ITEM_STAT_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_item_stat_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_item_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stat_value);
	}
	~SP_STATIC_ITEM_STAT_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_stat_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

	}
};

class SP_STATIC_ITEM_STAT_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 4097, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_stat_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_stat_value = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_ITEM_STAT_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_item_stat_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOut(mOut.c_item_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stat_value);
	}
	~SP_STATIC_ITEM_STAT_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_stat_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

	}
};

class SP_STATIC_MAIL_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_mail_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_mail_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_title = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_MAIL_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_mail_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_mail_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_title = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_mail_sid);
		BindOutStr(mOut.c_mail_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_title, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_MAIL_SELECT()
	{
     if (mOut.c_mail_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_mail_type));

     if (mOut.c_title)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_title));

     if (mOut.c_msg)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_MAIL_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_mail_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_mail_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_title = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_MAIL_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_mail_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_mail_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_title = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_mail_sid);
		BindOutStr(mOut.c_mail_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_title, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_MAIL_SELECT_ALL()
	{
     if (mOut.c_mail_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_mail_type));

     if (mOut.c_title)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_title));

     if (mOut.c_msg)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_MAP_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_map_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_tile_theme = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_grid_col_count_factor = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_grid_row_count_factor = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division0 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division1 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division2 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division3 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division4 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division5 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division6 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_x0 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_x1 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_x2 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_x3 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_y0 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_y1 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_y2 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_y3 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_w0 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_w1 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_w2 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_w3 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_random_seed = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_initial_system_group_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_initial_system_group_fsm_state_name = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_MAP_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_map_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_initial_system_group_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_initial_system_group_fsm_state_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_map_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_tile_theme);
		BindOut(mOut.c_grid_col_count_factor);
		BindOut(mOut.c_grid_row_count_factor);
		BindOut(mOut.c_level_division0);
		BindOut(mOut.c_level_division1);
		BindOut(mOut.c_level_division2);
		BindOut(mOut.c_level_division3);
		BindOut(mOut.c_level_division4);
		BindOut(mOut.c_level_division5);
		BindOut(mOut.c_level_division6);
		BindOut(mOut.c_fractal_scale_x0);
		BindOut(mOut.c_fractal_scale_x1);
		BindOut(mOut.c_fractal_scale_x2);
		BindOut(mOut.c_fractal_scale_x3);
		BindOut(mOut.c_fractal_scale_y0);
		BindOut(mOut.c_fractal_scale_y1);
		BindOut(mOut.c_fractal_scale_y2);
		BindOut(mOut.c_fractal_scale_y3);
		BindOut(mOut.c_fractal_scale_w0);
		BindOut(mOut.c_fractal_scale_w1);
		BindOut(mOut.c_fractal_scale_w2);
		BindOut(mOut.c_fractal_scale_w3);
		BindOut(mOut.c_random_seed);
		BindOutStr(mOut.c_initial_system_group_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_initial_system_group_fsm_state_name, 51 * sizeof(wchar_t));
	}
	~SP_STATIC_MAP_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_initial_system_group_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_initial_system_group_name));

     if (mOut.c_initial_system_group_fsm_state_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_initial_system_group_fsm_state_name));

	}
};

class SP_STATIC_MAP_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_map_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_tile_theme = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_grid_col_count_factor = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_grid_row_count_factor = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division0 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division1 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division2 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division3 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division4 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division5 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_level_division6 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_x0 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_x1 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_x2 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_x3 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_y0 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_y1 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_y2 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_y3 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_w0 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_w1 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_w2 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_fractal_scale_w3 = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_random_seed = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_initial_system_group_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_initial_system_group_fsm_state_name = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_MAP_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_map_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_initial_system_group_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_initial_system_group_fsm_state_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_map_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_tile_theme);
		BindOut(mOut.c_grid_col_count_factor);
		BindOut(mOut.c_grid_row_count_factor);
		BindOut(mOut.c_level_division0);
		BindOut(mOut.c_level_division1);
		BindOut(mOut.c_level_division2);
		BindOut(mOut.c_level_division3);
		BindOut(mOut.c_level_division4);
		BindOut(mOut.c_level_division5);
		BindOut(mOut.c_level_division6);
		BindOut(mOut.c_fractal_scale_x0);
		BindOut(mOut.c_fractal_scale_x1);
		BindOut(mOut.c_fractal_scale_x2);
		BindOut(mOut.c_fractal_scale_x3);
		BindOut(mOut.c_fractal_scale_y0);
		BindOut(mOut.c_fractal_scale_y1);
		BindOut(mOut.c_fractal_scale_y2);
		BindOut(mOut.c_fractal_scale_y3);
		BindOut(mOut.c_fractal_scale_w0);
		BindOut(mOut.c_fractal_scale_w1);
		BindOut(mOut.c_fractal_scale_w2);
		BindOut(mOut.c_fractal_scale_w3);
		BindOut(mOut.c_random_seed);
		BindOutStr(mOut.c_initial_system_group_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_initial_system_group_fsm_state_name, 51 * sizeof(wchar_t));
	}
	~SP_STATIC_MAP_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_initial_system_group_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_initial_system_group_name));

     if (mOut.c_initial_system_group_fsm_state_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_initial_system_group_fsm_state_name));

	}
};

class SP_STATIC_MISSION_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_mission_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_ratio = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_MISSION_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_mission_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_ratio = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_mission_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_ratio, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_MISSION_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_ratio)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_ratio));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_MISSION_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_mission_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_ratio = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_MISSION_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_mission_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_ratio = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_mission_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_ratio, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_MISSION_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_ratio)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_ratio));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_PRODUCT_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_product_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_product_id_google = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_product_id_apple = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_price_google = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_price_apple = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_PRODUCT_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_product_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_product_id_google = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_product_id_apple = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_product_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_product_id_google, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_product_id_apple, 51 * sizeof(wchar_t));
		BindOut(mOut.c_price_google);
		BindOut(mOut.c_price_apple);
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_PRODUCT_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_product_id_google)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_product_id_google));

     if (mOut.c_product_id_apple)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_product_id_apple));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_PRODUCT_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_product_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_product_id_google = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_product_id_apple = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_price_google = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_price_apple = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_PRODUCT_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_product_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_product_id_google = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_product_id_apple = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_product_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_product_id_google, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_product_id_apple, 51 * sizeof(wchar_t));
		BindOut(mOut.c_price_google);
		BindOut(mOut.c_price_apple);
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_PRODUCT_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_product_id_google)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_product_id_google));

     if (mOut.c_product_id_apple)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_product_id_apple));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_QUEST_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_quest_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_quest_chapter = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quest_index = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_complete_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_complete_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_QUEST_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_quest_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_complete_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_complete_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_quest_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_quest_chapter);
		BindOut(mOut.c_quest_index);
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_complete_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_complete_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_QUEST_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

     if (mOut.c_complete_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_complete_condition_script));

     if (mOut.c_complete_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_complete_action_script));

	}
};

class SP_STATIC_QUEST_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_quest_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_quest_chapter = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quest_index = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_complete_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_complete_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_QUEST_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_quest_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_complete_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_complete_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_quest_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_quest_chapter);
		BindOut(mOut.c_quest_index);
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_complete_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_complete_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_QUEST_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

     if (mOut.c_complete_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_complete_condition_script));

     if (mOut.c_complete_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_complete_action_script));

	}
};

class SP_STATIC_REWARD_GROUP_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_reward_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_value = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_REWARD_GROUP_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_reward_group_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_sid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_reward_sid);
		BindOut(mOut.c_reward_value);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_item_quantity);
	}
	~SP_STATIC_REWARD_GROUP_SELECT()
	{
	}
};

class SP_STATIC_REWARD_GROUP_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_reward_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_value = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_REWARD_GROUP_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_reward_group_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_id);
		BindOut(mOut.c_reward_sid);
		BindOut(mOut.c_reward_value);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_item_quantity);
	}
	~SP_STATIC_REWARD_GROUP_SELECT_ALL()
	{
	}
};

class SP_STATIC_REWARD_RATIO_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_reward_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_value = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_REWARD_RATIO_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_reward_ratio_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_sid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_reward_sid);
		BindOut(mOut.c_reward_value);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_item_quantity);
	}
	~SP_STATIC_REWARD_RATIO_SELECT()
	{
	}
};

class SP_STATIC_REWARD_RATIO_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_reward_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_value = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_REWARD_RATIO_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_reward_ratio_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_id);
		BindOut(mOut.c_reward_sid);
		BindOut(mOut.c_reward_value);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_item_quantity);
	}
	~SP_STATIC_REWARD_RATIO_SELECT_ALL()
	{
	}
};

class SP_STATIC_REWARD_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_reward_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_reward_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_REWARD_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_reward_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_reward_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_reward_sid);
		BindOutStr(mOut.c_reward_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_item_quantity);
	}
	~SP_STATIC_REWARD_SELECT()
	{
     if (mOut.c_reward_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_reward_type));

	}
};

class SP_STATIC_REWARD_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_reward_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_reward_type = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_REWARD_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_reward_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_reward_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_reward_sid);
		BindOutStr(mOut.c_reward_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_item_quantity);
	}
	~SP_STATIC_REWARD_SELECT_ALL()
	{
     if (mOut.c_reward_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_reward_type));

	}
};

class SP_STATIC_SCHEDULE_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_schedule_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_SCHEDULE_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_schedule_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_schedule_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_SCHEDULE_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_SCHEDULE_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_schedule_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_SCHEDULE_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_schedule_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_schedule_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_SCHEDULE_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_STAT_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_stat_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_STAT_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_stat_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_id);
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
	}
	~SP_STATIC_STAT_SELECT()
	{
     if (mOut.c_stat_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

	}
};

class SP_STATIC_STAT_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_stat_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_STAT_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_stat_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
	}
	~SP_STATIC_STAT_SELECT_ALL()
	{
     if (mOut.c_stat_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

	}
};

class SP_STATIC_STOCK_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_stock_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_stock_slot = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_ratio = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_STOCK_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_stock_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_stock_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stock_slot);
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOut(mOut.c_ratio);
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_STOCK_SELECT()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_STOCK_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_stock_sid = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_name = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_stock_slot = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_condition_script = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_ratio = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_action_script = nullptr; // type: 254, flags: 0, charset: 33 
	} mOut;

public:
	SP_STATIC_STOCK_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_stock_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_condition_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_action_script = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_stock_sid);
		BindOutStr(mOut.c_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stock_slot);
		BindOutStr(mOut.c_condition_script, 251 * sizeof(wchar_t));
		BindOut(mOut.c_ratio);
		BindOutStr(mOut.c_action_script, 251 * sizeof(wchar_t));
	}
	~SP_STATIC_STOCK_SELECT_ALL()
	{
     if (mOut.c_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_name));

     if (mOut.c_condition_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_condition_script));

     if (mOut.c_action_script)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_action_script));

	}
};

class SP_STATIC_TRANSLATION_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_display_key = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_language_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_language_text = nullptr; // type: 252, flags: 16, charset: 33 
	} mOut;

public:
	SP_STATIC_TRANSLATION_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_translation_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_display_key = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_language_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_language_text = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_sid);

		BindOut(mOut.c_id);
		BindOutStr(mOut.c_display_key, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_language_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_language_text, 65536 * sizeof(wchar_t));
	}
	~SP_STATIC_TRANSLATION_SELECT()
	{
     if (mOut.c_display_key)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_display_key));

     if (mOut.c_language_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_language_type));

     if (mOut.c_language_text)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_language_text));

	}
};

class SP_STATIC_TRANSLATION_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_display_key = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_language_type = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_language_text = nullptr; // type: 252, flags: 16, charset: 33 
	} mOut;

public:
	SP_STATIC_TRANSLATION_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_translation_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_display_key = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_language_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_language_text = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOutStr(mOut.c_display_key, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_language_type, 51 * sizeof(wchar_t));
		BindOutStr(mOut.c_language_text, 65536 * sizeof(wchar_t));
	}
	~SP_STATIC_TRANSLATION_SELECT_ALL()
	{
     if (mOut.c_display_key)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_display_key));

     if (mOut.c_language_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_language_type));

     if (mOut.c_language_text)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_language_text));

	}
};

class SP_STATIC_USER_EXP_SELECT : public DbSp
{
public:
   const int64_t mIn_p_sid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_sid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_USER_EXP_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_sid
	)
		:
		DbSp(L"{call sp_static_user_exp_select(?)}", session),
		mIn_p_sid(in_p_sid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_sid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_user_level);
		BindOut(mOut.c_mail_sid);
	}
	~SP_STATIC_USER_EXP_SELECT()
	{
	}
};

class SP_STATIC_USER_EXP_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_sid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_STATIC_USER_EXP_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_static_user_exp_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_id);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_user_level);
		BindOut(mOut.c_mail_sid);
	}
	~SP_STATIC_USER_EXP_SELECT_ALL()
	{
	}
};

