// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserCharacter/UserCharacterRow.h"

#include "Packet/StaticData/Character/CharacterDocLookup.h"
#include "Packet/StaticData/Character/CharacterDoc.h"
#include "Packet/StaticDataAccessor/StaticDataAccessor.h"
#include "Packet/StaticDataCatalog/StaticDataCatalog.h"
#include "Packet/UserCache/UserItem/UserItemRow.h"


UserCharacterRow::UserCharacterRow(
	PacketKeep<ITEM>&& data, 
	UserItemRowPtr itemRow
)
	:
	mData(std::move(data)),
	mUserItemRow(itemRow)
{
}
const CharacterDoc* UserCharacterRow::GetDoc()
{
	if (!mDoc)
	{
		if (std::shared_ptr<CharacterDocLookup> view = gStaticDataAccessor->Get<CharacterDocLookup>())
		{
			const ITEM& character = Data();

			if (const CharacterDoc* doc = view->Find(character.Get_c_item_sid()))
			{
				mDoc = doc;
			}
		}
	}

	return mDoc;
}
