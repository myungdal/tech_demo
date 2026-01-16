// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UserCacheTableBase
// 사용자 캐시 테이블의 기본 클래스입니다.
// 모든 사용자 테이블(UserItem, UserCharacter 등)은 이 클래스를 상속받습니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PACKET_API UserCacheTableBase
{
public:
	virtual ~UserCacheTableBase() = default;
};
