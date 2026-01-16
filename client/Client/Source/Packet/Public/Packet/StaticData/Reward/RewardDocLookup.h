// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


class STATIC_REWARD;
struct RewardDoc;

class PACKET_API RewardDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using RewardDocMap = std::unordered_map<RewardSid, RewardDoc*>;

	RewardDocMap mRewardDocMap;

public:
	virtual ~RewardDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const RewardDoc* Find(RewardSid rewardSid) const;
};
