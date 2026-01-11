// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// PacketTransactorBase.h - 패킷 트랜잭션 기본 클래스
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 패킷 트랜잭션 처리
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 패킷 트랜잭션 기본 클래스 (트랜잭션 패턴 구현)
class PacketTransactorBase
{
private:
	// 실행 결과
	Result mResult = Result::SUCCEEDED;

public:
	PacketTransactorBase() = default;
	virtual ~PacketTransactorBase() = default;

	// 트랜잭션 실행
	void Run();

	// 에러 발생 여부 확인
	bool HasError() const { return (Result::SUCCEEDED != mResult); }
	
	// 실행 결과 조회
	const Result GetResult() const { return mResult; }

protected:
	// 사전 검증 (파생 클래스에서 오버라이드)
	virtual Result OnPreValidate() { return Result::SUCCEEDED; }
	
	// 사전 갱신 (파생 클래스에서 오버라이드)
	virtual void OnPreUpdate() {}

	// 메인 갱신 (파생 클래스에서 오버라이드)
	virtual Result OnUpdate() { return Result::SUCCEEDED; }

	// 커밋 (파생 클래스에서 오버라이드)
	virtual bool OnCommit() { return true; }
	
	// 롤백 (파생 클래스에서 오버라이드)
	virtual bool OnRollback() { return true; }

	// 완료 처리 (파생 클래스에서 오버라이드)
	virtual void OnFinish() {}
	
	// 에러 처리 (파생 클래스에서 오버라이드)
	virtual void OnError() {}

	// 로그 출력 (파생 클래스에서 오버라이드)
	virtual void OnLog() {}
};
