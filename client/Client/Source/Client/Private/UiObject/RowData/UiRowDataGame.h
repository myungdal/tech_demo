// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "UiRowDataGame.generated.h"


class GAME;

UCLASS()
class UUiRowDataGame : public UObject
{
	GENERATED_BODY()

public:
	const GAME* mData;
};
