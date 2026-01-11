// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_ACCOUNT_DELETE : public DbSp
{
public:
   const int64_t mIn_p_uuid;
   const int64_t mIn_p_account_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_uuid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_DELETE(
		class DbSessionBase& session,
		const int64_t& in_p_uuid,
		const int64_t& in_p_account_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_delete(?, ?, ?)}", session),
		mIn_p_uuid(in_p_uuid),
		mIn_p_account_id(in_p_account_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);
		BindIn(mIn_p_account_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_uuid);
	}
	~SP_ACCOUNT_DELETE()
	{
	}
};

class SP_ACCOUNT_DELETED_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_device_token = nullptr; // type: 252, flags: 16, charset: 33 
           wchar_t* c_google_token = nullptr; // type: 252, flags: 16, charset: 33 
           wchar_t* c_apple_token = nullptr; // type: 252, flags: 16, charset: 33 
           int64_t c_account_state = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_db_shard_idx = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_DELETED_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_account_deleted_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_device_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_google_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_apple_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_account_id);
		BindOutStr(mOut.c_device_token, 65536 * sizeof(wchar_t));
		BindOutStr(mOut.c_google_token, 65536 * sizeof(wchar_t));
		BindOutStr(mOut.c_apple_token, 65536 * sizeof(wchar_t));
		BindOut(mOut.c_account_state);
		BindOut(mOut.c_db_shard_idx);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_ACCOUNT_DELETED_SELECT()
	{
     if (mOut.c_device_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_device_token));

     if (mOut.c_google_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_google_token));

     if (mOut.c_apple_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_apple_token));

	}
};

class SP_ACCOUNT_DELETED_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_device_token = nullptr; // type: 252, flags: 16, charset: 33 
           wchar_t* c_google_token = nullptr; // type: 252, flags: 16, charset: 33 
           wchar_t* c_apple_token = nullptr; // type: 252, flags: 16, charset: 33 
           int64_t c_account_state = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_db_shard_idx = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_DELETED_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_account_deleted_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_device_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_google_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_apple_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOut(mOut.c_account_id);
		BindOutStr(mOut.c_device_token, 65536 * sizeof(wchar_t));
		BindOutStr(mOut.c_google_token, 65536 * sizeof(wchar_t));
		BindOutStr(mOut.c_apple_token, 65536 * sizeof(wchar_t));
		BindOut(mOut.c_account_state);
		BindOut(mOut.c_db_shard_idx);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_ACCOUNT_DELETED_SELECT_ALL()
	{
     if (mOut.c_device_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_device_token));

     if (mOut.c_google_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_google_token));

     if (mOut.c_apple_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_apple_token));

	}
};

class SP_ACCOUNT_INSERT : public DbSp
{
public:
   const int64_t mIn_p_new_account_id;
   const wchar_t* mIn_p_device_token;
   const wchar_t* mIn_p_google_token;
   const wchar_t* mIn_p_apple_token;
   const int64_t mIn_p_db_shard_idx;
   const int64_t mIn_p_account_state;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_new_account_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_new_account_id,
		const wchar_t* in_p_device_token,
		const wchar_t* in_p_google_token,
		const wchar_t* in_p_apple_token,
		const int64_t& in_p_db_shard_idx,
		const int64_t& in_p_account_state,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_insert(?, ?, ?, ?, ?, ?, ?)}", session),
		mIn_p_new_account_id(in_p_new_account_id),
		mIn_p_device_token(in_p_device_token),
		mIn_p_google_token(in_p_google_token),
		mIn_p_apple_token(in_p_apple_token),
		mIn_p_db_shard_idx(in_p_db_shard_idx),
		mIn_p_account_state(in_p_account_state),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_new_account_id);
		BindInStr(mIn_p_device_token);
		BindInStr(mIn_p_google_token);
		BindInStr(mIn_p_apple_token);
		BindIn(mIn_p_db_shard_idx);
		BindIn(mIn_p_account_state);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_new_account_id);
	}
	~SP_ACCOUNT_INSERT()
	{
	}
};

