// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Key/StringKey/StringKeyPool.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 문자열 키 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 문자열을 해시 포인터로 변환하여 빠른 비교를 제공하는 클래스.
// - 책임: 문자열 해시 관리 및 빠른 비교 연산
// - 스레드 모델: 단일 스레드 (StringKeyPool과 함께 사용)
class StringKey
{
private:
	const wchar_t* mHash = nullptr;  // 해시된 문자열 포인터 (StringKeyPool에서 관리)

public:
	StringKey() = default;

	// 문자열로부터 StringKey를 생성한다.
	// - key: 원본 문자열 (UTF-16)
	// - pool: StringKeyPool 참조
	explicit StringKey(const wchar_t* key, StringKeyPool& pool)
		:
		mHash(pool.GetHash(key))
	{
	}

	StringKey(const StringKey& o)
		:
		mHash(o.mHash)
	{
	}

	explicit StringKey(StringKey&& o) noexcept
		:
		mHash(o.mHash)
	{
	}

public:
	StringKey& operator=(const StringKey& o)
	{
		mHash = o.mHash;
		return *this;
	}

	StringKey& operator=(StringKey&& o) noexcept
	{
		mHash = o.mHash;
		return *this;
	}

public:
	// 저장된 문자열 포인터를 반환한다.
	// - return: 문자열 포인터
	const wchar_t* GetString() const
	{
		return mHash;
	}

	// 문자열 길이를 반환한다.
	// - return: 길이 (문자 수)
	uint8_t GetLength() const
	{
		uint8_t length = 0;
		
		if (const uint16_t* ptr = reinterpret_cast<const uint16_t*>(mHash))
		{
			while (*ptr)
			{
				++ptr;
				++length;
			}
		}
		
		return length;
	}

public:
	bool operator==(const StringKey& o) const
	{
		return (mHash == o.mHash);
	}

	bool operator!=(const StringKey& o) const
	{
		return (mHash != o.mHash);
	}

	bool operator<(const StringKey& o) const
	{
		return (mHash < o.mHash);
	}

	bool operator>(const StringKey& o) const
	{
		return (mHash > o.mHash);
	}
};


namespace std
{
	template <>
	struct hash<StringKey>
	{
		std::size_t operator()(const StringKey& k) const noexcept
		{
			return *reinterpret_cast<const std::size_t*>(k.GetString());
		}
	};
}
