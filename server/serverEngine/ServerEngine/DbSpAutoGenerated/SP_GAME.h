// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_GAME_DELETE : public DbSp
{
public:
   const int64_t mIn_p_uuid;
   const int64_t mIn_p_game_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_uuid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_GAME_DELETE(
		class DbSessionBase& session,
		const int64_t& in_p_uuid,
		const int64_t& in_p_game_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_game_delete(?, ?, ?)}", session),
		mIn_p_uuid(in_p_uuid),
		mIn_p_game_id(in_p_game_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);
		BindIn(mIn_p_game_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_uuid);
	}
	~SP_GAME_DELETE()
	{
	}
};

class SP_GAME_DELETED_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_server_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_game_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_GAME_DELETED_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_game_deleted_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_server_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_game_sid);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_GAME_DELETED_SELECT()
	{
	}
};

class SP_GAME_DELETED_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_server_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_game_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_GAME_DELETED_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_game_deleted_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_id);
		BindOut(mOut.c_game_id);
		BindOut(mOut.c_server_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_game_sid);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_GAME_DELETED_SELECT_ALL()
	{
	}
};

class SP_GAME_INSERT : public DbSp
{
public:
   const int64_t mIn_p_new_game_id;
   const int64_t mIn_p_server_id;
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_game_sid;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_new_game_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_GAME_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_new_game_id,
		const int64_t& in_p_server_id,
		const int64_t& in_p_user_id,
		const int64_t& in_p_game_sid,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_game_insert(?, ?, ?, ?, ?)}", session),
		mIn_p_new_game_id(in_p_new_game_id),
		mIn_p_server_id(in_p_server_id),
		mIn_p_user_id(in_p_user_id),
		mIn_p_game_sid(in_p_game_sid),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_new_game_id);
		BindIn(mIn_p_server_id);
		BindIn(mIn_p_user_id);
		BindIn(mIn_p_game_sid);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_new_game_id);
	}
	~SP_GAME_INSERT()
	{
	}
};

class SP_GAME_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_game_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_server_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_game_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_GAME_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_game_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_game_id);
		BindOut(mOut.c_server_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_game_sid);
		BindOutDate(mOut.c_date_created);
	}
	~SP_GAME_SELECT()
	{
	}
};

class SP_GAME_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_game_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_server_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_game_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_GAME_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_game_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_game_id);
		BindOut(mOut.c_server_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_game_sid);
		BindOutDate(mOut.c_date_created);
	}
	~SP_GAME_SELECT_ALL()
	{
	}
};

