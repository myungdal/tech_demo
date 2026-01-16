// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataCatalog/Internal/StaticDataListBase.h"


template <typename _StaticData>
class StaticDataList : public StaticDataListBase
{
private:
	friend class StaticDataCatalog;
	static inline size_t mStaticDataListIdx = 0;

private:
	std::vector<PacketKeep<_StaticData>> mList;

public:
	std::vector<PacketKeep<_StaticData>>& List() { return mList; }
};

