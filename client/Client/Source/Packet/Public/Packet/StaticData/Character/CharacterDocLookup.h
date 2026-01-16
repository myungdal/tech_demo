// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct CharacterDoc;

class PACKET_API CharacterDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using CharacterDocMap = std::unordered_map<ItemSid, CharacterDoc*>;

	CharacterDocMap mCharacterDocMap;

public:
	virtual ~CharacterDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const CharacterDoc* Find(ItemSid characterItemSid) const;
};
