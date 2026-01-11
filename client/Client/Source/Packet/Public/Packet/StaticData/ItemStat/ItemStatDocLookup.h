// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct ItemStatDoc;

class PACKET_API ItemStatDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using ItemStatDocMap = std::unordered_map<ItemSid, ItemStatDoc*>;

	ItemStatDocMap mItemStatDocMap;

public:
	virtual ~ItemStatDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const ItemStatDoc* Find(ItemSid itemSid) const;
	const ItemStatDocMap& GetItemStatDocMap()  const { return mItemStatDocMap; }
};
