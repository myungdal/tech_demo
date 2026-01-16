// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "StringUtil.h"

#include <cwctype>


void StringUtil::ToLower(std::wstring& str)
{
	std::transform(str.begin(), str.end(), str.begin(), 
		[](wchar_t ch) {
			return static_cast<wchar_t>(std::towlower(ch));
		});
}

void StringUtil::ToUpper(std::wstring& str)
{
	std::transform(str.begin(), str.end(), str.begin(), 
		[](wchar_t ch) {
			return static_cast<wchar_t>(std::towupper(ch));
		});
}

std::wstring StringUtil::ToLowerCopy(const std::wstring& str)
{
	std::wstring result = str;
	ToLower(result);
	return result;
}

std::wstring StringUtil::ToUpperCopy(const std::wstring& str)
{
	std::wstring result = str;
	ToUpper(result);
	return result;
}

bool StringUtil::CompareCaseInSensitive(const std::wstring& a, const std::wstring& b)
{
	// 원본을 변경하지 않고 복사본으로 비교
	std::wstring lowerA = ToLowerCopy(a);
	std::wstring lowerB = ToLowerCopy(b);
	return (lowerA == lowerB);
}

bool StringUtil::CompareCaseInSensitive(const wchar_t* a, const wchar_t* b)
{
	std::wstring strA = a;
	std::wstring strB = b;
	return StringUtil::CompareCaseInSensitive(strA, strB);
}


Buf_wchar_t StringUtil::w_to_w(const wchar_t* w)
{
	Buf_wchar_t _w;
	wcscpy_s(*_w, MAX_STRING_LENGTH, w);
	return Buf_wchar_t(_w);
}

Buf_char StringUtil::a_to_a(const char* a)
{
	Buf_char _a;
	strcpy_s(*_a, MAX_STRING_LENGTH, a);
	return Buf_char(_a);
}

Buf_wchar_t StringUtil::a_to_w(const char* a)
{
	Buf_wchar_t w;
	const int len = MultiByteToWideChar(CP_ACP, 0, a, -1, *w, MAX_STRING_LENGTH);
	if (0 == len)
	{
		(*w)[0] = 0;
		_DEBUG_BREAK;
		_DEBUG_LOG(RED, L"{}", GetLastError());
	}
	return Buf_wchar_t(w);
}

Buf_char StringUtil::w_to_a(const wchar_t* w)
{
	Buf_char a;
	const int len = WideCharToMultiByte(CP_ACP, 0, w, -1, *a, MAX_STRING_LENGTH, nullptr, nullptr);
	if (0 == len)
	{
		(*a)[0] = 0;
		_DEBUG_BREAK;
		_DEBUG_LOG(RED, L"{}", GetLastError());
	}
	return Buf_char(a);
}

Buf_wchar_t StringUtil::utf8_to_w(const char* uft8)
{
	Buf_wchar_t w;
	const int len = MultiByteToWideChar(CP_UTF8, 0, uft8, -1, *w, MAX_STRING_LENGTH);
	if (0 == len)
	{
		(*w)[0] = 0;
		_DEBUG_BREAK;
		_DEBUG_LOG(RED, L"{}", GetLastError());
	}
	return Buf_wchar_t(w);
}

Buf_char StringUtil::w_to_utf8(const wchar_t* w)
{
	Buf_char utf8;
	const int len = WideCharToMultiByte(CP_UTF8, 0, w, -1, *utf8, MAX_STRING_LENGTH, nullptr, nullptr);
	if (0 == len)
	{
		(*utf8)[0] = 0;
		_DEBUG_BREAK;
		_DEBUG_LOG(RED, L"{}", GetLastError());
	}
	return Buf_char(utf8);
}

