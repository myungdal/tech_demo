// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_ITEM_DELETE : public DbSp
{
public:
   const int64_t mIn_p_uuid;
   const int64_t mIn_p_item_id;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_uuid = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ITEM_DELETE(
		class DbSessionBase& session,
		const int64_t& in_p_uuid,
		const int64_t& in_p_item_id,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_item_delete(?, ?, ?)}", session),
		mIn_p_uuid(in_p_uuid),
		mIn_p_item_id(in_p_item_id),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);
		BindIn(mIn_p_item_id);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_uuid);
	}
	~SP_ITEM_DELETE()
	{
	}
};

class SP_ITEM_DELETED_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_parent_item_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_parent_mail_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_inventory_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quick_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_equip_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_color_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_stat_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_enchant_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_state_flag = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_cooltime = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ITEM_DELETED_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_item_deleted_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_item_id);
		BindOut(mOut.c_parent_item_id);
		BindOut(mOut.c_parent_mail_id);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_inventory_slot_index);
		BindOut(mOut.c_quick_slot_index);
		BindOut(mOut.c_equip_slot_index);
		BindOut(mOut.c_item_quantity);
		BindOut(mOut.c_item_level);
		BindOut(mOut.c_item_color_union);
		BindOut(mOut.c_item_stat_union);
		BindOut(mOut.c_item_enchant_union);
		BindOut(mOut.c_item_state_flag);
		BindOutDate(mOut.c_date_cooltime);
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_ITEM_DELETED_SELECT()
	{
	}
};

class SP_ITEM_DELETED_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_user_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_parent_item_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_parent_mail_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_inventory_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quick_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_equip_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_color_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_stat_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_enchant_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_state_flag = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_cooltime = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_deleted = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ITEM_DELETED_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_item_deleted_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_id);
		BindOut(mOut.c_user_id);
		BindOut(mOut.c_item_id);
		BindOut(mOut.c_parent_item_id);
		BindOut(mOut.c_parent_mail_id);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_inventory_slot_index);
		BindOut(mOut.c_quick_slot_index);
		BindOut(mOut.c_equip_slot_index);
		BindOut(mOut.c_item_quantity);
		BindOut(mOut.c_item_level);
		BindOut(mOut.c_item_color_union);
		BindOut(mOut.c_item_stat_union);
		BindOut(mOut.c_item_enchant_union);
		BindOut(mOut.c_item_state_flag);
		BindOutDate(mOut.c_date_cooltime);
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
		BindOutDate(mOut.c_date_deleted);
	}
	~SP_ITEM_DELETED_SELECT_ALL()
	{
	}
};

class SP_ITEM_INSERT : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_new_item_id;
   const int64_t mIn_p_parent_item_id;
   const int64_t mIn_p_parent_mail_id;
   const int64_t mIn_p_item_sid;
   const int64_t mIn_p_inventory_slot_index;
   const int64_t mIn_p_quick_slot_index;
   const int64_t mIn_p_equip_slot_index;
   const int64_t mIn_p_item_quantity;
   const int64_t mIn_p_item_level;
   const int64_t mIn_p_item_color_union;
   const int64_t mIn_p_item_stat_union;
   const int64_t mIn_p_item_enchant_union;
   const int64_t mIn_p_item_state_flag;
   const TIMESTAMP_STRUCT mIn_p_date_cooltime;
   const TIMESTAMP_STRUCT mIn_p_date_expire;
   const TIMESTAMP_STRUCT mIn_p_date_updated;
   const TIMESTAMP_STRUCT mIn_p_date_created;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_new_item_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ITEM_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_new_item_id,
		const int64_t& in_p_parent_item_id,
		const int64_t& in_p_parent_mail_id,
		const int64_t& in_p_item_sid,
		const int64_t& in_p_inventory_slot_index,
		const int64_t& in_p_quick_slot_index,
		const int64_t& in_p_equip_slot_index,
		const int64_t& in_p_item_quantity,
		const int64_t& in_p_item_level,
		const int64_t& in_p_item_color_union,
		const int64_t& in_p_item_stat_union,
		const int64_t& in_p_item_enchant_union,
		const int64_t& in_p_item_state_flag,
		const TIMESTAMP_STRUCT& in_p_date_cooltime,
		const TIMESTAMP_STRUCT& in_p_date_expire,
		const TIMESTAMP_STRUCT& in_p_date_updated,
		const TIMESTAMP_STRUCT& in_p_date_created
	)
		:
		DbSp(L"{call sp_item_insert(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_new_item_id(in_p_new_item_id),
		mIn_p_parent_item_id(in_p_parent_item_id),
		mIn_p_parent_mail_id(in_p_parent_mail_id),
		mIn_p_item_sid(in_p_item_sid),
		mIn_p_inventory_slot_index(in_p_inventory_slot_index),
		mIn_p_quick_slot_index(in_p_quick_slot_index),
		mIn_p_equip_slot_index(in_p_equip_slot_index),
		mIn_p_item_quantity(in_p_item_quantity),
		mIn_p_item_level(in_p_item_level),
		mIn_p_item_color_union(in_p_item_color_union),
		mIn_p_item_stat_union(in_p_item_stat_union),
		mIn_p_item_enchant_union(in_p_item_enchant_union),
		mIn_p_item_state_flag(in_p_item_state_flag),
		mIn_p_date_cooltime(in_p_date_cooltime),
		mIn_p_date_expire(in_p_date_expire),
		mIn_p_date_updated(in_p_date_updated),
		mIn_p_date_created(in_p_date_created)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_new_item_id);
		BindIn(mIn_p_parent_item_id);
		BindIn(mIn_p_parent_mail_id);
		BindIn(mIn_p_item_sid);
		BindIn(mIn_p_inventory_slot_index);
		BindIn(mIn_p_quick_slot_index);
		BindIn(mIn_p_equip_slot_index);
		BindIn(mIn_p_item_quantity);
		BindIn(mIn_p_item_level);
		BindIn(mIn_p_item_color_union);
		BindIn(mIn_p_item_stat_union);
		BindIn(mIn_p_item_enchant_union);
		BindIn(mIn_p_item_state_flag);
		BindInDate(mIn_p_date_cooltime);
		BindInDate(mIn_p_date_expire);
		BindInDate(mIn_p_date_updated);
		BindInDate(mIn_p_date_created);

		BindOut(mOut.m_result);
		BindOut(mOut.p_new_item_id);
	}
	~SP_ITEM_INSERT()
	{
	}
};

