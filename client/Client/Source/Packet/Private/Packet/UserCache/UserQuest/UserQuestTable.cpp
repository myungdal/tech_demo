// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserQuest/UserQuestTable.h"

#include "Packet/UserCache/UserQuest/UserQuestRow.h"


UserQuestRowPtr UserQuestTable::Find(QuestSid sid)
{
	auto it = mTable.find(sid);

	return (it != mTable.end()) ? it->second : nullptr;
}

void UserQuestTable::Upsert(UserQuestRowPtr data)
{
	const QuestSid questSid = data->Data().Get_c_quest_sid();

	auto it = mTable.find(questSid);

	if (it != mTable.end())
	{
		it->second = data;
	}
	else
	{
		mTable.emplace(questSid, data);
	}
}
