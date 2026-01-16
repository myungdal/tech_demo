// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 함수 호출자 기반 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 함수 호출자 인터페이스.
// - 책임: 가변 인자 함수 호출 인터페이스 정의
template <typename... _Args>
class InvokerBase
{
public:
	virtual ~InvokerBase() = default;

public:
	// 함수를 호출한다.
	// - args: 전달할 인자들
	virtual void Invoke(_Args&&... args) = 0;
};

// 함수 호출자 구현 클래스.
// - 책임: 멤버 함수 포인터를 통한 실제 호출 구현
template <typename _Owner, typename _Function, typename... _Args>
class InvokerImpl final : public InvokerBase<_Args...>
{
public:
	using Function = std::decay_t<_Function>;

private:
	_Owner* mOwner = nullptr;        // 함수를 소유하는 객체 포인터
	Function mFunction = nullptr;     // 호출할 멤버 함수 포인터

public:
	DISABLE_COPY(InvokerImpl);
	explicit InvokerImpl(_Owner* owner, _Function&& function)
		:
		mOwner(owner),
		mFunction(std::forward<_Function>(function))
	{
	}

	void Invoke(_Args&&... args) override
	{
		(mOwner->*mFunction)(std::forward<_Args>(args)...);
	}
};

