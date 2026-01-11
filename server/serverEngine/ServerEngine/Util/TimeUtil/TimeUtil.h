// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// TimeUtil.h - 시간 유틸리티
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeUtil
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace TimeUtil
{
	// 시간 문자열 유효성 검증
	bool ValidateStrTime(const wchar_t* strTime);
	
	// 문자열에서 time_t 변환
	time_t TT_FROM_STR(const wchar_t* strTime);
	
	// 숫자에서 time_t 변환
	time_t TT_FROM_NUM(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t min, int32_t sec);
	
	// TIMESTAMP_STRUCT에서 time_t 변환
	time_t TT_FROM_TS(const TIMESTAMP_STRUCT& ts);
	
	// time_t에서 TIMESTAMP_STRUCT 변환
	TIMESTAMP_STRUCT TS_FROM_TT(const time_t& tt);
}
