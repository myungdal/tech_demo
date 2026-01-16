// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Common/Key/StringKey/StringKeyPool.h"


const wchar_t* StringKeyPool::GetHash(const wchar_t* key)
{
	// [WHY] 짧은 문자열 중복 방지를 위해 (메모리 절약)
	auto it0 = mKeySet0.find(key);
	if (mKeySet0.end() != it0)
	{
		return key;
	}

	// [WHY] 긴 문자열의 복사본을 반환하고 재사용
	auto it1 = mKeySet1.find(key);
	if (mKeySet1.end() != it1)
	{
		return it1->data();
	}

	// [WHY] 새로운 문자열이면 저장 후 재사용
	auto [it, newly] = mKeySet1.emplace(key);
	mKeySet0.emplace(it->data());
	return it->data();
}
