// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 분산 환경용 고성능 고유 식별자 클래스 (Twitter Snowflake 변형)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 분산 환경을 위한 고성능 고유 식별자 (64비트).
// 
// [WHY] Twitter Snowflake 핵심 개념 차용 - "ID 발급자 분산"
// - 핵심 아이디어: 중앙 ID 서버 없이 각 발급자가 독립적으로 전역 고유 ID 생성
// - 본 구현: 발급자ID = AppId(서버) + ThreadId(스레드)로 세분화
// - 시간(38비트) + AppId(8비트) + ThreadId(8비트) + 순차(9비트) + 양수플래그(1비트) = 64비트
// - 서버/스레드 단위로 완전 독립적 ID 생성 (Lock 불필요)
// 
// [WHY] 서버 가상화 설계
// - 모든 ID가 전역적으로 고유하므로 서버 구분이 ID 내부에 내장됨
// - 서버 통합/이전 시 ID 재매핑 불필요 (ID 자체가 출처 정보 포함)
// - DB 샤딩에서도 ID만으로 원본 서버 추적 가능
// - 서버 스케일 아웃/인 시 별도 마이그레이션 로직 불필요
// 
// 비트 구성:
// - 시간(38비트): 약 8.7년간 유일성 보장 (밀리초 단위)
// - AppId(8비트): 256개 서버 인스턴스 구분
// - ThreadId(8비트): 256개 스레드 구분
// - 순차(9비트): 동일 밀리초 내 512개 ID 생성 가능
// - 양수플래그(1비트): DB 호환성 (signed bigint 양수 보장)
// 
// - 책임: 전역 고유 ID 생성 및 관리
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
#pragma pack(push, 1)
class Uuid
{
private:
	union UuidUnion
	{
		int64_t mValue;  // 전체 값 (64비트)

		struct
		{
			uint8_t mPositiveFlag : 1;   // 양수 플래그 (1비트)
			uint64_t mTime : 38;         // 시간 (38비트)
			uint8_t mAppId;              // 앱 ID (8비트)
			uint8_t mThreadId;           // 스레드 ID (8비트)
			uint16_t mSequence : 9;      // 순차 번호 (9비트)
		} mFields;

		constexpr UuidUnion()
			:
			mValue(0LL)
		{
		}

		constexpr UuidUnion(int64_t value)
			:
			mValue(value)
		{
		}

		constexpr explicit UuidUnion(ClockMs timeMs, uint8_t appId, uint8_t threadId, uint16_t sequence)
			:
			mValue(0LL)
		{			
			mFields.mPositiveFlag = 0;
			// [WHY] 38비트 마스크로 시간 값 제한
			mFields.mTime = (timeMs.count() & ((1ULL << 38) - 1));
			mFields.mAppId = appId;
			mFields.mThreadId = threadId;
			// [WHY] 9비트 마스크로 순차 번호 제한
			mFields.mSequence = (sequence & ((1 << 9) - 1));
		}
	} mData;

public:
	constexpr Uuid()
		:
		mData(0ms, 0, 0, 0)
	{
	}

	constexpr Uuid(int64_t value)
		:
		mData(value)
	{
	}

	constexpr explicit Uuid(ClockMs timeMs, uint8_t appId, uint8_t threadId, uint16_t sequence)
		:
		mData(timeMs, appId, threadId, sequence)
	{
	}

public:
	//Uuid& operator=(const Uuid& other)
	//{
	//	mData.mValue = other.mData.mValue;
	//	return *this;
	//}

	// 두 UUID가 같은지 비교한다.
	// - other: 비교할 UUID
	// - return: 같으면 true
	bool operator==(const Uuid& other) const
	{
		return mData.mValue == other.mData.mValue;
	}

	// 두 UUID의 크기를 비교한다.
	// - other: 비교할 UUID
	// - return: 작으면 true
	bool operator<(const Uuid& other) const
	{
		return mData.mValue < other.mData.mValue;
	}

	// 내부 데이터를 반환한다.
	// - return: 64비트 값 참조
	const int64_t& GetData() const { return mData.mValue; }

	// int64_t로 암시적 변환한다.
	operator const int64_t& () const { return mData.mValue; }

	friend struct std::hash<Uuid>;
};
#pragma pack(pop)

namespace std
{
	template <>
	struct hash<Uuid>
	{
		std::size_t operator()(const Uuid& id) const
		{
			return id.GetData();
		}
	};
}


template<typename _CharType>
struct std::formatter<Uuid, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(const Uuid& t, auto& ctx) const	{
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		result += std::format(L"\n{}uuid: {}", tTabStr, t.GetData());
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