class SP_ACCOUNT_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_account_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_device_token = nullptr; // type: 252, flags: 16, charset: 33 
           wchar_t* c_google_token = nullptr; // type: 252, flags: 16, charset: 33 
           wchar_t* c_apple_token = nullptr; // type: 252, flags: 16, charset: 33 
           int64_t c_db_shard_idx = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_account_state = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_ban = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_account_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_device_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_google_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_apple_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_account_id);
		BindOutStr(mOut.c_device_token, 65536 * sizeof(wchar_t));
		BindOutStr(mOut.c_google_token, 65536 * sizeof(wchar_t));
		BindOutStr(mOut.c_apple_token, 65536 * sizeof(wchar_t));
		BindOut(mOut.c_db_shard_idx);
		BindOut(mOut.c_account_state);
		BindOutDate(mOut.c_date_ban);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ACCOUNT_SELECT()
	{
     if (mOut.c_device_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_device_token));

     if (mOut.c_google_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_google_token));

     if (mOut.c_apple_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_apple_token));

	}
};

class SP_ACCOUNT_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_account_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_device_token = nullptr; // type: 252, flags: 16, charset: 33 
           wchar_t* c_google_token = nullptr; // type: 252, flags: 16, charset: 33 
           wchar_t* c_apple_token = nullptr; // type: 252, flags: 16, charset: 33 
           int64_t c_db_shard_idx = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_account_state = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_ban = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_account_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_device_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_google_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_apple_token = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_account_id);
		BindOutStr(mOut.c_device_token, 65536 * sizeof(wchar_t));
		BindOutStr(mOut.c_google_token, 65536 * sizeof(wchar_t));
		BindOutStr(mOut.c_apple_token, 65536 * sizeof(wchar_t));
		BindOut(mOut.c_db_shard_idx);
		BindOut(mOut.c_account_state);
		BindOutDate(mOut.c_date_ban);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ACCOUNT_SELECT_ALL()
	{
     if (mOut.c_device_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_device_token));

     if (mOut.c_google_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_google_token));

     if (mOut.c_apple_token)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_apple_token));

	}
};

class SP_ACCOUNT_UPDATE_BAN : public DbSp
{
public:
   const int64_t mIn_p_account_id;
   const int64_t mIn_p_account_state;
   const TIMESTAMP_STRUCT mIn_p_date_ban;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_account_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_UPDATE_BAN(
		class DbSessionBase& session,
		const int64_t& in_p_account_id,
		const int64_t& in_p_account_state,
		const TIMESTAMP_STRUCT& in_p_date_ban,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_update_ban(?, ?, ?, ?)}", session),
		mIn_p_account_id(in_p_account_id),
		mIn_p_account_state(in_p_account_state),
		mIn_p_date_ban(in_p_date_ban),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_account_id);
		BindIn(mIn_p_account_state);
		BindInDate(mIn_p_date_ban);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_account_id);
	}
	~SP_ACCOUNT_UPDATE_BAN()
	{
	}
};

class SP_ACCOUNT_UPDATE_STATE : public DbSp
{
public:
   const int64_t mIn_p_account_id;
   const int64_t mIn_p_account_state;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_account_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_UPDATE_STATE(
		class DbSessionBase& session,
		const int64_t& in_p_account_id,
		const int64_t& in_p_account_state,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_update_state(?, ?, ?)}", session),
		mIn_p_account_id(in_p_account_id),
		mIn_p_account_state(in_p_account_state),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_account_id);
		BindIn(mIn_p_account_state);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_account_id);
	}
	~SP_ACCOUNT_UPDATE_STATE()
	{
	}
};

class SP_ACCOUNT_UPDATE_TOKEN : public DbSp
{
public:
   const int64_t mIn_p_account_id;
   const wchar_t* mIn_p_device_token;
   const wchar_t* mIn_p_google_token;
   const wchar_t* mIn_p_apple_token;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_account_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_UPDATE_TOKEN(
		class DbSessionBase& session,
		const int64_t& in_p_account_id,
		const wchar_t* in_p_device_token,
		const wchar_t* in_p_google_token,
		const wchar_t* in_p_apple_token,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_update_token(?, ?, ?, ?, ?)}", session),
		mIn_p_account_id(in_p_account_id),
		mIn_p_device_token(in_p_device_token),
		mIn_p_google_token(in_p_google_token),
		mIn_p_apple_token(in_p_apple_token),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_account_id);
		BindInStr(mIn_p_device_token);
		BindInStr(mIn_p_google_token);
		BindInStr(mIn_p_apple_token);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_account_id);
	}
	~SP_ACCOUNT_UPDATE_TOKEN()
	{
	}
};

class SP_ACCOUNT_USER_DELETE : public DbSp
{
public:
   const int64_t mIn_p_uuid;
   const int64_t mIn_p_user_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_uuid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_USER_DELETE(
		class DbSessionBase& session,
		const int64_t& in_p_uuid,
		const int64_t& in_p_user_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_user_delete(?, ?, ?)}", session),
		mIn_p_uuid(in_p_uuid),
		mIn_p_user_id(in_p_user_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);
		BindIn(mIn_p_user_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_uuid);
	}
	~SP_ACCOUNT_USER_DELETE()
	{
	}
};

