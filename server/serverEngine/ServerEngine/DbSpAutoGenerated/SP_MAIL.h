// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_MAIL_DELETE : public DbSp
{
public:
   const int64_t mIn_p_uuid;
   const int64_t mIn_p_mail_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_uuid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_MAIL_DELETE(
		class DbSessionBase& session,
		const int64_t& in_p_uuid,
		const int64_t& in_p_mail_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_mail_delete(?, ?, ?)}", session),
		mIn_p_uuid(in_p_uuid),
		mIn_p_mail_id(in_p_mail_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);
		BindIn(mIn_p_mail_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_uuid);
	}
	~SP_MAIL_DELETE()
	{
	}
};

class SP_MAIL_DELETED_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_read_flag = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_flag = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_title = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg_args = nullptr; // type: 254, flags: 0, charset: 33 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_MAIL_DELETED_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_mail_deleted_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_title = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg_args = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_mail_id);
		BindOut(mOut.c_mail_sid);
		BindOut(mOut.c_read_flag);
		BindOut(mOut.c_reward_flag);
		BindOutStr(mOut.c_title, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg_args, 251 * sizeof(wchar_t));
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_MAIL_DELETED_SELECT()
	{
     if (mOut.c_title)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_title));

     if (mOut.c_msg)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg));

     if (mOut.c_msg_args)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg_args));

	}
};

class SP_MAIL_DELETED_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_read_flag = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_flag = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_title = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg_args = nullptr; // type: 254, flags: 0, charset: 33 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_MAIL_DELETED_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_mail_deleted_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_title = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg_args = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_mail_id);
		BindOut(mOut.c_mail_sid);
		BindOut(mOut.c_read_flag);
		BindOut(mOut.c_reward_flag);
		BindOutStr(mOut.c_title, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg_args, 251 * sizeof(wchar_t));
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_MAIL_DELETED_SELECT_ALL()
	{
     if (mOut.c_title)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_title));

     if (mOut.c_msg)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg));

     if (mOut.c_msg_args)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg_args));

	}
};

class SP_MAIL_INSERT : public DbSp
{
public:
   const int64_t mIn_p_new_mail_id;
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_mail_sid;
   const wchar_t* mIn_p_title;
   const wchar_t* mIn_p_msg;
   const wchar_t* mIn_p_msg_args;
   const int64_t mIn_p_reward_sid;
   const TIMESTAMP_STRUCT mIn_p_date_expire;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_new_mail_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_MAIL_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_new_mail_id,
		const int64_t& in_p_user_id,
		const int64_t& in_p_mail_sid,
		const wchar_t* in_p_title,
		const wchar_t* in_p_msg,
		const wchar_t* in_p_msg_args,
		const int64_t& in_p_reward_sid,
		const TIMESTAMP_STRUCT& in_p_date_expire,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_mail_insert(?, ?, ?, ?, ?, ?, ?, ?, ?)}", session),
		mIn_p_new_mail_id(in_p_new_mail_id),
		mIn_p_user_id(in_p_user_id),
		mIn_p_mail_sid(in_p_mail_sid),
		mIn_p_title(in_p_title),
		mIn_p_msg(in_p_msg),
		mIn_p_msg_args(in_p_msg_args),
		mIn_p_reward_sid(in_p_reward_sid),
		mIn_p_date_expire(in_p_date_expire),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_new_mail_id);
		BindIn(mIn_p_user_id);
		BindIn(mIn_p_mail_sid);
		BindInStr(mIn_p_title);
		BindInStr(mIn_p_msg);
		BindInStr(mIn_p_msg_args);
		BindIn(mIn_p_reward_sid);
		BindInDate(mIn_p_date_expire);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_new_mail_id);
	}
	~SP_MAIL_INSERT()
	{
	}
};

class SP_MAIL_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_mail_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_read_flag = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_flag = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_title = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg_args = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_reward_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 16520, charset: 63 
	} mOut;

public:
	SP_MAIL_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_mail_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_title = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg_args = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_mail_id);
		BindOut(mOut.c_mail_sid);
		BindOut(mOut.c_read_flag);
		BindOut(mOut.c_reward_flag);
		BindOutStr(mOut.c_title, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg_args, 251 * sizeof(wchar_t));
		BindOut(mOut.c_reward_sid);
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_MAIL_SELECT()
	{
     if (mOut.c_title)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_title));

     if (mOut.c_msg)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg));

     if (mOut.c_msg_args)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg_args));

	}
};

