// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserMission/UserMissionRow.h"

#include "Packet/StaticData/Mission/MissionDocLookup.h"
#include "Packet/StaticData/Mission/MissionDoc.h"
#include "Packet/StaticDataAccessor/StaticDataAccessor.h"
#include "Packet/StaticDataCatalog/StaticDataCatalog.h"


UserMissionRow::UserMissionRow(PacketKeep<MISSION>&& data)
	:
	mData(std::move(data))
{
}
const MissionDoc* UserMissionRow::GetDoc()
{
	if (!mDoc)
	{
		if (std::shared_ptr<MissionDocLookup> view = gStaticDataAccessor->Get<MissionDocLookup>())
		{
			const MISSION& mission = Data();

			if (const MissionDoc* doc = view->Find(mission.Get_c_mission_sid()))
			{
				mDoc = doc;
			}
		}
	}

	return mDoc;
}