class SP_ACCOUNT_USER_DELETED_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 4097, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_type = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_state = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_user_name = nullptr; // type: 254, flags: 16388, charset: 33 
           int64_t c_user_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_level_highest = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_ban = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_USER_DELETED_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_account_user_deleted_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_user_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_type);
		BindOut(mOut.c_user_state);
		BindOutStr(mOut.c_user_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_user_level);
		BindOut(mOut.c_user_level_highest);
		BindOutDate(mOut.c_date_ban);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_ACCOUNT_USER_DELETED_SELECT()
	{
     if (mOut.c_user_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_user_name));

	}
};

class SP_ACCOUNT_USER_DELETED_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 4097, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_type = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_state = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_user_name = nullptr; // type: 254, flags: 16388, charset: 33 
           int64_t c_user_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_level_highest = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_ban = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_USER_DELETED_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_account_user_deleted_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_user_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_type);
		BindOut(mOut.c_user_state);
		BindOutStr(mOut.c_user_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_user_level);
		BindOut(mOut.c_user_level_highest);
		BindOutDate(mOut.c_date_ban);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_ACCOUNT_USER_DELETED_SELECT_ALL()
	{
     if (mOut.c_user_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_user_name));

	}
};

class SP_ACCOUNT_USER_DELETED_SELECT_BY_ACCOUNT : public DbSp
{
public:
   const int64_t mIn_p_account_id;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 4097, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_type = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_state = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_user_name = nullptr; // type: 254, flags: 16388, charset: 33 
           int64_t c_user_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_level_highest = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_ban = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_USER_DELETED_SELECT_BY_ACCOUNT(
		class DbSessionBase& session,
		const int64_t& in_p_account_id
	)
		:
		DbSp(L"{call sp_account_user_deleted_select_by_account(?)}", session),
		mIn_p_account_id(in_p_account_id)
	{
       if (false == session.IsReady())
           return;

       mOut.c_user_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_account_id);

		BindOut(mOut.c_id);
		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_type);
		BindOut(mOut.c_user_state);
		BindOutStr(mOut.c_user_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_user_level);
		BindOut(mOut.c_user_level_highest);
		BindOutDate(mOut.c_date_ban);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_ACCOUNT_USER_DELETED_SELECT_BY_ACCOUNT()
	{
     if (mOut.c_user_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_user_name));

	}
};

class SP_ACCOUNT_USER_GAME_INSERT : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_game_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_USER_GAME_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_game_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_user_game_insert(?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_game_id(in_p_game_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_game_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_ACCOUNT_USER_GAME_INSERT()
	{
	}
};

class SP_ACCOUNT_USER_GAME_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 20487, charset: 63 
           int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_USER_GAME_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_account_user_game_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_game_id);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_ACCOUNT_USER_GAME_SELECT()
	{
	}
};

class SP_ACCOUNT_USER_GAME_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 20487, charset: 63 
           int64_t c_game_id = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_USER_GAME_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_account_user_game_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_user_id);
		BindOut(mOut.c_game_id);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_ACCOUNT_USER_GAME_SELECT_ALL()
	{
	}
};

class SP_ACCOUNT_USER_GAME_UPDATE : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_game_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_USER_GAME_UPDATE(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_game_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_user_game_update(?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_game_id(in_p_game_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_game_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_ACCOUNT_USER_GAME_UPDATE()
	{
	}
};

class SP_ACCOUNT_USER_INSERT : public DbSp
{
public:
   const int64_t mIn_p_account_id;
   const int64_t mIn_p_server_id;
   const int64_t mIn_p_new_user_id;
   const int64_t mIn_p_user_type;
   const int64_t mIn_p_user_state;
   const wchar_t* mIn_p_user_name;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_new_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_USER_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_account_id,
		const int64_t& in_p_server_id,
		const int64_t& in_p_new_user_id,
		const int64_t& in_p_user_type,
		const int64_t& in_p_user_state,
		const wchar_t* in_p_user_name,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_user_insert(?, ?, ?, ?, ?, ?, ?)}", session),
		mIn_p_account_id(in_p_account_id),
		mIn_p_server_id(in_p_server_id),
		mIn_p_new_user_id(in_p_new_user_id),
		mIn_p_user_type(in_p_user_type),
		mIn_p_user_state(in_p_user_state),
		mIn_p_user_name(in_p_user_name),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_account_id);
		BindIn(mIn_p_server_id);
		BindIn(mIn_p_new_user_id);
		BindIn(mIn_p_user_type);
		BindIn(mIn_p_user_state);
		BindInStr(mIn_p_user_name);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_new_user_id);
	}
	~SP_ACCOUNT_USER_INSERT()
	{
	}
};

