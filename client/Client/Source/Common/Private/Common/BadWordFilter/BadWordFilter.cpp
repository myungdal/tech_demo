// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BadWordFilter - Trie(트라이) 기반 다중 패턴 문자열 검색
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 알고리즘: Trie-based Multi-Pattern Matching (트라이 기반 다중 패턴 매칭)
//
// 역사:
//   - Trie 자료구조는 1960년 Edward Fredkin이 명명 ("retrieval"에서 유래)
//   - 문자열 검색에서 가장 기본적이고 직관적인 접두사 트리 구조
//   - 관련 개선 알고리즘: Aho-Corasick (1975년, failure link 추가로 O(n) 달성)
//
// 시간 복잡도:
//   - InsertWord: O(m) - m은 단어 길이
//   - ValidateText: O(n * m) - n은 텍스트 길이, m은 가장 긴 금지어 길이
//     (Aho-Corasick 사용 시 O(n + z)로 개선 가능, z는 매칭 횟수)
//
// 공간 복잡도: O(ALPHABET_SIZE * M * K) - M은 평균 단어 길이, K는 단어 수
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Common/BadWordFilter/BadWordFilter.h"

#include "Common/BadWordFilter/BadWordNode.h"


BadWordFilter::BadWordFilter()
{
	mRoot = _NEW<BadWordNode>();
}

// 금지어를 트라이에 삽입한다.
// 예: "bad" 삽입 시 root → 'b' → 'a' → 'd'(BLACK) 경로 생성
void BadWordFilter::InsertWord(const std::wstring& word, BadWordType leafType)
{
	BadWordNode* node = mRoot;

	// 트라이에 문자별로 노드를 생성하며 경로를 만든다
	for (wchar_t ch : word)
	{
		BadWordNodeMap::iterator it = node->mChildren.find(ch);

		if (it == node->mChildren.end())
		{
			auto emplaceResult = node->mChildren.emplace(ch, _NEW<BadWordNode>());
			it = emplaceResult.first;
		}

		node = it->second;
	}

	// 마지막 노드에 타입을 설정한다 (이 노드가 금지어의 끝을 나타냄)
	node->mLeafType = leafType;
}

// 텍스트에 금지어가 포함되어 있는지 검증한다.
// 모든 시작 위치에서 트라이를 탐색하여 가장 긴 매칭을 찾는다.
bool BadWordFilter::ValidateText(const std::wstring& text) const
{
	const size_t len = text.length();

	// 모든 시작 위치에서 트라이 탐색을 시도한다 (Brute-force 방식)
	for (size_t i = 0; i < len; ++i)
	{
		BadWordType lastLeafType = BadWordType::NONE;
		BadWordNode* node = mRoot;

		// i 위치부터 트라이를 따라가며 매칭되는 금지어를 찾는다
		for (size_t j = i; j < len; ++j)
		{
			const wchar_t ch = text[j];
			const auto it = node->mChildren.find(ch);

			// 더 이상 매칭되는 경로가 없으면 종료
			if (it == node->mChildren.end())
				break;

			// 자식 노드로 이동
			node = it->second;

			// [중요] 이동 후 현재 노드의 타입을 체크한다
			// 예: "bad"와 "badword" 둘 다 금지어일 때, "bad" 매칭 후에도 계속 탐색하여
			//     더 긴 "badword"도 찾을 수 있도록 lastLeafType을 갱신한다
			if (node->mLeafType != BadWordType::NONE)
				lastLeafType = node->mLeafType;
		}

		// BLACK 타입 금지어가 발견되면 검증 실패
		if (lastLeafType == BadWordType::BLACK)
			return false;
	}

	return true;
}
