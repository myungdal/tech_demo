// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct ItemDoc;

class PACKET_API ItemDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using ItemDocMap = std::unordered_map<ItemSid, ItemDoc*>;

	ItemDocMap mItemDocMap;

public:
	virtual ~ItemDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const ItemDoc* Find(ItemSid itemSid) const;
	const ItemDocMap& GetItemDocMap()  const { return mItemDocMap; }
};
