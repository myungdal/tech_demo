// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Serialization/JsonWriter.h"
#include "MapData.generated.h"


class FJsonObject;

USTRUCT()
struct FFractalScale
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	uint8 mX = 0;

	UPROPERTY(EditAnywhere)
	uint8 mY = 0;

	UPROPERTY(EditAnywhere)
	uint8 mW = 0;
};

UCLASS()
class UMapData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 mMapSid = 0;

	UPROPERTY(EditAnywhere)
	FString mName = TEXT("");

	UPROPERTY(EditAnywhere)
	uint8 mTileTheme = 0;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "4", ClampMax = "11"))
	uint8 mWidthFactor = 8;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "4", ClampMax = "11"))
	uint8 mHeightFactor = 8;
	
	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<uint8> mLevelDivisionList;

	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<FFractalScale> mFractalScaleList;

	UPROPERTY(EditAnywhere)
	int32 mRandomSeed = 999;

	UPROPERTY(EditAnywhere)
	FString mInitialSystemGroupName = TEXT("");

	UPROPERTY(EditAnywhere)
	FString mInitialSystemGroupFsmStateName = TEXT("");

public:
	explicit UMapData()
	{
		mLevelDivisionList.Add(0);
		mLevelDivisionList.Add(30);
		mLevelDivisionList.Add(40);
		mLevelDivisionList.Add(50);
		mLevelDivisionList.Add(60);
		mLevelDivisionList.Add(70);
		mLevelDivisionList.Add(100);

		mFractalScaleList.Add(FFractalScale{8, 8, 1 });
		mFractalScaleList.Add(FFractalScale{9, 9, 2 });
		mFractalScaleList.Add(FFractalScale{13, 13, 3 });
		mFractalScaleList.Add(FFractalScale{14, 14, 4 });

		mRandomSeed = FMath::Rand();

		mInitialSystemGroupName = TEXT("");
		mInitialSystemGroupFsmStateName = TEXT("");
	}

public:
	void Save(TSharedRef<TJsonWriter<>> jsonWriter) const;
	void Load(const TSharedRef<FJsonObject> dataObj);
};
