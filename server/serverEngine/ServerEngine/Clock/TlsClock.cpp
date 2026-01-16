// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "TlsClock.h"

import ServerEngineCore;
import ServerEngineNetwork;


TlsClock::TlsClock()
{
	Update();
}
void TlsClock::Update()
{
	tClockInternalMembers.mLocalNow = CLOCK_NOW;
	tClockInternalMembers.mLocalNowMs = CLOCK_MS_FROM_DR(tClockInternalMembers.mLocalNow.time_since_epoch());
	mLocalNowTt = nullptr;
	mLocalNowTm = nullptr;
	mLocalNowTs = nullptr;

	tClockInternalMembers.mGlobalNow = gNetworkManager ? gNetworkManager->GetGlobalNow() : tClockInternalMembers.mLocalNow;
	tClockInternalMembers.mGlobalNowMs = CLOCK_MS_FROM_DR(tClockInternalMembers.mGlobalNow.time_since_epoch());
	mGlobalNowTt = nullptr;
	mGlobalNowTm = nullptr;
	mGlobalNowTs = nullptr;
}

const ClockPoint& TlsClock::GetLocalNow() const
{ 
	return tClockInternalMembers.mLocalNow;
}
	
const ClockMs& TlsClock::GetLocalNowMs() const
{ 
	return tClockInternalMembers.mLocalNowMs;
}

const time_t& TlsClock::GetLocalNowTt()
{
	if (mLocalNowTt)
		return *mLocalNowTt;

	tClockInternalMembers.mLocalNowTt = CLOCK_POINT_TO_TT(tClockInternalMembers.mLocalNow);

	mLocalNowTt = &tClockInternalMembers.mLocalNowTt;

	return *mLocalNowTt;
}

const tm& TlsClock::GetLocalNowTm()
{
	if (mLocalNowTm)
		return *mLocalNowTm;

	const time_t& tt = GetLocalNowTt();

	_gmtime64_s(&tClockInternalMembers.mLocalNowTm, &tt);

	mLocalNowTm = &tClockInternalMembers.mLocalNowTm;

	return *mLocalNowTm;
}

const TIMESTAMP_STRUCT& TlsClock::GetLocalNowTs()
{
	if (mLocalNowTs)
		return *mLocalNowTs;

	const tm& tm = GetLocalNowTm();

	if (70 >= tm.tm_year)
	{
		memset(&tClockInternalMembers.mLocalNowTs, 0, sizeof(TIMESTAMP_STRUCT));
	}
	else
	{
		tClockInternalMembers.mLocalNowTs.year = (SQLSMALLINT)tm.tm_year + 1900;
		tClockInternalMembers.mLocalNowTs.month = (SQLSMALLINT)tm.tm_mon + 1;
		tClockInternalMembers.mLocalNowTs.day = (SQLSMALLINT)tm.tm_mday;
		tClockInternalMembers.mLocalNowTs.hour = (SQLSMALLINT)tm.tm_hour;
		tClockInternalMembers.mLocalNowTs.minute = (SQLSMALLINT)tm.tm_min;
		tClockInternalMembers.mLocalNowTs.second = (SQLSMALLINT)tm.tm_sec;
		tClockInternalMembers.mLocalNowTs.fraction = 0;
	}

	mLocalNowTs = &tClockInternalMembers.mLocalNowTs;

	return *mLocalNowTs;
}

const ClockPoint& TlsClock::GetGlobalNow() const 
{ 
	return tClockInternalMembers.mGlobalNow;
}

const ClockMs& TlsClock::GetGlobalNowMs() const 
{ 
	return tClockInternalMembers.mGlobalNowMs;
}

const time_t& TlsClock::GetGlobalNowTt()
{
	if (mGlobalNowTt)
		return *mGlobalNowTt;

	tClockInternalMembers.mGlobalNowTt = CLOCK_POINT_TO_TT(tClockInternalMembers.mGlobalNow);

	mGlobalNowTt = &tClockInternalMembers.mGlobalNowTt;

	return *mGlobalNowTt;
}

const tm& TlsClock::GetGlobalNowTm()
{
	if (mGlobalNowTm)
		return *mGlobalNowTm;

	const time_t& tt = GetGlobalNowTt();

	_gmtime64_s(&tClockInternalMembers.mGlobalNowTm, &tt);

	mGlobalNowTm = &tClockInternalMembers.mGlobalNowTm;

	return *mGlobalNowTm;
}

const TIMESTAMP_STRUCT& TlsClock::GetGlobalNowTs()
{
	if (mGlobalNowTs)
		return *mGlobalNowTs;

	const tm& tm = GetGlobalNowTm();

	if (70 >= tm.tm_year)
	{
		memset(&tClockInternalMembers.mGlobalNowTs, 0, sizeof(TIMESTAMP_STRUCT));
	}
	else
	{
		tClockInternalMembers.mGlobalNowTs.year = (SQLSMALLINT)tm.tm_year + 1900;
		tClockInternalMembers.mGlobalNowTs.month = (SQLSMALLINT)tm.tm_mon + 1;
		tClockInternalMembers.mGlobalNowTs.day = (SQLSMALLINT)tm.tm_mday;
		tClockInternalMembers.mGlobalNowTs.hour = (SQLSMALLINT)tm.tm_hour;
		tClockInternalMembers.mGlobalNowTs.minute = (SQLSMALLINT)tm.tm_min;
		tClockInternalMembers.mGlobalNowTs.second = (SQLSMALLINT)tm.tm_sec;
		tClockInternalMembers.mGlobalNowTs.fraction = 0;
	}

	mGlobalNowTs = &tClockInternalMembers.mGlobalNowTs;

	return *mGlobalNowTs;
}
