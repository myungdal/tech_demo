// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "UiRowDataEntity.generated.h"


UCLASS()
class UUiRowDataEntity : public UObject
{
	GENERATED_BODY()

public:
	EntityIdp mEntityIdp = INVALID_ENTITY_IDP;	
};
