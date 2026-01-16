// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/BadWordFilter/BadWordType.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BadWordFilter - Trie(트라이) 기반 금지어 필터
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 알고리즘: Trie-based Multi-Pattern Matching (트라이 기반 다중 패턴 매칭)
//
// Trie (Prefix Tree, 접두사 트리):
//   - 1960년 Edward Fredkin이 명명 ("retrieval"에서 유래, "트라이"로 발음)
//   - 문자열 집합을 효율적으로 저장하고 검색하기 위한 트리 자료구조
//   - 각 노드는 문자를 나타내며, 루트에서 리프까지의 경로가 하나의 단어를 형성
//
// 사용 예:
//   BadWordFilter filter;
//   filter.InsertWord(L"금지어", BadWordType::BLACK);
//   filter.InsertWord(L"허용어", BadWordType::WHITE);
//   bool isValid = filter.ValidateText(L"이 문장에 금지어가 있습니다");  // false 반환
//
// 관련 알고리즘:
//   - Aho-Corasick (1975): failure link를 추가하여 O(n) 선형 시간 달성
//   - Rabin-Karp: 해시 기반 다중 패턴 매칭
//
// - 책임: 금지어 등록 및 텍스트 검증
// - 스레드 모델: 단일 스레드 (동시성 보호 없음)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class COMMON_API BadWordFilter
{
private:
	BadWordNode* mRoot = nullptr;  // 트라이 루트 노드 (빈 문자열을 나타냄)

public:
	BadWordFilter();

	// 금지어를 트라이에 추가한다.
	// - word: 추가할 금지어 (UTF-16)
	// - leafType: 금지어 타입 (BLACK: 차단, WHITE: 허용)
	// - 시간 복잡도: O(m), m = 단어 길이
	void InsertWord(const std::wstring& word, BadWordType leafType);

	// 텍스트에 금지어가 포함되어 있는지 검증한다.
	// - text: 검증할 텍스트 (UTF-16)
	// - return: BLACK 타입 금지어가 발견되면 false, 그 외 true
	// - 시간 복잡도: O(n * m), n = 텍스트 길이, m = 가장 긴 금지어 길이
	bool ValidateText(const std::wstring& text) const;
};
