// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MapData.h"

#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"


void UMapData::Save(TSharedRef<TJsonWriter<>> jsonWriter) const
{
	jsonWriter->WriteValue(TEXT("mMapSid"), FString::Printf(TEXT("%d"), mMapSid));
	jsonWriter->WriteValue(TEXT("mName"), FString::Printf(TEXT("%s"), *mName));
	jsonWriter->WriteValue(TEXT("mTileTheme"), FString::Printf(TEXT("%d"), mTileTheme));
	jsonWriter->WriteValue(TEXT("mWidthFactor"), FString::Printf(TEXT("%d"), mWidthFactor));
	jsonWriter->WriteValue(TEXT("mHeightFactor"), FString::Printf(TEXT("%d"), mHeightFactor));
	jsonWriter->WriteArrayStart(TEXT("mLevelDivisionList"));
	for (uint8 value : mLevelDivisionList)
	{
		jsonWriter->WriteValue(FString::Printf(TEXT("%d"), value));
	}
	jsonWriter->WriteArrayEnd();
	jsonWriter->WriteArrayStart(TEXT("mFractalScaleList"));
	for (FFractalScale value : mFractalScaleList)
	{
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("mX"), FString::Printf(TEXT("%d"), value.mX));
		jsonWriter->WriteValue(TEXT("mY"), FString::Printf(TEXT("%d"), value.mY));
		jsonWriter->WriteValue(TEXT("mW"), FString::Printf(TEXT("%d"), value.mW));
		jsonWriter->WriteObjectEnd();
	}
	jsonWriter->WriteArrayEnd();
	jsonWriter->WriteValue(TEXT("mRandomSeed"), FString::Printf(TEXT("%d"), mRandomSeed));
	jsonWriter->WriteValue(TEXT("mInitialSystemGroupName"), FString::Printf(TEXT("%s"), *mInitialSystemGroupName));
	jsonWriter->WriteValue(TEXT("mInitialSystemGroupFsmStateName"), FString::Printf(TEXT("%s"), *mInitialSystemGroupFsmStateName));
}

void UMapData::Load(const TSharedRef<FJsonObject> dataObj)
{
	mMapSid = 0;
	dataObj->TryGetNumberField(TEXT("mMapSid"), mMapSid);

	mName = TEXT("");
	dataObj->TryGetStringField(TEXT("mName"), mName);

	int32 _mTileTheme = 0;
	dataObj->TryGetNumberField(TEXT("mTileTheme"), _mTileTheme);
	mTileTheme = _mTileTheme;

	int32 _mWidthFactor = 0;
	dataObj->TryGetNumberField(TEXT("mWidthFactor"), _mWidthFactor);
	mWidthFactor = _mWidthFactor;

	int32 _mHeightFactor = 0;
	dataObj->TryGetNumberField(TEXT("mHeightFactor"), _mHeightFactor);
	mHeightFactor = _mHeightFactor;

	{
		mLevelDivisionList.Empty(0);
		const TArray<TSharedPtr<FJsonValue>>* valueList;
		if (true == dataObj->TryGetArrayField(TEXT("mLevelDivisionList"), valueList))
		{
			for (int32 i = 0; i < valueList->Num(); ++i)
			{
				int32 value = 0;
				(*valueList)[i]->TryGetNumber(value);
				mLevelDivisionList.Push(value);
			}
		}
	}

	{
		mFractalScaleList.Empty(0);
		const TArray<TSharedPtr<FJsonValue>>* valueList;
		if (true == dataObj->TryGetArrayField(TEXT("mFractalScaleList"), valueList))
		{
			for (int32 i = 0; i < valueList->Num(); ++i)
			{
				const TSharedPtr<FJsonObject>* obj;
				if (false == (*valueList)[i]->TryGetObject(obj))
					continue;

				int32 _mX;
				(*obj)->TryGetNumberField(TEXT("mX"), _mX);

				int32 _mY;
				(*obj)->TryGetNumberField(TEXT("mY"), _mY);

				int32 _mW;
				(*obj)->TryGetNumberField(TEXT("mW"), _mW);

				mFractalScaleList.Push(FFractalScale{ 
					static_cast<uint8>(_mX),
					static_cast<uint8>(_mY),
					static_cast<uint8>(_mW)
				});
			}
		}
	}

	int32 _mRandomSeed;
	dataObj->TryGetNumberField(TEXT("mRandomSeed"), _mRandomSeed);
	mRandomSeed = _mRandomSeed;

	mInitialSystemGroupName = TEXT("");
	dataObj->TryGetStringField(TEXT("mInitialSystemGroupName"), mInitialSystemGroupName);

	mInitialSystemGroupFsmStateName = TEXT("");
	dataObj->TryGetStringField(TEXT("mInitialSystemGroupFsmStateName"), mInitialSystemGroupFsmStateName);
}
