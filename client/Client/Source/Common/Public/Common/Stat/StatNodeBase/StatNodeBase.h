// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class StatContainer;

// 스탯 노드 기반 클래스.
// - 책임: 스탯 의존성 관리 및 값 업데이트
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class COMMON_API StatNodeBase
{
private:
	StatType mStatType = StatType::MAX;                    // 이 노드의 스탯 타입
	std::set<StatType> mDependencySet;                     // 의존하는 스탯 타입 집합
	std::set<StatType> mDependentSet;                      // 이 스탯에 의존하는 스탯 타입 집합

public:
	explicit StatNodeBase(StatType statType, std::initializer_list<StatType>&& dependencySet);
	virtual ~StatNodeBase() = default;

public:
	// 의존하는 스탯 타입을 추가한다.
	// - statType: 의존할 스탯 타입
	void InsertDependency(StatType statType) { mDependencySet.insert(statType); }

	// 이 스탯에 의존하는 스탯 타입을 추가한다.
	// - statType: 의존하는 스탯 타입
	void InsertDependent(StatType statType) { mDependentSet.insert(statType); }

public:
	// 스탯 값을 업데이트한다 (파생 클래스에서 구현).
	// - statContainer: StatContainer 참조
	// - return: 업데이트된 스탯 값
	virtual StatValue UpdateStatValue(StatContainer& statContainer) const = 0;

public:
	// 의존하는 스탯 타입 집합을 반환한다.
	// - return: 의존성 집합
	const std::set<StatType>& GetDependencySet() const { return mDependencySet; }

	// 이 스탯에 의존하는 스탯 타입 집합을 반환한다.
	// - return: 의존성 집합
	const std::set<StatType>& GetDependentSet() const { return mDependentSet; }	

	// 스탯 타입을 반환한다.
	// - return: 스탯 타입
	StatType GetStatType() const { return mStatType; }

	// StatContainer에서 스탯 값을 가져온다.
	// - statContainer: StatContainer 참조
	// - statType: 스탯 타입
	// - return: 스탯 값
	StatValue GetStatValue(StatContainer& statContainer, StatType statType) const;
};
