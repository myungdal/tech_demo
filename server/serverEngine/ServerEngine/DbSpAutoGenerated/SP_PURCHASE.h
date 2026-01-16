// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_PURCHASE_DELETED_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_purchase_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_guild_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_product_sid = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_product_receipt = nullptr; // type: 252, flags: 16, charset: 33 
           int64_t c_purchase_state = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_PURCHASE_DELETED_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_purchase_deleted_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_product_receipt = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_purchase_id);
		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_guild_id);
		BindOut(mOut.c_product_sid);
		BindOutStr(mOut.c_product_receipt, 65536 * sizeof(wchar_t));
		BindOut(mOut.c_purchase_state);
		BindOut(mOut.c_mail_id);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_PURCHASE_DELETED_SELECT()
	{
     if (mOut.c_product_receipt)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_product_receipt));

	}
};

class SP_PURCHASE_DELETED_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_purchase_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_guild_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_product_sid = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_product_receipt = nullptr; // type: 252, flags: 16, charset: 33 
           int64_t c_purchase_state = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_PURCHASE_DELETED_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_purchase_deleted_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_product_receipt = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_id);
		BindOut(mOut.c_purchase_id);
		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_guild_id);
		BindOut(mOut.c_product_sid);
		BindOutStr(mOut.c_product_receipt, 65536 * sizeof(wchar_t));
		BindOut(mOut.c_purchase_state);
		BindOut(mOut.c_mail_id);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_PURCHASE_DELETED_SELECT_ALL()
	{
     if (mOut.c_product_receipt)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_product_receipt));

	}
};

class SP_PURCHASE_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_purchase_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_guild_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_product_sid = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_product_receipt = nullptr; // type: 252, flags: 16, charset: 33 
           int64_t c_purchase_state = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_PURCHASE_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_purchase_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_product_receipt = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_purchase_id);
		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_guild_id);
		BindOut(mOut.c_product_sid);
		BindOutStr(mOut.c_product_receipt, 65536 * sizeof(wchar_t));
		BindOut(mOut.c_purchase_state);
		BindOut(mOut.c_mail_id);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_PURCHASE_SELECT()
	{
     if (mOut.c_product_receipt)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_product_receipt));

	}
};

class SP_PURCHASE_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_purchase_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_account_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_guild_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_product_sid = 0; // type: 8, flags: 0, charset: 63 
           wchar_t* c_product_receipt = nullptr; // type: 252, flags: 16, charset: 33 
           int64_t c_purchase_state = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mail_id = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_PURCHASE_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_purchase_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_product_receipt = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_purchase_id);
		BindOut(mOut.c_account_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_guild_id);
		BindOut(mOut.c_product_sid);
		BindOutStr(mOut.c_product_receipt, 65536 * sizeof(wchar_t));
		BindOut(mOut.c_purchase_state);
		BindOut(mOut.c_mail_id);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_PURCHASE_SELECT_ALL()
	{
     if (mOut.c_product_receipt)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_product_receipt));

	}
};

