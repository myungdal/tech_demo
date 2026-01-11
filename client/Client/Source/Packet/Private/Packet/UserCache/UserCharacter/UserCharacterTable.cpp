// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserCharacter/UserCharacterTable.h"

#include "Packet/UserCache/UserCharacter/UserCharacterRow.h"


UserCharacterRowPtr UserCharacterTable::Find(ItemId characterItemId)
{
	auto it = mTable.find(characterItemId);

	return (it != mTable.end()) ? it->second : nullptr;
}

void UserCharacterTable::Upsert(UserCharacterRowPtr data)
{
	const ItemId& characterItemId = data->Data().Get_c_item_id();

	auto it = mTable.find(characterItemId);

	if (it != mTable.end())
	{
		it->second = data;
	}
	else
	{
		mTable.emplace(characterItemId, data);
	}
}
