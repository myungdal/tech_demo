// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserQuest/UserQuestRow.h"

#include "Packet/StaticData/Quest/QuestDocLookup.h"
#include "Packet/StaticData/Quest/QuestDoc.h"
#include "Packet/StaticDataAccessor/StaticDataAccessor.h"
#include "Packet/StaticDataCatalog/StaticDataCatalog.h"


UserQuestRow::UserQuestRow(PacketKeep<QUEST>&& data)
	:
	mData(std::move(data))
{
}
const QuestDoc* UserQuestRow::GetDoc()
{
	if (!mDoc)
	{
		if (std::shared_ptr<QuestDocLookup> view = gStaticDataAccessor->Get<QuestDocLookup>())
		{
			const QUEST& quest = Data();

			if (const QuestDoc* doc = view->Find(quest.Get_c_quest_sid()))
			{
				mDoc = doc;
			}
		}
	}

	return mDoc;
}
