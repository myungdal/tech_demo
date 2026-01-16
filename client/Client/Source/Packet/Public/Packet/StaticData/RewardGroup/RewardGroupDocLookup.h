// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct RewardGroupDoc;

class PACKET_API RewardGroupDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using RewardGroupDocMap = std::unordered_map<RewardSid, RewardGroupDoc*>;

	RewardGroupDocMap mRewardGroupDocMap;

public:
	virtual ~RewardGroupDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const RewardGroupDoc* Find(RewardSid rewardSid) const;
};