class SP_ITEM_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_item_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_parent_item_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_parent_mail_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_inventory_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quick_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_equip_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_color_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_stat_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_enchant_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_state_flag = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_cooltime = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ITEM_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_item_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_item_id);
		BindOut(mOut.c_parent_item_id);
		BindOut(mOut.c_parent_mail_id);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_inventory_slot_index);
		BindOut(mOut.c_quick_slot_index);
		BindOut(mOut.c_equip_slot_index);
		BindOut(mOut.c_item_quantity);
		BindOut(mOut.c_item_level);
		BindOut(mOut.c_item_color_union);
		BindOut(mOut.c_item_stat_union);
		BindOut(mOut.c_item_enchant_union);
		BindOut(mOut.c_item_state_flag);
		BindOutDate(mOut.c_date_cooltime);
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ITEM_SELECT()
	{
	}
};

class SP_ITEM_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_item_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_parent_item_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_parent_mail_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_inventory_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quick_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_equip_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_color_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_stat_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_enchant_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_state_flag = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_cooltime = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ITEM_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_item_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_user_id);
		BindOut(mOut.c_item_id);
		BindOut(mOut.c_parent_item_id);
		BindOut(mOut.c_parent_mail_id);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_inventory_slot_index);
		BindOut(mOut.c_quick_slot_index);
		BindOut(mOut.c_equip_slot_index);
		BindOut(mOut.c_item_quantity);
		BindOut(mOut.c_item_level);
		BindOut(mOut.c_item_color_union);
		BindOut(mOut.c_item_stat_union);
		BindOut(mOut.c_item_enchant_union);
		BindOut(mOut.c_item_state_flag);
		BindOutDate(mOut.c_date_cooltime);
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ITEM_SELECT_ALL()
	{
	}
};

class SP_ITEM_SELECT_BY_USER : public DbSp
{
public:
   const int64_t mIn_p_user_id;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_item_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_parent_item_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_parent_mail_id = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_inventory_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_quick_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_equip_slot_index = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_quantity = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_level = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_color_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_stat_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_enchant_union = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_item_state_flag = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_cooltime = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_expire = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
           TIMESTAMP_STRUCT c_date_created = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ITEM_SELECT_BY_USER(
		class DbSessionBase& session,
		const int64_t& in_p_user_id
	)
		:
		DbSp(L"{call sp_item_select_by_user(?)}", session),
		mIn_p_user_id(in_p_user_id)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_item_id);
		BindOut(mOut.c_parent_item_id);
		BindOut(mOut.c_parent_mail_id);
		BindOut(mOut.c_item_sid);
		BindOut(mOut.c_inventory_slot_index);
		BindOut(mOut.c_quick_slot_index);
		BindOut(mOut.c_equip_slot_index);
		BindOut(mOut.c_item_quantity);
		BindOut(mOut.c_item_level);
		BindOut(mOut.c_item_color_union);
		BindOut(mOut.c_item_stat_union);
		BindOut(mOut.c_item_enchant_union);
		BindOut(mOut.c_item_state_flag);
		BindOutDate(mOut.c_date_cooltime);
		BindOutDate(mOut.c_date_expire);
		BindOutDate(mOut.c_date_updated);
		BindOutDate(mOut.c_date_created);
	}
	~SP_ITEM_SELECT_BY_USER()
	{
	}
};

