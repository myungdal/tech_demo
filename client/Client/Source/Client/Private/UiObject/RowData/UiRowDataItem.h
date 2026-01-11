// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"

#include "Packet/UserCache/UserItem/UserItemRow.h"

#include "UiRowDataItem.generated.h"


UCLASS()
class UUiRowDataItem : public UObject
{
	GENERATED_BODY()

public:
	const UserItemRow* mData[4];
	bool mRedDot[4]{ false, false, false, false };
	bool mDisabled[4]{ false, false, false, false };
};
