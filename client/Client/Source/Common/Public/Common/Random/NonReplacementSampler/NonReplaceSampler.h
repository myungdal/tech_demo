// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 비복원 랜덤 샘플러 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 비복원 랜덤 추출기 (한 번 선택된 아이템은 제거됨).
// - 책임: 비율 기반 비복원 랜덤 선택
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
template<typename _ItemType>
class NonReplaceSampler final
{
private:
	size_t mMinTotalRatio = 0;  // 이 값 이하로 떨어지면 랜덤 테이블을 다시 생성

private:
	using ItemRatioPair = std::pair<_ItemType, size_t>;  // 아이템-비율 쌍
	using ItemRatioList = std::vector<ItemRatioPair>;     // 아이템-비율 리스트

	ItemRatioList mItemRatioListSrc;  // 원본 아이템-비율 리스트

private:
	ItemRatioList mItemRatioList;     // 현재 아이템-비율 리스트 (선택 시 감소)
	size_t mTotalRatio = 0;            // 현재 전체 비율 합계
	
public:
	explicit NonReplaceSampler(size_t minTotalRatio)
		:
		mMinTotalRatio(minTotalRatio)
	{
	}

	// 샘플러를 초기화한다 (원본 리스트로 복원).
	void Clear()
	{
		mItemRatioList = mItemRatioListSrc;
		
		mTotalRatio = 0;
		for (const auto& [type, size] : mItemRatioListSrc)
		{
			mTotalRatio += size;
		}
	}

	// 아이템과 비율을 추가한다.
	// - item: 추가할 아이템
	// - ratio: 비율 (가중치)
	void Add(const _ItemType& item, size_t ratio)
	{
		mItemRatioListSrc.push_back(std::make_pair(item, ratio));
	}

	// 샘플러를 직렬화한다.
	// - return: 직렬화된 문자열
	std::wstring Serialize() const
	{
		std::wstringstream ss;

		ss << mMinTotalRatio << L'\n';

		for (const auto& [type, size] : mItemRatioListSrc)
		{
			ss << type << L'\n'; 
			ss << size << L'\n';
		}
		
		for (const auto& [type, size] : mItemRatioList)
		{
			ss << type << L'\n';
			ss << size << L'\n';
		}

		std::wstring result = ss.str();

		// [WHY] 마지막 개행 제거
		if (false == result.empty())
			result.pop_back();

		return result;
	}

	// 샘플러를 역직렬화한다.
	// - samples: 직렬화된 문자열
	void Deserialize(const std::wstring& samples)
	{
		if (false == TryDeserialize(samples))
		{
			Clear();
		}
	}

private:
	bool TryDeserialize(const std::wstring& samples)
	{
		std::wstringstream ss(samples);

		if (ss.eof())
			return false;

		size_t minTotalRatio;
		ss >> minTotalRatio;
		if (mMinTotalRatio != minTotalRatio)
			return false;

		if (ss.eof())
			return false;

		for (auto& it : mItemRatioListSrc)
		{
			if (ss.eof())
				return false;

			ItemRatioPair item;
			ss >> item.first >> item.second;
			if ((it.first != item.first) || (it.second != item.second))
				return false;
		}

		mItemRatioList = mItemRatioListSrc;
		mTotalRatio = 0;

		for (auto& it : mItemRatioList)
		{
			if (ss.eof())
				return false;

			ItemRatioPair item;
			ss >> item.first >> item.second;
			if (it.first != item.first)
				return false;

			it.second = item.second;
			mTotalRatio += it.second;
		}

		if (false == ss.eof())
			return false;

		return true;
	}

public:
	// 비율에 따라 랜덤으로 아이템을 선택한다 (비복원).
	// - item: 출력 파라미터 (선택된 아이템)
	// - _Func: 랜덤 생성기 함수 (std::mt19937, DeterministicRandom32, Mt19937Random 등)
	// - return: 성공 시 true
	template<class _Urng>
	bool Pick(OUT _ItemType& item, _Urng&& _Func)
	{        
		// [WHY] 비율이 최소값 이하로 떨어지면 테이블 재생성
		if (mTotalRatio <= mMinTotalRatio)
			Clear();

		if (mTotalRatio == 0)
			return false;

		// [WHY] 1을 더하여 1 ~ mTotalRatio 범위로 만듦
		const size_t randValue = 1ULL + _Func(mTotalRatio);

		size_t sum = 0;
		for (size_t i = 0; i < mItemRatioList.size(); ++i)
		{
			ItemRatioPair& itemRatioPair = mItemRatioList[i];

			sum += itemRatioPair.second;

			// [WHY] 누적 합이 랜덤 값보다 크거나 같으면 해당 아이템 선택
			if (randValue > sum)
				continue;

			item = itemRatioPair.first;

			// [WHY] 선택된 아이템의 비율 감소 (비복원)
			if (itemRatioPair.second > 0)
			{
				--itemRatioPair.second;
				--mTotalRatio;
			}

			return true;
		}

		return false;
	}
};