class SP_MAIL_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_mail_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_read_flag = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_flag = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_title = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg_args = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_reward_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 16520, charset: 63 
	} mOut;

public:
	SP_MAIL_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_mail_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_title = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg_args = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_user_id);
		BindOut(mOut.c_mail_id);
		BindOut(mOut.c_mail_sid);
		BindOut(mOut.c_read_flag);
		BindOut(mOut.c_reward_flag);
		BindOutStr(mOut.c_title, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg_args, 251 * sizeof(wchar_t));
		BindOut(mOut.c_reward_sid);
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_MAIL_SELECT_ALL()
	{
     if (mOut.c_title)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_title));

     if (mOut.c_msg)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg));

     if (mOut.c_msg_args)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg_args));

	}
};

class SP_MAIL_SELECT_BY_USER : public DbSp
{
public:
   const int64_t mIn_p_user_id;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_mail_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_read_flag = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_reward_flag = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_title = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg = nullptr; // type: 254, flags: 0, charset: 33 
           wchar_t* c_msg_args = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_reward_sid = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 16520, charset: 63 
	} mOut;

public:
	SP_MAIL_SELECT_BY_USER(
		class DbSessionBase& session,
		const int64_t& in_p_user_id
	)
		:
		DbSp(L"{call sp_mail_select_by_user(?)}", session),
		mIn_p_user_id(in_p_user_id)
	{
       if (false == session.IsReady())
           return;

       mOut.c_title = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));
       mOut.c_msg_args = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_user_id);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_mail_id);
		BindOut(mOut.c_mail_sid);
		BindOut(mOut.c_read_flag);
		BindOut(mOut.c_reward_flag);
		BindOutStr(mOut.c_title, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg, 251 * sizeof(wchar_t));
		BindOutStr(mOut.c_msg_args, 251 * sizeof(wchar_t));
		BindOut(mOut.c_reward_sid);
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_MAIL_SELECT_BY_USER()
	{
     if (mOut.c_title)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_title));

     if (mOut.c_msg)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg));

     if (mOut.c_msg_args)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_msg_args));

	}
};

class SP_MAIL_UPDATE : public DbSp
{
public:
   const int64_t mIn_p_mail_id;
   const int64_t mIn_p_read_flag;
   const int64_t mIn_p_reward_flag;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_mail_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_MAIL_UPDATE(
		class DbSessionBase& session,
		const int64_t& in_p_mail_id,
		const int64_t& in_p_read_flag,
		const int64_t& in_p_reward_flag,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_mail_update(?, ?, ?, ?)}", session),
		mIn_p_mail_id(in_p_mail_id),
		mIn_p_read_flag(in_p_read_flag),
		mIn_p_reward_flag(in_p_reward_flag),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_mail_id);
		BindIn(mIn_p_read_flag);
		BindIn(mIn_p_reward_flag);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_mail_id);
	}
	~SP_MAIL_UPDATE()
	{
	}
};

class SP_MAIL_UPDATE_READ : public DbSp
{
public:
   const int64_t mIn_p_mail_id;
   const int64_t mIn_p_read_flag;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_mail_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_MAIL_UPDATE_READ(
		class DbSessionBase& session,
		const int64_t& in_p_mail_id,
		const int64_t& in_p_read_flag,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_mail_update_read(?, ?, ?)}", session),
		mIn_p_mail_id(in_p_mail_id),
		mIn_p_read_flag(in_p_read_flag),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_mail_id);
		BindIn(mIn_p_read_flag);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_mail_id);
	}
	~SP_MAIL_UPDATE_READ()
	{
	}
};

class SP_MAIL_UPDATE_REWARD : public DbSp
{
public:
   const int64_t mIn_p_mail_id;
   const int64_t mIn_p_read_flag;
   const int64_t mIn_p_reward_flag;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_mail_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_MAIL_UPDATE_REWARD(
		class DbSessionBase& session,
		const int64_t& in_p_mail_id,
		const int64_t& in_p_read_flag,
		const int64_t& in_p_reward_flag,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_mail_update_reward(?, ?, ?, ?)}", session),
		mIn_p_mail_id(in_p_mail_id),
		mIn_p_read_flag(in_p_read_flag),
		mIn_p_reward_flag(in_p_reward_flag),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_mail_id);
		BindIn(mIn_p_read_flag);
		BindIn(mIn_p_reward_flag);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_mail_id);
	}
	~SP_MAIL_UPDATE_REWARD()
	{
	}
};

