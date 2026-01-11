// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "ServerEngine/DbSpBase/DbSp.h"

class SP_SERVER_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_server_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_server_name = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_home_game_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_server_state = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_wau_count_max = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_wau_count_busy = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_ccu_count_max = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_ccu_count_busy = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_SERVER_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_server_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;

       mOut.c_server_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));

		BindIn(mIn_p_uuid);

		BindOut(mOut.c_server_id);
		BindOutStr(mOut.c_server_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_home_game_sid);
		BindOut(mOut.c_server_state);
		BindOut(mOut.c_wau_count_max);
		BindOut(mOut.c_wau_count_busy);
		BindOut(mOut.c_ccu_count_max);
		BindOut(mOut.c_ccu_count_busy);
	}
	~SP_SERVER_SELECT()
	{
     if (mOut.c_server_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_server_name));

	}
};

class SP_SERVER_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_server_id = 0; // type: 8, flags: 20483, charset: 63 
           wchar_t* c_server_name = nullptr; // type: 254, flags: 0, charset: 33 
           int64_t c_home_game_sid = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_server_state = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_wau_count_max = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_wau_count_busy = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_ccu_count_max = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_ccu_count_busy = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_SERVER_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_server_select_all()}", session)
	{
       if (false == session.IsReady())
           return;

       mOut.c_server_name = reinterpret_cast<wchar_t*>(PacketStringBuffer::Pop(__FUNCTIONW__));


		BindOut(mOut.c_server_id);
		BindOutStr(mOut.c_server_name, 51 * sizeof(wchar_t));
		BindOut(mOut.c_home_game_sid);
		BindOut(mOut.c_server_state);
		BindOut(mOut.c_wau_count_max);
		BindOut(mOut.c_wau_count_busy);
		BindOut(mOut.c_ccu_count_max);
		BindOut(mOut.c_ccu_count_busy);
	}
	~SP_SERVER_SELECT_ALL()
	{
     if (mOut.c_server_name)
         PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mOut.c_server_name));

	}
};

class SP_SERVER_STATISTICS_SELECT : public DbSp
{
public:
   const int64_t mIn_p_uuid;

	struct Out
	{
           int64_t c_server_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_tuc_count = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mau_count = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_wau_count = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_ccu_count = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_SERVER_STATISTICS_SELECT(
		class DbSessionBase& session,
		const int64_t& in_p_uuid
	)
		:
		DbSp(L"{call sp_server_statistics_select(?)}", session),
		mIn_p_uuid(in_p_uuid)
	{
       if (false == session.IsReady())
           return;


		BindIn(mIn_p_uuid);

		BindOut(mOut.c_server_id);
		BindOut(mOut.c_tuc_count);
		BindOut(mOut.c_mau_count);
		BindOut(mOut.c_wau_count);
		BindOut(mOut.c_ccu_count);
	}
	~SP_SERVER_STATISTICS_SELECT()
	{
	}
};

class SP_SERVER_STATISTICS_SELECT_ALL : public DbSp
{
public:

	struct Out
	{
           int64_t c_server_id = 0; // type: 8, flags: 20483, charset: 63 
           int64_t c_tuc_count = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_mau_count = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_wau_count = 0; // type: 8, flags: 0, charset: 63 
           int64_t c_ccu_count = 0; // type: 8, flags: 0, charset: 63 
	} mOut;

public:
	SP_SERVER_STATISTICS_SELECT_ALL(
		class DbSessionBase& session
	)
		:
		DbSp(L"{call sp_server_statistics_select_all()}", session)
	{
       if (false == session.IsReady())
           return;



		BindOut(mOut.c_server_id);
		BindOut(mOut.c_tuc_count);
		BindOut(mOut.c_mau_count);
		BindOut(mOut.c_wau_count);
		BindOut(mOut.c_ccu_count);
	}
	~SP_SERVER_STATISTICS_SELECT_ALL()
	{
	}
};

