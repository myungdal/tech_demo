// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class StatNodeBuilder;
class Serializer;
class Deserializer;

// 스탯 컨테이너 클래스.
// - 책임: 스탯 값 저장 및 의존성 기반 업데이트 관리
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class COMMON_API StatContainer
{
private:
	StatArray mPureStatValues;      // 순수 스탯 값 (의존성 계산 전)
	StatArray mStatValues;          // 최종 스탯 값 (의존성 계산 후)

private:
	using NeedsUpdateArray = std::array<bool, static_cast<size_t>(StatType::MAX)>;
	NeedsUpdateArray mNeedsUpdateArray;  // 업데이트 필요 여부 플래그 배열

public:
	StatContainer();
	
	// 스탯을 직렬화한다.
	// - serializer: 직렬화기 참조
	void Serialize(Serializer& serializer);

	// 스탯을 역직렬화한다.
	// - deserializer: 역직렬화기 참조
	void Deserialize(Deserializer& deserializer);

	// 순수 스탯 값으로부터 병합한다.
	// - pureStatValues: 병합할 순수 스탯 값 배열
	void MergeFromStatValues(const StatArray& pureStatValues);

	// 다른 StatContainer로부터 복사한다.
	// - other: 복사할 StatContainer 참조
	void CopyFromOther(const StatContainer& other);

private:
	// 스탯 타입의 업데이트 필요 플래그를 설정한다.
	// - statType: 스탯 타입
	void SetNeedUpdate(StatType statType);

public:
	// 스탯 값을 증가시킨다.
	// - statType: 스탯 타입
	// - statValue: 증가할 값
	void IncreaseStatValue(StatType statType, StatValue statValue);

	// 스탯 값을 감소시킨다.
	// - statType: 스탯 타입
	// - statValue: 감소할 값
	void DecreaseStatValue(StatType statType, StatValue statValue);

	// 순수 스탯 값을 반환한다 (의존성 계산 전).
	// - statType: 스탯 타입
	// - return: 순수 스탯 값
	StatValue GetPureStatValue(StatType statType) const;

	// 최종 스탯 값을 반환한다 (의존성 계산 후, 필요 시 업데이트).
	// - statType: 스탯 타입
	// - return: 최종 스탯 값
	StatValue GetStatValue(StatType statType);
};


template<typename _CharType>
struct std::formatter<StatContainer, _CharType> {
	constexpr auto parse(auto& ctx) { return ctx.end(); }
	auto format(MAYBE_UNUSED StatContainer& t, auto& ctx) const {
		std::wstring result = L"";
		tTabStr += std::wstring(L"\t");
		
		// 모든 StatType을 순회하며 출력
		for (int i = 0; i < static_cast<int>(StatType::MAX); ++i)
		{
			StatType statType = static_cast<StatType>(i);
			StatValue statValue = t.GetStatValue(statType);
			
			// 값이 0이 아닌 스탯만 출력
			const wchar_t* statName = GetStatTypeString(statType);
			result += std::format(L"\n{}{}: {}", tTabStr, statName, statValue);
		}
		
		tTabStr.pop_back();
		return std::ranges::copy(result, ctx.out()).out;
	}
};
