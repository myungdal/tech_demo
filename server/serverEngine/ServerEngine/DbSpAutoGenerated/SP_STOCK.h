// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_STOCK_DELETE : public DbSp
{
public:
   const int64_t mIn_p_uuid;
   const int64_t mIn_p_stock_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_uuid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_STOCK_DELETE(
		class DbSessionBase& session,
		const int64_t& in_p_uuid,
		const int64_t& in_p_stock_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_stock_delete(?, ?, ?)}", session),
		mIn_p_uuid(in_p_uuid),
		mIn_p_stock_id(in_p_stock_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);
		BindIn(mIn_p_stock_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_uuid);
	}
	~SP_STOCK_DELETE()
	{
	}
};

class SP_STOCK_INSERT : public DbSp
{
public:
   const int64_t mIn_p_new_stock_id;
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_stock_sid;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_new_stock_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_STOCK_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_new_stock_id,
		const int64_t& in_p_user_id,
		const int64_t& in_p_stock_sid,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_stock_insert(?, ?, ?, ?)}", session),
		mIn_p_new_stock_id(in_p_new_stock_id),
		mIn_p_user_id(in_p_user_id),
		mIn_p_stock_sid(in_p_stock_sid),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_new_stock_id);
		BindIn(mIn_p_user_id);
		BindIn(mIn_p_stock_sid);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_new_stock_id);
	}
	~SP_STOCK_INSERT()
	{
	}
};

class SP_STOCK_UPDATE : public DbSp
{
public:
   const int64_t mIn_p_stock_id;
   const int64_t mIn_p_stock_sid;
   const int64_t mIn_p_reward_flag;
   const int64_t mIn_p_stock_reset_count;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_stock_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_STOCK_UPDATE(
		class DbSessionBase& session,
		const int64_t& in_p_stock_id,
		const int64_t& in_p_stock_sid,
		const int64_t& in_p_reward_flag,
		const int64_t& in_p_stock_reset_count,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_stock_update(?, ?, ?, ?, ?)}", session),
		mIn_p_stock_id(in_p_stock_id),
		mIn_p_stock_sid(in_p_stock_sid),
		mIn_p_reward_flag(in_p_reward_flag),
		mIn_p_stock_reset_count(in_p_stock_reset_count),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_stock_id);
		BindIn(mIn_p_stock_sid);
		BindIn(mIn_p_reward_flag);
		BindIn(mIn_p_stock_reset_count);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_stock_id);
	}
	~SP_STOCK_UPDATE()
	{
	}
};

