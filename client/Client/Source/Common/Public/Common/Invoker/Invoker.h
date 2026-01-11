// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Invoker/Base/InvokerBase.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 함수 호출자 템플릿 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 가변 인자 함수들을 등록하고 일괄 호출하는 클래스.
// - 책임: 콜백 함수 등록 및 호출 관리
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
template <class... _Args>
class Invoker
{
public:
	using InvokerBase = InvokerBase<_Args...>;
	using InvokerBasePtr = std::shared_ptr<InvokerBase>;
	using InvokerBasePtrList = std::list<InvokerBasePtr>;
	using iterator = typename InvokerBasePtrList::iterator;

private:
	InvokerBasePtrList mInvokerBasePtrList;  // 등록된 호출자 리스트

public:
	~Invoker()
	{
		Clear();
	}

public:
	// 등록된 모든 함수를 호출한다.
	// - args: 전달할 인자들
	inline void Invoke(_Args&&... args)
	{
		for (std::shared_ptr<InvokerBase>& invokerBasePtr : mInvokerBasePtrList)
		{
			invokerBasePtr->Invoke(std::forward<_Args>(args)...);
		}
	}

	// 호출할 함수를 추가한다.
	// - owner: 함수를 소유하는 객체 포인터
	// - function: 호출할 함수
	// - return: 자기 자신 참조 (체이닝 가능)
	template <typename _Owner, typename _Function>
	Invoker<_Args...>& Add(_Owner* owner, _Function&& function)
	{
		std::shared_ptr<InvokerBase> invokerBasePtr = std::make_shared<InvokerImpl<_Owner, _Function, _Args...>>(owner, std::forward<_Function>(function));
		mInvokerBasePtrList.push_back(invokerBasePtr);
		return *this;
	}

	// 다른 Invoker의 함수들을 병합한다.
	// - invoker: 병합할 Invoker 참조
	void Merge(Invoker& invoker)
	{
		mInvokerBasePtrList.merge(invoker.mInvokerBasePtrList);
	}

public:
	// 등록된 모든 함수를 제거한다.
	void Clear()
	{
		mInvokerBasePtrList.clear();
	}

public:
	// 반복자 시작 위치를 반환한다.
	inline iterator begin()
	{
		return mInvokerBasePtrList.begin();
	}

	// 반복자 끝 위치를 반환한다.
	inline iterator end()
	{
		return mInvokerBasePtrList.end();
	}
};


#if 0

#include "Common/Invoker/Invoker.h"

class Npc
{
public:
	void OnDamaged(int a, int b)
	{
		std::wcout << a << b << L"_npc\n";
	}
};

class Pc
{
public:
	void OnDamaged2(int a, int b)
	{
		std::wcout << a << b << L"_pc\n";
	}
};

Invoker<int, int> mOnDamaged;

Npc npc;
Pc pc;

mOnDamaged.Add(&npc, &Npc::OnDamaged);
mOnDamaged.Add(&pc, &Pc::OnDamaged2);

mOnDamaged.Invoke(1, 2);

#endif
