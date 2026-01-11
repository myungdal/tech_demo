// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GUID 클래스 (36글자, 37바이트)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// UUID v4 형식의 GUID를 저장하는 클래스.
// - 책임: GUID 생성 및 관리
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
#pragma pack(push, 1)
class Guid
{
private:
	std::array<wchar_t, 37> mData;  // GUID 문자열 (36글자 + 널 종료)

public:
	Guid()
	{
		mData.fill(L'\0');
	}

	Guid(const Guid& other)
	{
		std::copy(other.mData.begin(), other.mData.end(), mData.begin());
	}

	Guid(const std::wstring& str)
	{
		std::copy(str.begin(), str.end(), mData.begin());
	}

	constexpr Guid(const wchar_t* wstr)
	{
		if (nullptr == wstr)
		{
			mData.fill(L'\0');
		}
		else
		{
			for (size_t i = 0; i < 36; ++i)
			{
				mData[i] = wstr[i];
			}

			mData[36] = L'\0';
		}
	}

public:
	// UUID v4 형식의 GUID를 생성한다.
	// - return: 자기 자신 참조
	Guid& Generate()
	{
		static const wchar_t* hex_digits = L"0123456789ABCDEF";
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<size_t> dis(0, 15);

		// [WHY] 람다로 헥스 문자 설정 로직 재사용
		auto set_hex = [&](size_t pos, size_t count)
			{
				for (size_t i = 0; i < count; ++i)
				{
					mData[pos + i] = hex_digits[dis(gen)];
				}
			};

		// [WHY] UUID v4 형식: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
		set_hex(0, 8);
		mData[8] = L'-';
		set_hex(9, 4);
		mData[13] = L'-';
		mData[14] = L'4';  // 버전 4
		set_hex(15, 3);
		mData[18] = L'-';
		// [WHY] y는 8, 9, A, B 중 하나 (0x80 ~ 0xBF)
		mData[19] = (hex_digits[dis(gen)] & 0x3F) | 0x80;
		set_hex(20, 3);
		mData[23] = L'-';
		set_hex(24, 12);

		mData[36] = L'\0';

		return *this;
	}

	// GUID를 wchar_t 배열로 복사한다.
	// - wstr: 출력 버퍼 (최소 37바이트)
	void CopyToWstr(OUT wchar_t* wstr) const
	{
		for (size_t i = 0; i < 36; ++i)
		{
			wstr[i] = mData[i];
		}

		wstr[36] = L'\0';
	}

	Guid& operator=(const Guid& other)
	{
		std::copy(other.mData.begin(), other.mData.end(), mData.begin());
		return *this;
	}

	Guid& operator=(const std::wstring& str)
	{
		std::copy(str.begin(), str.end(), mData.begin());
		return *this;
	}

	Guid& operator=(const wchar_t* wstr)
	{
		if (nullptr == wstr)
		{
			mData.fill(L'\0');
		}
		else
		{
			for (size_t i = 0; i < 36; ++i)
			{
				mData[i] = wstr[i];
			}

			mData[36] = L'\0';
		}

		return *this;
	}

	// 두 GUID가 같은지 비교한다.
	// - other: 비교할 GUID
	// - return: 같으면 true
	bool operator==(const Guid& other) const
	{
		return mData == other.mData;
	}

	// 두 GUID의 크기를 비교한다.
	// - other: 비교할 GUID
	// - return: 작으면 true
	bool operator<(const Guid& other) const
	{
		return std::lexicographical_compare(
			mData.begin(), mData.end(),
			other.mData.begin(), other.mData.end()
		);
	}

	// GUID를 문자열로 변환한다.
	// - return: GUID 문자열
	std::wstring ToString() const
	{
		return std::wstring(mData.data());
	}

	// 내부 데이터 포인터를 반환한다.
	// - return: GUID 문자열 포인터
	const wchar_t* GetData() const { return mData.data(); }

	// wchar_t*로 암시적 변환한다.
	operator const wchar_t* () const { return mData.data(); }

	friend struct std::hash<Guid>;
};
#pragma pack(pop)


namespace std 
{
	template <>
	struct hash<Guid>
	{
		std::size_t operator()(const Guid& id) const
		{
			size_t hash = 2166136261u;
			for (wchar_t c : id.mData) 
			{
				hash ^= static_cast<size_t>(c);
				hash *= 16777619;
			}
			return hash;
		}
	};
}


template<typename _CharType>
struct std::formatter<Guid, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const Guid& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}guid: {}", tTabStr, t.GetData());
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
