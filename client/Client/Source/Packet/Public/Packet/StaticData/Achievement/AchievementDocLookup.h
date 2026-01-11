// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct AchievementDoc;

class PACKET_API AchievementDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using AchievementDocMap = std::unordered_map<AchievementSid, AchievementDoc*>;

	AchievementDocMap mAchievementDocMap;

public:
	virtual ~AchievementDocLookup();

private:
	friend class StaticDataAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const AchievementDoc* Find(AchievementSid achievementSid) const;
};
