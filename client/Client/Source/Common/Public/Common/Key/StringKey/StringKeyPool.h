// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StringKey 소유자 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// StringKey가 사용하는 문자열 해시를 관리하는 클래스.
// - 책임: 문자열 해시 저장 및 중복 제거
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
class StringKeyPool
{
private:
	std::unordered_set<const wchar_t*> mKeySet0;      // 원본 포인터 저장 (짧은 문자열)
	std::unordered_set<std::wstring> mKeySet1;        // 복사본 저장 (긴 문자열)

public:
	// 문자열의 해시 포인터를 반환한다. 없으면 저장 후 반환.
	// - key: 원본 문자열 (UTF-16)
	// - return: 해시된 문자열 포인터
	const wchar_t* GetHash(const wchar_t* key);
};

