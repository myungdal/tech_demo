// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "DecoData.h"

#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"


void UDecoData::Save(TSharedRef<TJsonWriter<>> jsonWriter) const
{
	jsonWriter->WriteValue(TEXT("mName"), FString::Printf(TEXT("%s"), *mName));
	jsonWriter->WriteValue(TEXT("mValue0"), FString::Printf(TEXT("%d"), mValue0));
	jsonWriter->WriteValue(TEXT("mValue1"), FString::Printf(TEXT("%d"), mValue1));
}

void UDecoData::Load(const TSharedRef<FJsonObject> dataObj)
{
	dataObj->TryGetStringField(TEXT("mName"), mName);

	dataObj->TryGetNumberField(TEXT("mValue0"), mValue0);

	dataObj->TryGetNumberField(TEXT("mValue1"), mValue1);
}
