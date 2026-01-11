// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "TimeUtil.h"


bool TimeUtil::ValidateStrTime(const wchar_t* strTime)
{
	const std::wregex regex(LR"(\d{1,4}\.\d{1,2}\.\d{1,2} \d{1,2}:\d{1,2}:\d{1,2})");
	return std::regex_match(strTime, regex);
}

time_t TimeUtil::TT_FROM_STR(const wchar_t* strTime)
{
	const std::wstring stringTime(strTime);
	std::wstringstream streamTime(stringTime);
	std::tm tm;
	streamTime >> std::get_time(&tm, L"%Y-%m-%d %H:%M:%S");
	return _mkgmtime(&tm);
}

time_t TimeUtil::TT_FROM_NUM(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t min, int32_t sec)
{
	std::tm tm = {};

	tm.tm_year = year - 1900; // tm_year는 1900 기준
	tm.tm_mon = month - 1; // 0~11
	tm.tm_mday = day; // 1~31
	tm.tm_hour = hour; // 0~23
	tm.tm_min = min; // 0~59
	tm.tm_sec = sec; // 0~60 (윤초)

	tm.tm_isdst = 0; // 서머타임 사용 안 함

	return _mkgmtime(&tm);
}

time_t TimeUtil::TT_FROM_TS(const TIMESTAMP_STRUCT& ts)
{
	if (1970 >= ts.year)
		return 0;

	std::tm tm;
	tm.tm_isdst = 0;
	tm.tm_yday = 0;
	tm.tm_wday = 0;

	tm.tm_year = ts.year - 1900;
	tm.tm_mon = ts.month - 1;
	tm.tm_mday = ts.day;
	tm.tm_hour = ts.hour;
	tm.tm_min = ts.minute;
	tm.tm_sec = ts.second;

	return _mkgmtime(&tm);
}

TIMESTAMP_STRUCT TimeUtil::TS_FROM_TT(const time_t& tt)
{
	tm tm;
	_gmtime64_s(&tm, &tt);

	TIMESTAMP_STRUCT ts;
	if (70 >= tm.tm_year)
	{
		memset(&ts, 0, sizeof(TIMESTAMP_STRUCT));
		return ts;
	}

	ts.year = (SQLSMALLINT)tm.tm_year + 1900;
	ts.month = (SQLSMALLINT)tm.tm_mon + 1;
	ts.day = (SQLSMALLINT)tm.tm_mday;
	ts.hour = (SQLSMALLINT)tm.tm_hour;
	ts.minute = (SQLSMALLINT)tm.tm_min;
	ts.second = (SQLSMALLINT)tm.tm_sec;
	ts.fraction = 0;

	return ts;
}
