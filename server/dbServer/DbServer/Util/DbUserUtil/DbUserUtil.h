// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class UserDbSession;
class UserCacheAccessor;

namespace DbUserUtil
{
	Result IncreaseUserExp(UserDbSession& userDbSession, UserCacheAccessor& userCache, UserExp userExpDelta);
}

