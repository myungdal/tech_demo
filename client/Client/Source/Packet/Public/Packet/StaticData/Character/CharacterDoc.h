// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class STATIC_CHARACTER;
struct ItemDoc;
struct CharacterSkillDoc;

struct CharacterDoc
{
	const STATIC_CHARACTER* mStaticCharacter = nullptr;
	const ItemDoc* mItemDoc = nullptr;
	const CharacterSkillDoc* mCharacterSkillDoc = nullptr;
};
