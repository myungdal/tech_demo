// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEnginePacket;
import ServerEngineDb;

#include "DbUserUtil.h"

Result DbUserUtil::IncreaseUserExp(UserDbSession& userDbSession, UserCacheAccessor& userCache, UserExp userExpDelta)
{
	Result result = Result::SUCCEEDED;
	do
	{
		if (0 == userExpDelta)
		{
			result = Result::WRONG_VALUE;
			break;
		}
		USER& user = userCache.GetUser();
		UserExp userExp = (user.Get_c_user_exp() + userExpDelta);
		if (userExp > MAX_USER_EXP)
		{
			userExp = MAX_USER_EXP;
		}
		const UserLevel userLevel = MIN_USER_LEVEL;

		SP_USER_UPDATE_EXP spUserUpdateExp(
			userDbSession,
			user.Get_c_user_id(),
			userExp,
			userLevel,
			tClock.GetGlobalNowTs()
		);
		result = DbUtil::ExecuteSp(spUserUpdateExp);
		if (Result::SUCCEEDED != result)
			break;
		
		user.Ref_c_user_exp() = userExp;

	} while (false);

	return result;
}

