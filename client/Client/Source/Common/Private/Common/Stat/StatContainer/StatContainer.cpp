// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Stat/StatContainer/StatContainer.h"

#include "Common/Stat/StatNodeBase/StatNodeBase.h"
#include "Common/Stat/StatNodeBuilder.h"
#include "Common/Serializer/Serializer.h"
#include "Common/Serializer/Deserializer.h"


StatContainer::StatContainer()
{
	mPureStatValues.fill(0);
	mStatValues.fill(0);
	// [WHY] 초기화 시 모든 스탯이 업데이트 필요 상태
	mNeedsUpdateArray.fill(true);
}

void StatContainer::Serialize(Serializer& serializer)
{
	// [WHY] 순수 스탯 값들만 저장 (의존성 계산 전 값)
	serializer.WriteBinary((const uint8_t*)mPureStatValues.data(), sizeof(StatArray));
}

void StatContainer::Deserialize(Deserializer& deserializer)
{
	deserializer.ReadBinary((uint8_t*)mPureStatValues.data(), sizeof(StatArray));

	// [WHY] 순수 스탯 값만 가져왔기 때문에 모두 업데이트가 필요
	mNeedsUpdateArray.fill(true);
}

void StatContainer::MergeFromStatValues(const StatArray& pureStatValues)
{
	for (size_t i = 0; i < mPureStatValues.size(); ++i)
	{
		// [WHY] 0이 아닌 값만 병합
		if (!pureStatValues[i])
			continue;

		mPureStatValues[i] += pureStatValues[i];
		mNeedsUpdateArray[i] = true;
	}
}

void StatContainer::CopyFromOther(const StatContainer& other)
{
	mPureStatValues = other.mPureStatValues;
	mStatValues = other.mStatValues;
	mNeedsUpdateArray = other.mNeedsUpdateArray;
}

void StatContainer::SetNeedUpdate(StatType statType)
{
	// [WHY] BFS로 의존성 그래프를 순회하여 모든 의존 스탯을 업데이트 필요 상태로 설정
	std::queue<StatType> queue;
	queue.push(statType);

	while (!queue.empty())
	{
		StatType current = queue.front();
		queue.pop();

		const auto& dependentSet = gStatNodeBuilder.GetStatNode(current).GetDependentSet();
		for (StatType dependent : dependentSet) 
		{
			bool& needsUpdate = mNeedsUpdateArray.at(static_cast<size_t>(dependent));

			// [WHY] 이미 업데이트 필요 상태면 스킵 (순환 참조 방지)
			if (!needsUpdate)
			{
				needsUpdate = true;
				queue.push(dependent);
			}
		}
	}
}

void StatContainer::IncreaseStatValue(StatType statType, StatValue statValue)
{
	mPureStatValues.at(static_cast<size_t>(statType)) += statValue;
	SetNeedUpdate(statType);
}

void StatContainer::DecreaseStatValue(StatType statType, StatValue statValue)
{
	mPureStatValues.at(static_cast<size_t>(statType)) -= statValue;
	SetNeedUpdate(statType);
}

StatValue StatContainer::GetPureStatValue(StatType statType) const
{
	return mPureStatValues.at(static_cast<size_t>(statType));
}

StatValue StatContainer::GetStatValue(StatType statType)
{
	// [WHY] 업데이트가 필요하면 의존성 계산 후 값 업데이트
	if (mNeedsUpdateArray.at(static_cast<size_t>(statType)))
	{
		mStatValues.at(static_cast<size_t>(statType)) = gStatNodeBuilder.GetStatNode(statType).UpdateStatValue(*this);
		mNeedsUpdateArray.at(static_cast<size_t>(statType)) = false;
	}
	return mStatValues.at(static_cast<size_t>(statType));
}
