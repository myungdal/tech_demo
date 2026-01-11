// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// StringUtil.h - 문자열 유틸리티
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StringUtil
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace StringUtil
{
	// 문자열 소문자 변환 (In-place)
	void ToLower(std::wstring& str);
	
	// 문자열 대문자 변환 (In-place)
	void ToUpper(std::wstring& str);
	
	// 문자열 소문자 변환 (복사본 반환)
	std::wstring ToLowerCopy(const std::wstring& str);
	
	// 문자열 대문자 변환 (복사본 반환)
	std::wstring ToUpperCopy(const std::wstring& str);
	
	// 대소문자 무시 비교
	bool CompareCaseInSensitive(const std::wstring& a, const std::wstring& b);
	bool CompareCaseInSensitive(const wchar_t* a, const wchar_t* b);

	// 최대 문자열 길이
	constexpr size_t MAX_STRING_LENGTH = (MAX_PACKET_BUFFER_SIZE >> 1);

	// 문자열 인코딩 변환
	Buf_wchar_t w_to_w(const wchar_t* w);
	Buf_char a_to_a(const char* a);
	Buf_wchar_t a_to_w(const char* a);
	Buf_char w_to_a(const wchar_t* w);
	Buf_wchar_t utf8_to_w(const char* uft8);
	Buf_char w_to_utf8(const wchar_t* w);

	template<typename _JsonObj>
	Buf_wchar_t GetJsonStr(_JsonObj& jsonObj)
	{
		return StringUtil::a_to_w(jsonObj.template get<std::string>().c_str());
	}
	bool ValidateName(const wchar_t* name, size_t lengthMin, size_t lengthMax);

	inline int32_t w_to_int32(const wchar_t* w) { return std::stoi(w); }
	inline int64_t w_to_int64(const wchar_t* w) { return std::stoll(w); }

	template<typename... Args>
	inline std::tuple<Buf_wchar_t, int> FormatStr(const std::wformat_string<Args...> formatStr, Args&&... args)
	{
		Buf_wchar_t w = {};
		const auto result = std::format_to_n(*w, StringUtil::MAX_STRING_LENGTH - 1, formatStr, std::forward<Args>(args)...);

		(*w)[std::min(static_cast<size_t>(StringUtil::MAX_STRING_LENGTH - 1), static_cast<size_t>(result.size))] = L'\0';

		return std::make_tuple(w, static_cast<int>(result.size));
	}

}

#if 0
#define FormatStr(format, ...) \
	[&]() -> std::tuple<Buf_wchar_t, int> { \
		Buf_wchar_t w; \
		const auto result = std::format_to_n(*w, (StringUtil::MAX_STRING_LENGTH - 1), format, __VA_ARGS__); \
		(*w)[std::min(static_cast<size_t>(StringUtil::MAX_STRING_LENGTH - 1), static_cast<size_t>(result.size))] = 0; \
		return std::make_tuple(w, static_cast<int>(result.size)); \
	}()
#elif 0
template <typename... Args>
inline std::tuple<Buf_wchar_t, int> FormatStr(const wchar_t* format, Args&&... args) 
{
	Buf_wchar_t w;
	const auto result = std::format_to_n(
		*w,
		StringUtil::MAX_STRING_LENGTH - 1,
		format,
		std::forward<Args>(args)...
	);

	(*w)[std::min< size_t>(
		static_cast<size_t>(StringUtil::MAX_STRING_LENGTH - 1),
		static_cast<size_t>(result.size)
	)] = L'\0';

	return std::make_tuple(w, static_cast<int>(result.size));
}
#endif
