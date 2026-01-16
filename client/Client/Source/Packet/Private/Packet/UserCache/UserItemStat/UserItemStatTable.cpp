// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "Packet/UserCache/UserItemStat/UserItemStatTable.h"
#include "Packet/UserCache/UserItemStat/UserItemStatRow.h"

void UserItemStatTable::Upsert(const UserItemStatRowPtr& row)
{
	const ItemId itemId = row->Data().ConstRef_c_item_id();
	const StatType statType = row->Data().Get_c_stat_type();

	// ItemId로 StatMap 가져오기 (없으면 자동 생성)
	StatMap& statMap = mTable[itemId];

	// StatType을 키로 Row 저장 (기존 값 자동 교체)
	statMap[statType] = row;
}
