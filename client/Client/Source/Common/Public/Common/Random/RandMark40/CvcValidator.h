// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <cctype>

#include "Common/Random/RandMark40/RandMarkDef.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CVC 패턴 검증기 (CVC Pattern Validator)
// 자음-모음-자음 패턴의 문자열을 검증하고 필터링하는 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class COMMON_API CvcValidator final
{
private:
	// 공통 상수는 RandMarkDef.h에서 가져옴
	static constexpr auto& C_SET = RandMark::CONSONANTS;
	static constexpr auto& V_SET = RandMark::VOWELS;
	static constexpr size_t C_SET_SIZE = RandMark::CONSONANT_COUNT;
	static constexpr size_t V_SET_SIZE = RandMark::VOWEL_COUNT;

public:
	CvcValidator() = default;
	~CvcValidator() = default;

	// CVC 패턴 검증 (3글자 문자열이 자음-모음-자음 패턴인지 확인)
	bool IsValidCvcPattern(const std::wstring& str) noexcept;
	
	// 금칙어 목록에서 실제 생성 가능한 CVC만 필터링하여 반환
	std::vector<std::wstring> FilterGeneratableBadWords(const std::vector<std::wstring>& badWords) noexcept;
	
	// 문자열을 소문자로 변환
	static std::wstring ToLower(const std::wstring& str) noexcept;
	
	// 문자열을 대문자로 변환
	static std::wstring ToUpper(const std::wstring& str) noexcept;

private:
	// 문자가 지정된 집합에 속하는지 확인
	bool IsInSet(wchar_t ch, const wchar_t* set, size_t setSize) noexcept;
};
