// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 컴포넌트 베이스 - ECS 패턴의 Component 기반 클래스
// - 엔티티의 데이터를 저장하는 구성 요소
// - 템플릿 기반으로 타입 안전성 보장
// [WHY] static 멤버로 ComponentType 관리하여 런타임 타입 체크 최소화
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 컴포넌트 베이스 클래스 - 모든 컴포넌트의 최상위
struct ComponentBase
{
};

// 컴포넌트 템플릿 - 실제 컴포넌트 구현의 기반
// _Component: 실제 컴포넌트 타입
// _ComponentGroup: 컴포넌트 그룹 (그룹핑 없으면 자기 자신)
template<typename _Component, typename _ComponentGroup = _Component>
struct MMOSYNC_API Component : ComponentBase
{
private:
	friend class ComponentRegisterer;
	friend class Engine;

	using ComponentGroup = _ComponentGroup;

private:
	static ComponentType mComponentType;           // 컴포넌트 고유 타입 ID
	static ComponentTypeValue mComponentTypeValue; // 비트셋으로 표현한 타입 값 (빠른 검사)

private:
	// 컴포넌트 타입 초기화 (Engine 시작 시 호출)
	static void SetupComponent(ComponentType componentType)
	{
		mComponentType = componentType;
		mComponentTypeValue.set(_IDX(componentType), true);
	}

	// 컴포넌트 그룹 타입 추가 등록
	static void SetupComponentGroup(ComponentType componentType)
	{
		mComponentTypeValue.set(_IDX(componentType), true);
	}

public:
	// 컴포넌트 타입 ID 반환
	static const ComponentType& GetComponentType() { return mComponentType; }

	// 컴포넌트 타입 비트 값 반환 (빠른 Has 체크용)
	static const ComponentTypeValue& GetComponentTypeValue() { return mComponentTypeValue;	}
};
