// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct BuildingDoc;

class PACKET_API BuildingDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using BuildingDocMap = std::unordered_map<ItemSid, BuildingDoc*>;

	BuildingDocMap mBuildingDocMap;

public:
	virtual ~BuildingDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const BuildingDoc* Find(ItemSid buildingItemSid) const;
};
