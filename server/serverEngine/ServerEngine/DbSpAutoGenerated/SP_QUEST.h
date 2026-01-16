// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_QUEST_DELETE : public DbSp
{
public:
   const int64_t mIn_p_uuid;
   const int64_t mIn_p_quest_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_uuid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_QUEST_DELETE(
		class DbSessionBase& session,
		const int64_t& in_p_uuid,
		const int64_t& in_p_quest_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_quest_delete(?, ?, ?)}", session),
		mIn_p_uuid(in_p_uuid),
		mIn_p_quest_id(in_p_quest_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);
		BindIn(mIn_p_quest_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_uuid);
	}
	~SP_QUEST_DELETE()
	{
	}
};

class SP_QUEST_DELETED_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quest_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quest_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_delta = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_QUEST_DELETED_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_quest_deleted_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_quest_id);
		BindOut(mOut.c_quest_sid);
		BindOut(mOut.c_activity_point_delta);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_QUEST_DELETED_SELECT()
	{
	}
};

class SP_QUEST_DELETED_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quest_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quest_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_delta = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_QUEST_DELETED_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_quest_deleted_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_quest_id);
		BindOut(mOut.c_quest_sid);
		BindOut(mOut.c_activity_point_delta);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_QUEST_DELETED_SELECT_ALL()
	{
	}
};

class SP_QUEST_INSERT : public DbSp
{
public:
   const int64_t mIn_p_new_quest_id;
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_quest_sid;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_new_quest_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_QUEST_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_new_quest_id,
		const int64_t& in_p_user_id,
		const int64_t& in_p_quest_sid,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_quest_insert(?, ?, ?, ?)}", session),
		mIn_p_new_quest_id(in_p_new_quest_id),
		mIn_p_user_id(in_p_user_id),
		mIn_p_quest_sid(in_p_quest_sid),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_new_quest_id);
		BindIn(mIn_p_user_id);
		BindIn(mIn_p_quest_sid);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_new_quest_id);
	}
	~SP_QUEST_INSERT()
	{
	}
};

class SP_QUEST_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_quest_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_quest_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_delta = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_QUEST_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_quest_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_quest_id);
		BindOut(mOut.c_quest_sid);
		BindOut(mOut.c_activity_point_delta);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_QUEST_SELECT()
	{
	}
};

class SP_QUEST_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_quest_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_quest_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_delta = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_QUEST_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_quest_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_user_id);
		BindOut(mOut.c_quest_id);
		BindOut(mOut.c_quest_sid);
		BindOut(mOut.c_activity_point_delta);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_QUEST_SELECT_ALL()
	{
	}
};

class SP_QUEST_SELECT_BY_USER : public DbSp
{
public:
   const int64_t mIn_p_user_id;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_quest_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_quest_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_activity_point_delta = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_QUEST_SELECT_BY_USER(
		class DbSessionBase& session,
		const int64_t& in_p_user_id
	)
		:
		DbSp(L"{call sp_quest_select_by_user(?)}", session),
		mIn_p_user_id(in_p_user_id)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_quest_id);
		BindOut(mOut.c_quest_sid);
		BindOut(mOut.c_activity_point_delta);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_QUEST_SELECT_BY_USER()
	{
	}
};

class SP_QUEST_UPDATE : public DbSp
{
public:
   const int64_t mIn_p_quest_id;
   const int64_t mIn_p_quest_sid;
   const int64_t mIn_p_quest_score_point;
   const int64_t mIn_p_quest_score_time;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_quest_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_QUEST_UPDATE(
		class DbSessionBase& session,
		const int64_t& in_p_quest_id,
		const int64_t& in_p_quest_sid,
		const int64_t& in_p_quest_score_point,
		const int64_t& in_p_quest_score_time,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_quest_update(?, ?, ?, ?, ?)}", session),
		mIn_p_quest_id(in_p_quest_id),
		mIn_p_quest_sid(in_p_quest_sid),
		mIn_p_quest_score_point(in_p_quest_score_point),
		mIn_p_quest_score_time(in_p_quest_score_time),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_quest_id);
		BindIn(mIn_p_quest_sid);
		BindIn(mIn_p_quest_score_point);
		BindIn(mIn_p_quest_score_time);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_quest_id);
	}
	~SP_QUEST_UPDATE()
	{
	}
};