class SP_ITEM_STAT_INSERT : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_item_id;
   const wchar_t* mIn_p_stat_type;
   const int64_t mIn_p_stat_value;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_item_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ITEM_STAT_INSERT(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_item_id,
		const wchar_t* in_p_stat_type,
		const int64_t& in_p_stat_value,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_item_stat_insert(?, ?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_item_id(in_p_item_id),
		mIn_p_stat_type(in_p_stat_type),
		mIn_p_stat_value(in_p_stat_value),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_item_id);
		BindInStr(mIn_p_stat_type);
		BindIn(mIn_p_stat_value);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_item_id);
	}
	~SP_ITEM_STAT_INSERT()
	{
	}
};

class SP_ITEM_STAT_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_item_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_stat_type = nullptr; // type: 254, flags: 20483, charset: 33 
           int64_t c_stat_value = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ITEM_STAT_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_item_stat_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_item_id);
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stat_value);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_ITEM_STAT_SELECT()
	{
     if (mOut.c_stat_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

	}
};

class SP_ITEM_STAT_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_item_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_stat_type = nullptr; // type: 254, flags: 20483, charset: 33 
           int64_t c_stat_value = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ITEM_STAT_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_item_stat_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_user_id);
		BindOut(mOut.c_item_id);
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stat_value);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_ITEM_STAT_SELECT_ALL()
	{
     if (mOut.c_stat_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

	}
};

class SP_ITEM_STAT_SELECT_BY_USER : public DbSp
{
public:
   const int64_t mIn_p_user_id;

	struct Out
	{
           int64_t c_user_id = 0; // type: 8, flags: 16392, charset: 63 
           int64_t c_item_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_stat_type = nullptr; // type: 254, flags: 20483, charset: 33 
           int64_t c_stat_value = 0; // type: 8, flags: 0, charset: 63 
           TIMESTAMP_STRUCT c_date_updated = {}; // type: 12, flags: 128, charset: 63 
	} mOut;

public:
	SP_ITEM_STAT_SELECT_BY_USER(
		class DbSessionBase& session,
		const int64_t& in_p_user_id
	)
		:
		DbSp(L"{call sp_item_stat_select_by_user(?)}", session),
		mIn_p_user_id(in_p_user_id)
	{
       if (false == session.IsReady())
           return;

       mOut.c_stat_type = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_user_id);

		BindOut(mOut.c_user_id);
		BindOut(mOut.c_item_id);
		BindOutStr(mOut.c_stat_type, 51 * sizeof(wchar_t));
		BindOut(mOut.c_stat_value);
		BindOutDate(mOut.c_date_updated);
	}
	~SP_ITEM_STAT_SELECT_BY_USER()
	{
     if (mOut.c_stat_type)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_stat_type));

	}
};

class SP_ITEM_STAT_UPDATE : public DbSp
{
public:
   const int64_t mIn_p_user_id;
   const int64_t mIn_p_item_id;
   const wchar_t* mIn_p_stat_type;
   const int64_t mIn_p_stat_value;
   const TIMESTAMP_STRUCT mIn_p_now;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_item_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ITEM_STAT_UPDATE(
		class DbSessionBase& session,
		const int64_t& in_p_user_id,
		const int64_t& in_p_item_id,
		const wchar_t* in_p_stat_type,
		const int64_t& in_p_stat_value,
		const TIMESTAMP_STRUCT& in_p_now
	)
		:
		DbSp(L"{call sp_item_stat_update(?, ?, ?, ?, ?)}", session),
		mIn_p_user_id(in_p_user_id),
		mIn_p_item_id(in_p_item_id),
		mIn_p_stat_type(in_p_stat_type),
		mIn_p_stat_value(in_p_stat_value),
		mIn_p_now(in_p_now)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_user_id);
		BindIn(mIn_p_item_id);
		BindInStr(mIn_p_stat_type);
		BindIn(mIn_p_stat_value);
		BindInDate(mIn_p_now);

		BindOut(mOut.m_result);
		BindOut(mOut.p_item_id);
	}
	~SP_ITEM_STAT_UPDATE()
	{
	}
};