class SP_ACCOUNT_USER_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_server_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_type = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_state = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_user_name = nullptr; // type: 254, flags: 16388, charset: 33 
           int64_t c_user_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_level_highest = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_ban = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_USER_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_account_user_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_user_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_account_id);
		BindOut(mOut.c_server_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_type);
		BindOut(mOut.c_user_state);
		BindOutStr(mOut.c_user_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_user_level);
		BindOut(mOut.c_user_level_highest);
		BindOutDate(mOut.c_date_ban);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ACCOUNT_USER_SELECT()
	{
     if (mOut.c_user_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_user_name));

	}
};

class SP_ACCOUNT_USER_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_server_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_type = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_state = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_user_name = nullptr; // type: 254, flags: 16388, charset: 33 
           int64_t c_user_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_level_highest = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_ban = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_USER_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_account_user_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_user_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_account_id);
		BindOut(mOut.c_server_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_type);
		BindOut(mOut.c_user_state);
		BindOutStr(mOut.c_user_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_user_level);
		BindOut(mOut.c_user_level_highest);
		BindOutDate(mOut.c_date_ban);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ACCOUNT_USER_SELECT_ALL()
	{
     if (mOut.c_user_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_user_name));

	}
};

class SP_ACCOUNT_USER_SELECT_BY_ACCOUNT : public DbSp
{
public:
   const int64_t mIn_p_account_id;

	struct Out
	{
           int64_t c_account_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_server_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_type = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_state = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_user_name = nullptr; // type: 254, flags: 16388, charset: 33 
           int64_t c_user_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_level_highest = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_ban = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ACCOUNT_USER_SELECT_BY_ACCOUNT(
		class DbSessionBase& session,
		const int64_t& in_p_account_id
	)
		:
		DbSp(L"{call sp_account_user_select_by_account(?)}", session),
		mIn_p_account_id(in_p_account_id)
	{
       if (false == session.IsReady())
           return;

       mOut.c_user_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_account_id);

		BindOut(mOut.c_account_id);
		BindOut(mOut.c_server_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_user_type);
		BindOut(mOut.c_user_state);
		BindOutStr(mOut.c_user_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_user_level);
		BindOut(mOut.c_user_level_highest);
		BindOutDate(mOut.c_date_ban);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ACCOUNT_USER_SELECT_BY_ACCOUNT()
	{
     if (mOut.c_user_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_user_name));

	}
};

class SP_ACCOUNT_USER_UPDATE_BAN : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_user_state;
   const TIMESTAMP_STRUCT mIn_p_date_ban;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_USER_UPDATE_BAN(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_user_state,
		const TIMESTAMP_STRUCT& in_p_date_ban,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_user_update_ban(?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_user_state(in_p_user_state),
		mIn_p_date_ban(in_p_date_ban),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_user_state);
		BindInDate(mIn_p_date_ban);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_ACCOUNT_USER_UPDATE_BAN()
	{
	}
};

class SP_ACCOUNT_USER_UPDATE_EXPIRE : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_user_state;
   const TIMESTAMP_STRUCT mIn_p_date_ban;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_USER_UPDATE_EXPIRE(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_user_state,
		const TIMESTAMP_STRUCT& in_p_date_ban,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_user_update_expire(?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_user_state(in_p_user_state),
		mIn_p_date_ban(in_p_date_ban),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_user_state);
		BindInDate(mIn_p_date_ban);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_ACCOUNT_USER_UPDATE_EXPIRE()
	{
	}
};

class SP_ACCOUNT_USER_UPDATE_NAME : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const wchar_t* mIn_p_user_name;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_USER_UPDATE_NAME(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const wchar_t* in_p_user_name,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_user_update_name(?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_user_name(in_p_user_name),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindInStr(mIn_p_user_name);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_ACCOUNT_USER_UPDATE_NAME()
	{
	}
};

class SP_ACCOUNT_USER_UPDATE_STATE : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_user_state;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_user_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ACCOUNT_USER_UPDATE_STATE(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_user_state,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_account_user_update_state(?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_user_state(in_p_user_state),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_user_state);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_user_id);
	}
	~SP_ACCOUNT_USER_UPDATE_STATE()
	{
	}
};

