// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserMission/UserMissionTable.h"

#include "Packet/UserCache/UserMission/UserMissionRow.h"


UserMissionRowPtr UserMissionTable::Find(const MissionId& id)
{
	auto it = mTable.find(id);

	return (it != mTable.end()) ? it->second : nullptr;
}

void UserMissionTable::Upsert(UserMissionRowPtr data)
{
	const MissionId& missionId = data->Data().Get_c_mission_id();

	auto it = mTable.find(missionId);

	if (it != mTable.end())
	{
		it->second = data;
	}
	else
	{
		mTable.emplace(missionId, data);
	}
}
