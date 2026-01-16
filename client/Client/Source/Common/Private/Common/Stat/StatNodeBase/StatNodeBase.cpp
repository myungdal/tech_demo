// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Stat/StatNodeBase/StatNodeBase.h"

#include "Common/Stat/statContainer/StatContainer.h"


StatNodeBase::StatNodeBase(StatType statType, std::initializer_list<StatType>&& dependencySet)
	:
	mStatType(statType),
	mDependencySet(std::move(dependencySet))
{
}
StatValue StatNodeBase::GetStatValue(StatContainer& statContainer, StatType statType) const
{
	// [WHY] 자기 자신의 스탯 타입이면 순수 값 반환
	if (mStatType == statType)
		return statContainer.GetPureStatValue(statType);

#if defined(UE_EDITOR) || defined(_DEBUG)
	// [WHY] 디버그 모드에서 의존성 검증
	if (!mDependencySet.count(statType))
		_ASSERT_CRASH(false);
#endif

	return statContainer.GetStatValue(statType);
}
