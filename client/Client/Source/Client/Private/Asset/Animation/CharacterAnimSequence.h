// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "UObject/NoExportTypes.h"
#include "CharacterAnimSequence.generated.h"

struct FSlateBrush;

UCLASS()
class UCharacterAnimSequence : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TArray<FSlateBrush> mBrushArray;

public:
	void Load(CharacterType characterType, CharacterAnimType animationType) noexcept;
	const FSlateBrush* GetBrush(float timePos) const noexcept;
};


