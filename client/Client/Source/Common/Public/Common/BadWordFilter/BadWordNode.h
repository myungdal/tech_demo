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
// BadWordNode - 트라이(Trie) 노드
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 트라이의 각 노드를 나타내는 구조체.
// - 각 노드는 하나의 문자에 대응 (루트는 빈 문자열)
// - mChildren: 다음 문자로의 전이를 저장하는 해시맵 (unordered_map)
// - mLeafType: 이 노드에서 단어가 끝나면 해당 타입 (NONE이면 중간 노드)
//
// 예: "bad", "bag" 등록 시 트라이 구조
//   root
//    └─ 'b'
//        └─ 'a'
//            ├─ 'd' (BLACK)
//            └─ 'g' (BLACK)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BadWordNode
{
	BadWordType mLeafType = BadWordType::NONE;  // 단어 종료 타입 (NONE: 중간 노드, BLACK/WHITE: 단어 끝)
	BadWordNodeMap mChildren;                    // 자식 노드 맵 (문자 → 다음 노드)

	~BadWordNode()
	{
		for (auto& [_, node] : mChildren)
		{
			_DELETE(node);
		}
	}
};

