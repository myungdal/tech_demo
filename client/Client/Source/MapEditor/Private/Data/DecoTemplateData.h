// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Serialization/JsonWriter.h"
#include "DecoTemplateData.generated.h"


class FJsonObject;

UCLASS()
class UDecoTemplateData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString mName = TEXT("");

	UPROPERTY(EditAnywhere)
	int32 mValue0 = 0;

	UPROPERTY(EditAnywhere)
	int32 mValue1 = 0;

	UPROPERTY(EditAnywhere, Meta=(AllowedClasses="PaperSprite"))
	FSoftObjectPath mSprite;

public:
	void Save(TSharedRef<TJsonWriter<>> jsonWriter) const;
	void Load(const TSharedRef<FJsonObject> dataObj);
};
