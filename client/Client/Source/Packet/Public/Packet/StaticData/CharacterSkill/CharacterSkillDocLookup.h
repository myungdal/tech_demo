// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct CharacterSkillDoc;

class PACKET_API CharacterSkillDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using CharacterSkillDocMap = std::unordered_map<ItemSid, CharacterSkillDoc*>;

	CharacterSkillDocMap mCharacterSkillDocMap;

public:
	virtual ~CharacterSkillDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const CharacterSkillDoc* Find(ItemSid characterItemSid) const;
};
