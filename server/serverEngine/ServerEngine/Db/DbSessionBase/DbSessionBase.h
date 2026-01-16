// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// DbSessionBase.h - DB 세션 기본 클래스
#pragma once

#include "ServerEngine/Db/DbSessionBase/CommitType.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DbSessionBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DbConnection;
class DbConnectionPool;

// DB 세션 기본 클래스 (트랜잭션 관리)
class DbSessionBase
{
protected:
	// 현재 사용 중인 DB 커넥션
	DbConnection* mConn = nullptr;
	
	// 커밋 타입 (AUTO/MANUAL)
	CommitType mCommitType = CommitType::MANUAL;
	
	// 해제 여부 (풀링용)
	bool mIsFree = true;

public:
	// 생성자
	explicit DbSessionBase(CommitType commitType = CommitType::MANUAL);
	
	// 가상 소멸자
	virtual ~DbSessionBase();

	// SQL 실행
	[[nodiscard]] bool Execute(const wchar_t* sql);
	
	// 트랜잭션 커밋
	[[nodiscard]] bool Commit();
	
	// 트랜잭션 롤백
	bool Rollback();
	
	// 세션 해제 (커넥션 반환)
	void Free();

	// 영향받은 행 개수 조회
	int64_t GetRowCount() const;

	// 세션 준비 상태 확인
	bool IsReady() const noexcept { return (nullptr == mConn) ? false : true; }

protected:
	// 커넥션 획득 (파생 클래스에서 구현)
	void Acquire();
	
	// 커넥션 반환 (파생 클래스에서 구현)
	void Release();

	// 커넥션 획득 훅 (파생 클래스에서 오버라이드)
	virtual DbConnection* OnAcquire() { return nullptr; };
	
	// 커넥션 반환 훅 (파생 클래스에서 오버라이드)
	virtual void OnRelease(MAYBE_UNUSED DbConnection* conn) {};

private:
	friend class DbSp;
	
	// DB 커넥션 조회 (DbSp에서만 접근)
	DbConnection* GetDbConn() const noexcept { return mConn; }
};
