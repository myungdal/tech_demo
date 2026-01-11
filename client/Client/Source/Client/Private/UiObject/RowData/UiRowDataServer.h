// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UiRowDataServer.generated.h"


class SERVER;

/**
 * 서버 목록 행 데이터
 */
UCLASS()
class UUiRowDataServer : public UObject
{
	GENERATED_BODY()

public:
	const SERVER* mData = nullptr;
};

