// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/StaticDataDocLookupBase/StaticDataDocLookupBase.h"


struct UserExpDoc;

class PACKET_API UserExpDocLookup final : public StaticDataDocLookupBase
{
private:
	friend class StaticDataAccessor;
	static inline size_t mViewIdx = 0;

private:
	using UserExpDocMap = std::map<UserExp, UserExpDoc*>;

	UserExpDocMap mUserExpDocMap;

public:
	virtual ~UserExpDocLookup();

private:
	friend class UserExpAccessor;

	void Clear() final;
	void Load() final;
	void Link() final;

public:
	const UserExpDoc* FindByExp(UserExp userExp) const;
	const UserExpDoc* FindByLevel(UserLevel userLevel) const;
};
