// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "UiRowDataComponent.generated.h"


UCLASS()
class UUiRowDataComponent : public UObject
{
	GENERATED_BODY()

public:
	EntityIdp mEntityIdp = INVALID_ENTITY_IDP;
	ComponentType mComponentType = ComponentType::NONE;
};
