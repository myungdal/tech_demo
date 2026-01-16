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

#include "StaticDbLoader.h"

bool StaticDbLoader::Load(SQLHENV env, OUT StaticDataCatalog& staticDataCatalog)
{	
	StaticDbSession staticDbSession(env, CommitType::AUTO);

	if (false == staticDbSession.IsReady())
	{
		_ASSERT_CRASH(false);
		return false;
	}

	{
		SP_STATIC_ACHIEVEMENT_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_ACHIEVEMENT::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_ACHIEVEMENT>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_ACHIEVEMENT_STEP_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_ACHIEVEMENT_STEP::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_ACHIEVEMENT_STEP>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_BUILDING_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_BUILDING::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_BUILDING>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_CHARACTER_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_CHARACTER::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_CHARACTER>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_CHARACTER_SKILL_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_CHARACTER_SKILL::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_CHARACTER_SKILL>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_GAME_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_GAME::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_GAME>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_ITEM_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_ITEM::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_ITEM>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_ITEM_STAT_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_ITEM_STAT::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_ITEM_STAT>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_MAIL_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_MAIL::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_MAIL>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_MAP_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_MAP::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_MAP>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_MISSION_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_MISSION::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_MISSION>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_PRODUCT_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_PRODUCT::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_PRODUCT>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_QUEST_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_QUEST::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_QUEST>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_REWARD_GROUP_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_REWARD_GROUP::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_REWARD_GROUP>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_REWARD_RATIO_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_REWARD_RATIO::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_REWARD_RATIO>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_REWARD_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_REWARD::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_REWARD>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_SCHEDULE_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_SCHEDULE::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_SCHEDULE>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_STAT_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_STAT::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_STAT>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_STOCK_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_STOCK::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_STOCK>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_TRANSLATION_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_TRANSLATION::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_TRANSLATION>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	{
		SP_STATIC_USER_EXP_SELECT_ALL sp(staticDbSession);
		Result result = DbUtil::FetchSp(sp,
			[&]()
			{
				STATIC_USER_EXP::Writer wp = DbUtil::SpToPacket(sp);
				staticDataCatalog.GetList<STATIC_USER_EXP>().emplace_back(wp.GetPacketBufPtr());
				return Result::SUCCEEDED;
			}
		);
		if (Result::SUCCEEDED != result)
			return false;
	}

	return true;
}

