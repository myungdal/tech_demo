// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스탯 노드 빌더 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 스탯 노드들을 생성하고 의존성을 관리하는 클래스.
// - 책임: 스탯 노드 생성 및 의존성 그래프 구축
// - 스레드 모델: 단일 스레드 (전역 싱글톤)
class COMMON_API StatNodeBuilder
{
private:
	StatNodeArray mNodes;  // 모든 스탯 노드 배열

public:
	StatNodeBuilder();

private:
	// 의존성 그래프를 구축한다.
	void BuildDependent();

	// 의존성을 추가한다.
	// - from: 의존하는 스탯 타입
	// - to: 의존받는 스탯 타입
	void InsertDependentTo(StatType from, StatType to);

public:
	// 스탯 타입에 해당하는 노드를 반환한다.
	// - statType: 스탯 타입
	// - return: StatNodeBase 참조
	StatNodeBase& GetStatNode(StatType statType);
};

inline StatNodeBuilder gStatNodeBuilder;  // 전역 스탯 노드 빌더
