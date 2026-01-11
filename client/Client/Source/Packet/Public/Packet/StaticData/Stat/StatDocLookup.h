// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct StatDoc;

class PACKET_API StatDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

public:
	using StatDocArray = std::array<StatDoc*, static_cast<size_t>(StatType::MAX)>;

private:
	StatDocArray mStatDocArray;

public:
	DISABLE_COPY(StatDocLookup);
	StatDocLookup();
	virtual ~StatDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const StatDoc* Find(StatType statType) const;
};
