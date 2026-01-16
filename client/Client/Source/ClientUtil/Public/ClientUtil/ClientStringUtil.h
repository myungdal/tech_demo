// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "CoreMinimal.h"


namespace ClientStringUtil
{
	_DLL_EXPORT Buf_wchar_t s_to_w(const FString& str);
	_DLL_EXPORT FString w_to_s(const wchar_t* wchar);
	_DLL_EXPORT FText w_to_t(const wchar_t* wchar);
}

