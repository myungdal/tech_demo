// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "UObject/NoExportTypes.h"
#include "EffectAnimSequence.generated.h"

struct FSlateBrush;

UCLASS()
class UEffectAnimSequence : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TArray<FSlateBrush> mBrushArray;

public:
	void Load(EffectAnimType effectType) noexcept;
	const FSlateBrush* GetBrush(float timePos) const noexcept;
};