bool StringUtil::ValidateName(const wchar_t* name, size_t lengthMin, size_t lengthMax)
{
	constexpr wchar_t MIN_WCHAR_NUMBER = 48;					// 숫자 시작 값
	constexpr wchar_t MAX_WCHAR_NUMBER = 57;					// 숫자 끝 값

	constexpr wchar_t MIN_WCHAR_ENGLISH_UPPER_CASE = 65;		// 영문 대문자 시작 값 'A'
	constexpr wchar_t MAX_WCHAR_ENGLISH_UPPER_CASE = 90;		// 영문 대문자 끝 값   'Z'

	constexpr wchar_t MIN_WCHAR_ENGLISH_LOWER_CASE = 97;		// 영문 소문자 시작 값 'a'
	constexpr wchar_t MAX_WCHAR_ENGLISH_LOWER_CASE = 122;		// 영문 소문자 끝 값   'z'

	constexpr wchar_t MIN_WCHAR_KOREAN = 44032;				// 한글 시작 값
	constexpr wchar_t MAX_WCHAR_KOREAN = 55203;				// 한글 끝 값

	//constexpr wchar_t MIN_WCHAR_KOREAN_CONSONANT = 12593;	// 한글 초성 시작 값
	//constexpr wchar_t MAX_WCHAR_KOREAN_CONSONANT = 12622;	// 한글 초성 끝 값

	//constexpr wchar_t MIN_WCHAR_KOREAN_VOWEL = 12623;		// 한글 중성 시작 값
	//constexpr wchar_t MAX_WCHAR_KOREAN_VOWEL = 12643;		// 한글 중성 끝 값

	const size_t length = std::wcslen(name);

	// 길이 0은 불허
	if (0 == length)
		return false;

	// 최대 허용 길이를 넘었음
	if (lengthMax < length)
		return false;

	// 정책에 따른 길이 
	size_t lengthForValidation = 0;

	// 정책에 따라, 한글자씩 살펴보면서 길이 검증한다
	for (size_t i = 0; i < length; ++i)
	{
		const wchar_t& ch = name[i];

		// 숫자일 경우, 길이 1로 계산
		if ((MIN_WCHAR_NUMBER <= ch) && (MAX_WCHAR_NUMBER >= ch))
		{
			++lengthForValidation;
		}
		// 영문 대문자일 경우, 길이 1로 계산 (A-Z: 65-90)
		else if ((MIN_WCHAR_ENGLISH_UPPER_CASE <= ch) && (MAX_WCHAR_ENGLISH_UPPER_CASE >= ch))
		{
			++lengthForValidation;
		}
		// 영문 소문자일 경우, 길이 1로 계산 (a-z: 97-122)
		else if ((MIN_WCHAR_ENGLISH_LOWER_CASE <= ch) && (MAX_WCHAR_ENGLISH_LOWER_CASE >= ch))
		{
			++lengthForValidation;
		}
		// 한글일 경우, 길이 2로 계산
		else if ((MIN_WCHAR_KOREAN <= ch) && (MAX_WCHAR_KOREAN >= ch))
		{
			lengthForValidation += 2;
		}
		//// 한글 자음일 경우, 길이 2로 계산
		//else if ((MIN_WCHAR_KOREAN_CONSONANT <= ch) && (MAX_WCHAR_KOREAN_CONSONANT >= ch))
		//{
		//	lengthForValidation += 2;
		//}
		//// 한글 모음일 경우, 길이 2로 계산
		//else if ((MIN_WCHAR_KOREAN_VOWEL <= ch) && (MAX_WCHAR_KOREAN_VOWEL >= ch))
		//{
		//	lengthForValidation += 2;
		//}
		// 이 외에는 허용되지 않은 문자이다
		else
		{
			return false;
		}

		// 최대 허용 길이를 넘었음
		if (lengthMax < lengthForValidation)
			return false;
	}

	// 최소 허용 길이보다 짧음
	if (lengthMin > lengthForValidation)
		return false;

	// 성공
	return true;
}


#if 0

#include <cstdlib>
#include <iostream>

std::wstring Utf8ToUtf16(const std::string_view str)
{
	if (str.empty())
		return {};

	std::wstring out(str.size() * sizeof(char), '\0');
	if (::mbstowcs_s(nullptr, out.data(), out.size() + 1, str.data(), _TRUNCATE) != 0)
		return {};
	out.resize(std::char_traits<wchar_t>::length(out.data()));
	return out;
}
std::string Utf16ToUtf8(const std::wstring_view wstr)
{
	if (wstr.empty())
		return {};
	// 변환에 필요한 사이즈 확보
	std::string out(wstr.size() * sizeof(wchar_t), '\0');

	// 멀티바이트 문자열로 변환
	if (::wcstombs_s(nullptr, out.data(), out.size() + 1, wstr.data(), _TRUNCATE) != 0)
		return {};

	// 문자열의 말단에서 뒤쪽의 불필요한 부분을 삭제한다
	out.resize(std::char_traits<char>::length(out.data()));

	return out;
}

int main()
{
	// 로케일 설정
	std::ios_base::sync_with_stdio(false);
	const std::locale defaultLoc("");
	std::locale::global(defaultLoc);
	const std::locale ctypeDefault(
		std::locale::classic(), defaultLoc, std::locale::ctype);
	std::wcout.imbue(ctypeDefault);
	std::wcin.imbue(ctypeDefault);

	// 문자열을 변환
	const auto wstr = Utf8ToUtf16("abcdeㄱㄴㄷ");
	const auto str = Utf16ToUtf8(L"fghij가나다");

	std::wcout << wstr << std::endl;
	std::cout << str << std::endl;

	return 0;
}

#endif
