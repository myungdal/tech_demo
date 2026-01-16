// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_LOG_ACHIEVEMENT_INSERT : public DbSp
{
public:
   const int64_t mIn_p_id;
   const int64_t mIn_p_log_id;
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_achievement_sid;
   const int64_t mIn_p_achievement_step_sid;
   const TIMESTAMP_STRUCT mIn_p_date_created;
   const TIMESTAMP_STRUCT mIn_p_date_log;

	struct Out
	{
	} mOut;

public:
	SP_LOG_ACHIEVEMENT_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_id,
		const int64_t& in_p_log_id,
		const int64_t& in_p_user_id,
		const int64_t& in_p_achievement_sid,
		const int64_t& in_p_achievement_step_sid,
		const TIMESTAMP_STRUCT& in_p_date_created,
		const TIMESTAMP_STRUCT& in_p_date_log
	)
		:
		DbSp(L"{call sp_log_achievement_insert(?, ?, ?, ?, ?, ?, ?)}", session),
		mIn_p_id(in_p_id),
		mIn_p_log_id(in_p_log_id),
		mIn_p_user_id(in_p_user_id),
		mIn_p_achievement_sid(in_p_achievement_sid),
		mIn_p_achievement_step_sid(in_p_achievement_step_sid),
		mIn_p_date_created(in_p_date_created),
		mIn_p_date_log(in_p_date_log)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_id);
		BindIn(mIn_p_log_id);
		BindIn(mIn_p_user_id);
		BindIn(mIn_p_achievement_sid);
		BindIn(mIn_p_achievement_step_sid);
		BindInDate(mIn_p_date_created);
		BindInDate(mIn_p_date_log);

	}
	~SP_LOG_ACHIEVEMENT_INSERT()
	{
	}
};

class SP_LOG_ACHIEVEMENT_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_log_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_achievement_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_achievement_step_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_log = {}; // type: 12, flags: 16520, charset: 63 
	} mOut;

public:
	SP_LOG_ACHIEVEMENT_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_log_achievement_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_log_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_achievement_sid);
		BindOut(mOut.c_achievement_step_sid);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_log);
	}
	~SP_LOG_ACHIEVEMENT_SELECT()
	{
	}
};

class SP_LOG_ACHIEVEMENT_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_log_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_achievement_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_achievement_step_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_log = {}; // type: 12, flags: 16520, charset: 63 
	} mOut;

public:
	SP_LOG_ACHIEVEMENT_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_log_achievement_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_id);
		BindOut(mOut.c_log_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_achievement_sid);
		BindOut(mOut.c_achievement_step_sid);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_log);
	}
	~SP_LOG_ACHIEVEMENT_SELECT_ALL()
	{
	}
};

