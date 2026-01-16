// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Random/RandMark40/CvcValidator.h"


bool CvcValidator::IsValidCvcPattern(const std::wstring& str) noexcept
{
	if (str.size() != 3)
		return false;

	// 대문자로 변환하여 검사
	wchar_t a = static_cast<wchar_t>(std::toupper(str[0]));
	wchar_t b = static_cast<wchar_t>(std::toupper(str[1]));
	wchar_t c = static_cast<wchar_t>(std::toupper(str[2]));

	// CVC 패턴 검사: 자음-모음-자음
	return IsInSet(a, C_SET.data(), C_SET_SIZE) && 
		   IsInSet(b, V_SET.data(), V_SET_SIZE) && 
		   IsInSet(c, C_SET.data(), C_SET_SIZE);
}

std::vector<std::wstring> CvcValidator::FilterGeneratableBadWords(const std::vector<std::wstring>& badWords) noexcept
{
	std::unordered_set<std::wstring> uniqueWords; // 소문자로 정규화 후 중복 제거
	uniqueWords.reserve(badWords.size() * 2);

	for (const auto& word : badWords)
	{
		if (!IsValidCvcPattern(word))
			continue;

		// 소문자로 변환하여 저장 (검색·비교용 일관성)
		std::wstring lowerWord = ToLower(word);
		uniqueWords.insert(lowerWord);
	}

	std::vector<std::wstring> result;
	result.reserve(uniqueWords.size());
	for (const auto& word : uniqueWords)
	{
		result.push_back(word);
	}
	std::sort(result.begin(), result.end()); // 정렬
	return result;
}

std::wstring CvcValidator::ToLower(const std::wstring& str) noexcept
{
	std::wstring result = str;
	for (auto& ch : result)
	{
		ch = static_cast<wchar_t>(std::tolower(ch));
	}
	return result;
}

std::wstring CvcValidator::ToUpper(const std::wstring& str) noexcept
{
	std::wstring result = str;
	for (auto& ch : result)
	{
		ch = static_cast<wchar_t>(std::toupper(ch));
	}
	return result;
}

bool CvcValidator::IsInSet(wchar_t ch, const wchar_t* set, size_t setSize) noexcept
{
	for (size_t i = 0; i < setSize; ++i)
	{
		if (set[i] == ch)
			return true;
	}
	return false;
}
