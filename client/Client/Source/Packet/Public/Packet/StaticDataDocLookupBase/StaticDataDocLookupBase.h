// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StaticDataDocLookupBase
// 정적 데이터 뷰의 기본 클래스입니다.
// 모든 정적 데이터 뷰는 이 클래스를 상속받습니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StaticDataDocLookupBase
{
public:
	virtual ~StaticDataDocLookupBase() = default;

	// 뷰 데이터 초기화
	virtual void Clear() {}

	// 정적 데이터 로드
	virtual void Load() {}

	// 다른 데이터와 연결 (참조 관계 설정)
	virtual void Link() {}
};
