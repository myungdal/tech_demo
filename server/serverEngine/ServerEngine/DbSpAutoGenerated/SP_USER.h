// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_USER_INSERT : public DbSp
{
public:
   const int64_t mIn_p_account_id;
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_user_exp;
   const int64_t mIn_p_user_exp_total;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_USER_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_account_id,
		const int64_t& in_p_user_id,
		const int64_t& in_p_user_exp,
		const int64_t& in_p_user_exp_total,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_user_insert(?, ?, ?, ?, ?)}", session),
		mIn_p_account_id(in_p_account_id),
		mIn_p_user_id(in_p_user_id),
		mIn_p_user_exp(in_p_user_exp),
		mIn_p_user_exp_total(in_p_user_exp_total),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_account_id);
		BindIn(mIn_p_user_id);
		BindIn(mIn_p_user_exp);
		BindIn(mIn_p_user_exp_total);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_USER_INSERT()
	{
	}
};

class SP_USER_POS_INSERT : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const wchar_t* mIn_p_game_type;
   const int64_t mIn_p_game_id;
   const int64_t mIn_p_x;
   const int64_t mIn_p_y;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_USER_POS_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const wchar_t* in_p_game_type,
		const int64_t& in_p_game_id,
		const int64_t& in_p_x,
		const int64_t& in_p_y,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_user_pos_insert(?, ?, ?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_game_type(in_p_game_type),
		mIn_p_game_id(in_p_game_id),
		mIn_p_x(in_p_x),
		mIn_p_y(in_p_y),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindInStr(mIn_p_game_type);
		BindIn(mIn_p_game_id);
		BindIn(mIn_p_x);
		BindIn(mIn_p_y);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_USER_POS_INSERT()
	{
	}
};

class SP_USER_POS_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_game_type = nullptr; // type: 254, flags: 20483, charset: 33 
           int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_x = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_y = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_USER_POS_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_user_pos_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_game_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOutStr(mOut.c_game_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_x);
		BindOut(mOut.c_y);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_USER_POS_SELECT()
	{
     if (mOut.c_game_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_game_type));

	}
};

class SP_USER_POS_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_game_type = nullptr; // type: 254, flags: 20483, charset: 33 
           int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_x = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_y = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_USER_POS_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_user_pos_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_game_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_user_id);
		BindOutStr(mOut.c_game_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_x);
		BindOut(mOut.c_y);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_USER_POS_SELECT_ALL()
	{
     if (mOut.c_game_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_game_type));

	}
};

class SP_USER_POS_UPDATE : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const wchar_t* mIn_p_game_type;
   const int64_t mIn_p_game_id;
   const int64_t mIn_p_x;
   const int64_t mIn_p_y;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_USER_POS_UPDATE(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const wchar_t* in_p_game_type,
		const int64_t& in_p_game_id,
		const int64_t& in_p_x,
		const int64_t& in_p_y,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_user_pos_update(?, ?, ?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_game_type(in_p_game_type),
		mIn_p_game_id(in_p_game_id),
		mIn_p_x(in_p_x),
		mIn_p_y(in_p_y),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindInStr(mIn_p_game_type);
		BindIn(mIn_p_game_id);
		BindIn(mIn_p_x);
		BindIn(mIn_p_y);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_USER_POS_UPDATE()
	{
	}
};

class SP_USER_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_exp_total = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_play_second_total = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_login = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_logout = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_USER_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_user_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_user_exp_total);
		BindOut(mOut.c_play_second_total);
		BindOutDate(mOut.c_date_login);
		BindOutDate(mOut.c_date_logout);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_USER_SELECT()
	{
	}
};

class SP_USER_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_exp_total = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_play_second_total = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_login = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_logout = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_USER_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_user_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_user_exp_total);
		BindOut(mOut.c_play_second_total);
		BindOutDate(mOut.c_date_login);
		BindOutDate(mOut.c_date_logout);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_USER_SELECT_ALL()
	{
	}
};

class SP_USER_SELECT_BY_ACCOUNT : public DbSp
{
public:
   const int64_t mIn_p_account_id;

	struct Out
	{
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_exp = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_exp_total = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_play_second_total = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_login = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_logout = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_USER_SELECT_BY_ACCOUNT(
		class DbSessionBase& session,
		const int64_t& in_p_account_id
	)
		:
		DbSp(L"{call sp_user_select_by_account(?)}", session),
		mIn_p_account_id(in_p_account_id)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_account_id);

		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_exp);
		BindOut(mOut.c_user_exp_total);
		BindOut(mOut.c_play_second_total);
		BindOutDate(mOut.c_date_login);
		BindOutDate(mOut.c_date_logout);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_USER_SELECT_BY_ACCOUNT()
	{
	}
};

class SP_USER_UPDATE : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_user_exp;
   const int64_t mIn_p_user_exp_total;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_USER_UPDATE(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_user_exp,
		const int64_t& in_p_user_exp_total,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_user_update(?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_user_exp(in_p_user_exp),
		mIn_p_user_exp_total(in_p_user_exp_total),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_user_exp);
		BindIn(mIn_p_user_exp_total);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_USER_UPDATE()
	{
	}
};

class SP_USER_UPDATE_EXP : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_user_exp;
   const int64_t mIn_p_user_level;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_USER_UPDATE_EXP(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_user_exp,
		const int64_t& in_p_user_level,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_user_update_exp(?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_user_exp(in_p_user_exp),
		mIn_p_user_level(in_p_user_level),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_user_exp);
		BindIn(mIn_p_user_level);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_USER_UPDATE_EXP()
	{
	}
};

class SP_USER_UPDATE_LOGIN : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_USER_UPDATE_LOGIN(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_user_update_login(?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_USER_UPDATE_LOGIN()
	{
	}
};

class SP_USER_UPDATE_LOGOUT : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_play_second_total;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_USER_UPDATE_LOGOUT(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_play_second_total,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_user_update_logout(?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_play_second_total(in_p_play_second_total),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_play_second_total);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_USER_UPDATE_LOGOUT()
	{
	}
};