class SP_ITEM_UPDATE : public DbSp
{
public:
   const int64_t mIn_p_item_id;
   const int64_t mIn_p_parent_item_id;
   const int64_t mIn_p_parent_mail_id;
   const int64_t mIn_p_item_sid;
   const int64_t mIn_p_inventory_slot_index;
   const int64_t mIn_p_quick_slot_index;
   const int64_t mIn_p_equip_slot_index;
   const int64_t mIn_p_item_quantity;
   const int64_t mIn_p_item_level;
   const int64_t mIn_p_item_color_union;
   const int64_t mIn_p_item_stat_union;
   const int64_t mIn_p_item_enchant_union;
   const int64_t mIn_p_item_state_flag;
   const TIMESTAMP_STRUCT mIn_p_date_cooltime;
   const TIMESTAMP_STRUCT mIn_p_date_expire;
   const TIMESTAMP_STRUCT mIn_p_date_updated;
   const TIMESTAMP_STRUCT mIn_p_date_created;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_item_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ITEM_UPDATE(
		class DbSessionBase& session,
		const int64_t& in_p_item_id,
		const int64_t& in_p_parent_item_id,
		const int64_t& in_p_parent_mail_id,
		const int64_t& in_p_item_sid,
		const int64_t& in_p_inventory_slot_index,
		const int64_t& in_p_quick_slot_index,
		const int64_t& in_p_equip_slot_index,
		const int64_t& in_p_item_quantity,
		const int64_t& in_p_item_level,
		const int64_t& in_p_item_color_union,
		const int64_t& in_p_item_stat_union,
		const int64_t& in_p_item_enchant_union,
		const int64_t& in_p_item_state_flag,
		const TIMESTAMP_STRUCT& in_p_date_cooltime,
		const TIMESTAMP_STRUCT& in_p_date_expire,
		const TIMESTAMP_STRUCT& in_p_date_updated,
		const TIMESTAMP_STRUCT& in_p_date_created
	)
		:
		DbSp(L"{call sp_item_update(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}", session),
		mIn_p_item_id(in_p_item_id),
		mIn_p_parent_item_id(in_p_parent_item_id),
		mIn_p_parent_mail_id(in_p_parent_mail_id),
		mIn_p_item_sid(in_p_item_sid),
		mIn_p_inventory_slot_index(in_p_inventory_slot_index),
		mIn_p_quick_slot_index(in_p_quick_slot_index),
		mIn_p_equip_slot_index(in_p_equip_slot_index),
		mIn_p_item_quantity(in_p_item_quantity),
		mIn_p_item_level(in_p_item_level),
		mIn_p_item_color_union(in_p_item_color_union),
		mIn_p_item_stat_union(in_p_item_stat_union),
		mIn_p_item_enchant_union(in_p_item_enchant_union),
		mIn_p_item_state_flag(in_p_item_state_flag),
		mIn_p_date_cooltime(in_p_date_cooltime),
		mIn_p_date_expire(in_p_date_expire),
		mIn_p_date_updated(in_p_date_updated),
		mIn_p_date_created(in_p_date_created)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_item_id);
		BindIn(mIn_p_parent_item_id);
		BindIn(mIn_p_parent_mail_id);
		BindIn(mIn_p_item_sid);
		BindIn(mIn_p_inventory_slot_index);
		BindIn(mIn_p_quick_slot_index);
		BindIn(mIn_p_equip_slot_index);
		BindIn(mIn_p_item_quantity);
		BindIn(mIn_p_item_level);
		BindIn(mIn_p_item_color_union);
		BindIn(mIn_p_item_stat_union);
		BindIn(mIn_p_item_enchant_union);
		BindIn(mIn_p_item_state_flag);
		BindInDate(mIn_p_date_cooltime);
		BindInDate(mIn_p_date_expire);
		BindInDate(mIn_p_date_updated);
		BindInDate(mIn_p_date_created);

		BindOut(mOut.m_result);
		BindOut(mOut.p_item_id);
	}
	~SP_ITEM_UPDATE()
	{
	}
};

class SP_ITEM_UPDATE_QUANTITY : public DbSp
{
public:
   const int64_t mIn_p_item_id;
   const int64_t mIn_p_item_quantity;
   const TIMESTAMP_STRUCT mIn_p_date_updated;

	struct Out
	{
           int64_t m_result = 0; // type: 8, flags: 0, charset: 63 
           int64_t p_item_id = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
   int64_t GetSpResult() const override { return mOut.m_result; }

public:
	SP_ITEM_UPDATE_QUANTITY(
		class DbSessionBase& session,
		const int64_t& in_p_item_id,
		const int64_t& in_p_item_quantity,
		const TIMESTAMP_STRUCT& in_p_date_updated
	)
		:
		DbSp(L"{call sp_item_update_quantity(?, ?, ?)}", session),
		mIn_p_item_id(in_p_item_id),
		mIn_p_item_quantity(in_p_item_quantity),
		mIn_p_date_updated(in_p_date_updated)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_item_id);
		BindIn(mIn_p_item_quantity);
		BindInDate(mIn_p_date_updated);

		BindOut(mOut.m_result);
		BindOut(mOut.p_item_id);
	}
	~SP_ITEM_UPDATE_QUANTITY()
	{
	}
};

