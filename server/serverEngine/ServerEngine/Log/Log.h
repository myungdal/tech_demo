// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Log : SLIST_ENTRY
{
	static constexpr size_t MAX_LOG_STR_LEN = 4096;

	uint16_t mColor = 0;
	wchar_t mStr[MAX_LOG_STR_LEN] = { 0 };
	size_t mOffsetForConsoleStr = 0;
	size_t mStrLen = 0;

	static Log* Acquire();
	void Dispose();

	Log* GetNext() const noexcept { return static_cast<Log*>(SLIST_ENTRY::Next); }

	template<typename _Color, typename... _Args>
	void Write(_Color color, const wchar_t* function, int line, const std::wformat_string<_Args...>& format, _Args&&... args)
	{
		const wchar_t* colorStr = L"";
		if (RED == color) colorStr = L"R!";
		else if (GREEN == color) colorStr = L"G!";
		else if (YELLOW == color) colorStr = L"Y!";
		else if (WHITE == color) colorStr = L"W!";

		mColor = color;

		tm localTm{};
		_localtime64_s(&localTm, &tClock.GetGlobalNowTt());

		constexpr size_t maxLen = Log::MAX_LOG_STR_LEN - 2;
		size_t remainingSize = maxLen;
		size_t writtenSize = 0;

		// 함수 이름과 라인 번호를 로그 문자열에 기록
		auto result = std::format_to_n(mStr + writtenSize, remainingSize,
			L"[{}{:04}]{:02}시{:02}분{:02}초\t",
			color,
			tThreadId,
			localTm.tm_hour,
			localTm.tm_min,
			localTm.tm_sec
		);

		writtenSize += result.size;
		remainingSize -= result.size;

		// 콘솔 출력용 Offset을 기록
		mOffsetForConsoleStr = writtenSize;

		// 로그 메시지를 포맷팅하고 문자열에 기록
		result = std::format_to_n(mStr + writtenSize, remainingSize, format, std::forward<_Args>(args)...);

		writtenSize += result.size;
		remainingSize -= result.size;

		result = std::format_to_n(mStr + writtenSize, remainingSize,
			L"\t--- {}({})",
			function,
			line
		);

		writtenSize += result.size;
		remainingSize -= result.size;

		// 로그 문자열의 끝에 개행 및 널 종료 문자를 추가
		if (writtenSize < Log::MAX_LOG_STR_LEN - 2)
		{
			mStr[writtenSize] = L'\n';
			mStr[writtenSize + 1] = L'\0';

			// 최종 길이
			mStrLen = writtenSize + 1;
		}
		else
		{
			mStr[Log::MAX_LOG_STR_LEN - 2] = L'\n';
			mStr[Log::MAX_LOG_STR_LEN - 1] = L'\0';

			// 최종 길이
			mStrLen = Log::MAX_LOG_STR_LEN;
		}
	}

	template<typename... _Args>
	void Write(const std::wformat_string<_Args...>& format, _Args&&... args)
	{
		tm localTm{};
		_localtime64_s(&localTm, &tClock.GetGlobalNowTt());
		
		constexpr size_t maxLen = Log::MAX_LOG_STR_LEN - 2;
		size_t remainingSize = maxLen;
		size_t writtenSize = 0;

		// 함수 이름과 라인 번호를 로그 문자열에 기록
		auto result = std::format_to_n(mStr + writtenSize, remainingSize,
			L"[{:04}]{:02}시{:02}분{:02}초\t",
			tThreadId,
			localTm.tm_hour,
			localTm.tm_min,
			localTm.tm_sec
		);

		writtenSize += result.size;
		remainingSize -= result.size;

		// 로그 메시지를 포맷팅하고 문자열에 기록
		result = std::format_to_n(mStr + writtenSize, remainingSize, format, std::forward<_Args>(args)...);

		writtenSize += result.size;
		remainingSize -= result.size;

		// 로그 문자열의 끝에 개행 및 널 종료 문자를 추가
		if (writtenSize < Log::MAX_LOG_STR_LEN - 2)
		{
			mStr[writtenSize] = L'\n';
			mStr[writtenSize + 1] = L'\0';

			// 최종 길이
			mStrLen = writtenSize + 1;
		}
		else
		{
			mStr[Log::MAX_LOG_STR_LEN - 2] = L'\n';
			mStr[Log::MAX_LOG_STR_LEN - 1] = L'\0';

			// 최종 길이
			mStrLen = Log::MAX_LOG_STR_LEN;
		}
	}
};

