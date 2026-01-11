// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 인자 목록 파싱 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 문자열 기반의 인자 목록을 파싱하는 클래스 (FSM이나 치트 명령 처리 등에 사용).
// - 책임: 공백/쉼표로 구분된 인자 문자열 파싱
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
// - 형식: "arg0 arg1 arg2,arg3,arg4 arg5" (공백으로 인자 구분, 쉼표로 배열 요소 구분)
class COMMON_API ArgList
{
private:
	using List = std::vector<std::wstring>;  // 인자 리스트 (배열 요소 포함)
	using Table = std::vector<List>;         // 인자 테이블 (인자 리스트들의 배열)

	Table mTable;  // 파싱된 인자 테이블

public:
	DISABLE_COPY(ArgList);
	explicit ArgList(const std::wstring& argString);

public:
	// 전체 인자 문자열을 반환한다.
	// - return: 전체 인자 문자열
	std::wstring GetFullString();

	// 첫 번째 인자를 제외한 전체 인자 문자열을 반환한다.
	// - return: 첫 번째 인자 제외한 문자열
	std::wstring GetFullStringExceptFirst();

	// 인자 개수를 반환한다.
	// - return: 인자 개수
	size_t GetSize() const;

public:
	// 인자가 존재하는지 확인한다.
	// - i: 인자 인덱스
	// - return: 존재하면 true
	bool Has(int8_t i) const;

	// 정수 타입 인자를 가져온다.
	// - i: 인자 인덱스
	// - defaultValue: 기본값 (인자가 없을 때)
	// - return: 정수 값
	template<typename _Type>
	_Type GetInt(int8_t i, _Type defaultValue) const
	{
		const wchar_t* strValue = GetStr(i);
		// [EDGE-CASE] 빈 문자열이면 기본값 반환
		if (strValue[0] == L'\0')
			return defaultValue;

		uint64_t intValue = _wcstoui64(strValue, nullptr, 10);
		return static_cast<_Type>(intValue);
	}

	// 문자열 인자를 가져온다.
	// - i: 인자 인덱스
	// - return: 문자열 포인터 (없으면 빈 문자열)
	const wchar_t* GetStr(int8_t i) const;

public:
	// 배열 인자의 크기를 반환한다.
	// - i: 인자 인덱스
	// - return: 배열 크기
	size_t GetArraySize(int8_t i) const;
	
	// 배열 인자의 정수 값을 가져온다.
	// - i: 인자 인덱스
	// - j: 배열 요소 인덱스
	// - return: 정수 값
	template<typename _Type>
	_Type GetArrayInt(int8_t i, int8_t j) const
	{
		const wchar_t* strValue = GetArrayStr(i, j);
		uint64_t intValue = _wcstoui64(strValue, nullptr, 10);
		return static_cast<_Type>(intValue);
	}

	// 배열 인자의 문자열 값을 가져온다.
	// - i: 인자 인덱스
	// - j: 배열 요소 인덱스
	// - return: 문자열 포인터
	const wchar_t* GetArrayStr(int8_t i, int8_t j) const;

private:
	// 인자 리스트를 가져온다.
	// - i: 인자 인덱스
	// - return: 인자 리스트 포인터 (없으면 nullptr)
	const ArgList::List* GetList(int8_t i) const;
};

#if 0

#include "Common/Invoker/ArgList.h"

void Test()
{
	ArgList argList(L"1 2 3,4,5 6");
	//ArgList argList(L"1 , 3,4,5 6");
	//ArgList argList(L"1 , , 6");
	//ArgList argList(L"");

	_MAY_UNUSED auto v0 = argList.GetInt<int>(0);
	_MAY_UNUSED auto v1 = argList.GetInt<int>(1);
	_MAY_UNUSED auto v20 = argList.GetArrayInt<int>(2, 0);
	_MAY_UNUSED auto v21 = argList.GetArrayInt<int>(2, 1);
	_MAY_UNUSED auto v22 = argList.GetArrayInt<int>(2, 2);
	_MAY_UNUSED auto v3 = argList.GetInt<int>(3);
	_MAY_UNUSED auto full = argList.GetFullString();
	_MAY_UNUSED auto size = argList.GetSize();
	_MAY_UNUSED auto arraySize0 = argList.GetArraySize(0);
	_MAY_UNUSED auto arraySize1 = argList.GetArraySize(1);
	_MAY_UNUSED auto arraySize2 = argList.GetArraySize(2);
	_MAY_UNUSED auto arraySize3 = argList.GetArraySize(3);
}

#endif
