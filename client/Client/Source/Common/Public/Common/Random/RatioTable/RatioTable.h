// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 비율 기반 랜덤 선택 테이블 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 지분률에 따라 아이템을 랜덤 선택하는 테이블.
// - 책임: 비율 기반 가중치 랜덤 선택
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
template<typename _ItemType>
class RatioTable final
{
private:
	using ItemRatioPair = std::pair<_ItemType, size_t>;  // 아이템-비율 쌍
	using ItemRatioList = std::vector<ItemRatioPair>;     // 아이템-비율 리스트

	ItemRatioList mItemRatioList;  // 아이템-비율 리스트
	size_t mTotalRatio = 0;         // 전체 비율 합계

public:
	// 테이블을 초기화한다.
	void Clear()
	{
		mItemRatioList.clear();
		mTotalRatio = 0;
	}

	// 아이템과 비율을 추가한다.
	// - item: 추가할 아이템
	// - ratio: 비율 (가중치)
	void Add(const _ItemType& item, size_t ratio)
	{
		mItemRatioList.push_back(std::make_pair(item, ratio));
		mTotalRatio += ratio;
	}
	
	// 인덱스로 아이템을 가져온다.
	// - idx: 인덱스
	// - item: 출력 파라미터 (아이템)
	// - return: 성공 시 true
	bool At(size_t idx, OUT _ItemType& item)
	{
		if (idx >= mItemRatioList.size())
			return false;

		const ItemRatioPair& itemRatioPair = mItemRatioList.at(idx);
		item = itemRatioPair.first;
		return true;
	}

	// 비율에 따라 랜덤으로 아이템을 선택한다.
	// - item: 출력 파라미터 (선택된 아이템)
	// - _Func: 랜덤 생성기 함수 (std::mt19937, DeterministicRandom32, Mt19937Random 등)
	// - return: 성공 시 true
	template<class _Urng>
	bool Pick(OUT _ItemType& item, _Urng&& _Func) const
	{
		// [WHY] 1을 더하여 1 ~ mTotalRatio 범위로 만듦
		const size_t randValue = (1 + _Func(mTotalRatio));

		size_t sum = 0;
		for (size_t i = 0; i < mItemRatioList.size(); ++i)
		{
			const ItemRatioPair& itemRatioPair = mItemRatioList[i];

			sum += itemRatioPair.second;

			// [WHY] 누적 합이 랜덤 값보다 크거나 같으면 해당 아이템 선택
			if (randValue > sum)
				continue;

			item = itemRatioPair.first;
			return true;
		}

		return false;
	}
};
