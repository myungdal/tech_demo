#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_CHARACTER_POS_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   wchar_t* c_game_type = nullptr; // type: 254, flags: 20483, charset: 33 
		   int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_x = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_y = 0; // type: 8, flags: 0, charset: 63 
		   TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_CHARACTER_POS_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_character_pos_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
	   if (false == session.IsReady())
		   return;

	   mOut.c_game_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOutStr(mOut.c_game_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_x);
		BindOut(mOut.c_y);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_CHARACTER_POS_SELECT()
	{
	 if (mOut.c_game_type)
		 PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_game_type));

	}
};

class SP_CHARACTER_POS_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   wchar_t* c_game_type = nullptr; // type: 254, flags: 20483, charset: 33 
		   int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_x = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_y = 0; // type: 8, flags: 0, charset: 63 
		   TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_CHARACTER_POS_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_character_pos_select_all()}", session)
	{
	   if (false == session.IsReady())
		   return;

	   mOut.c_game_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOutStr(mOut.c_game_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_x);
		BindOut(mOut.c_y);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_CHARACTER_POS_SELECT_ALL()
	{
	 if (mOut.c_game_type)
		 PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_game_type));

	}
};

class SP_CHARACTER_POS_SELECT_BY_USER : public DbSp
{
public:
   const int64_t mIn_p_user_id;

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   wchar_t* c_game_type = nullptr; // type: 254, flags: 20483, charset: 33 
		   int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_x = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_y = 0; // type: 8, flags: 0, charset: 63 
		   TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_CHARACTER_POS_SELECT_BY_USER(
		class DbSessionBase& session,
		const int64_t& in_p_user_id
	)
		:
		DbSp(L"{call sp_character_pos_select_by_user(?)}", session),
		mIn_p_user_id(in_p_user_id)
	{
	   if (false == session.IsReady())
		   return;

	   mOut.c_game_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_user_id);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOutStr(mOut.c_game_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_x);
		BindOut(mOut.c_y);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_CHARACTER_POS_SELECT_BY_USER()
	{
	 if (mOut.c_game_type)
		 PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_game_type));

	}
};

class SP_CHARACTER_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_x = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_y = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_date_updated = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_CHARACTER_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_character_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
	   if (false == session.IsReady())
		   return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_x);
		BindOut(mOut.c_y);
		BindOut(mOut.c_date_updated);
	}
	~SP_CHARACTER_SELECT()
	{
	}
};

class SP_CHARACTER_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_x = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_y = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_date_updated = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_CHARACTER_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_character_select_all()}", session)
	{
	   if (false == session.IsReady())
		   return;


		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_x);
		BindOut(mOut.c_y);
		BindOut(mOut.c_date_updated);
	}
	~SP_CHARACTER_SELECT_ALL()
	{
	}
};

class SP_CHARACTER_SELECT_BY_USER : public DbSp
{
public:
   const int64_t mIn_p_user_id;

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_x = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_y = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_date_updated = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_CHARACTER_SELECT_BY_USER(
		class DbSessionBase& session,
		const int64_t& in_p_user_id
	)
		:
		DbSp(L"{call sp_character_select_by_user(?)}", session),
		mIn_p_user_id(in_p_user_id)
	{
	   if (false == session.IsReady())
		   return;


		BindIn(mIn_p_user_id);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_x);
		BindOut(mOut.c_y);
		BindOut(mOut.c_date_updated);
	}
	~SP_CHARACTER_SELECT_BY_USER()
	{
	}
};

class SP_CHARACTER_STAT_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   wchar_t* c_stat_type = nullptr; // type: 254, flags: 20483, charset: 33 
		   int64_t c_stat_value = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_date_updated = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_CHARACTER_STAT_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_character_stat_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
	   if (false == session.IsReady())
		   return;

	   mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stat_value);
		BindOut(mOut.c_date_updated);
	}
	~SP_CHARACTER_STAT_SELECT()
	{
	 if (mOut.c_stat_type)
		 PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

	}
};

class SP_CHARACTER_STAT_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   wchar_t* c_stat_type = nullptr; // type: 254, flags: 20483, charset: 33 
		   int64_t c_stat_value = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_date_updated = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_CHARACTER_STAT_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_character_stat_select_all()}", session)
	{
	   if (false == session.IsReady())
		   return;

	   mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stat_value);
		BindOut(mOut.c_date_updated);
	}
	~SP_CHARACTER_STAT_SELECT_ALL()
	{
	 if (mOut.c_stat_type)
		 PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

	}
};

class SP_CHARACTER_STAT_SELECT_BY_USER : public DbSp
{
public:
   const int64_t mIn_p_user_id;

	struct Out
	{
		   int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
		   int64_t c_character_item_id = 0; // type: 8, flags: 20483, charset: 63 
		   wchar_t* c_stat_type = nullptr; // type: 254, flags: 20483, charset: 33 
		   int64_t c_stat_value = 0; // type: 8, flags: 0, charset: 63 
		   int64_t c_date_updated = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_CHARACTER_STAT_SELECT_BY_USER(
		class DbSessionBase& session,
		const int64_t& in_p_user_id
	)
		:
		DbSp(L"{call sp_character_stat_select_by_user(?)}", session),
		mIn_p_user_id(in_p_user_id)
	{
	   if (false == session.IsReady())
		   return;

	   mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_user_id);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_character_item_id);
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stat_value);
		BindOut(mOut.c_date_updated);
	}
	~SP_CHARACTER_STAT_SELECT_BY_USER()
	{
	 if (mOut.c_stat_type)
		 PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

	}
};

