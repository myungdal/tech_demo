// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// CommitType.h - DB 커밋 타입
// DbSession의 트랜잭션 커밋 정책을 지정한다.

#pragma once

enum class CommitType : uint8_t
{
	MANUAL = 0,
	AUTO
};

