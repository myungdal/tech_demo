// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_ACTIVITY_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_activity_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_activity_sid = 0; // type: 8, flags: 16384, charset: 63 
           int64_t c_activity_point = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_increased = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_decreased = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_min = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_max = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACTIVITY_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_activity_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_activity_id);
		BindOut(mOut.c_activity_sid);
		BindOut(mOut.c_activity_point);
		BindOut(mOut.c_activity_point_increased);
		BindOut(mOut.c_activity_point_decreased);
		BindOut(mOut.c_activity_point_min);
		BindOut(mOut.c_activity_point_max);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ACTIVITY_SELECT()
	{
	}
};

class SP_ACTIVITY_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_activity_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_activity_sid = 0; // type: 8, flags: 16384, charset: 63 
           int64_t c_activity_point = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_increased = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_decreased = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_min = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_max = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACTIVITY_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_activity_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_user_id);
		BindOut(mOut.c_activity_id);
		BindOut(mOut.c_activity_sid);
		BindOut(mOut.c_activity_point);
		BindOut(mOut.c_activity_point_increased);
		BindOut(mOut.c_activity_point_decreased);
		BindOut(mOut.c_activity_point_min);
		BindOut(mOut.c_activity_point_max);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ACTIVITY_SELECT_ALL()
	{
	}
};

class SP_ACTIVITY_SELECT_BY_USER : public DbSp
{
public:
   const int64_t mIn_p_user_id;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_activity_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_activity_sid = 0; // type: 8, flags: 16384, charset: 63 
           int64_t c_activity_point = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_increased = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_decreased = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_min = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_max = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACTIVITY_SELECT_BY_USER(
		class DbSessionBase& session,
		const int64_t& in_p_user_id
	)
		:
		DbSp(L"{call sp_activity_select_by_user(?)}", session),
		mIn_p_user_id(in_p_user_id)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_activity_id);
		BindOut(mOut.c_activity_sid);
		BindOut(mOut.c_activity_point);
		BindOut(mOut.c_activity_point_increased);
		BindOut(mOut.c_activity_point_decreased);
		BindOut(mOut.c_activity_point_min);
		BindOut(mOut.c_activity_point_max);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ACTIVITY_SELECT_BY_USER()
	{
	}
};

