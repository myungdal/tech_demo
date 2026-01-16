// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "UObject/NoExportTypes.h"
#include "CharacterAnimSet.generated.h"

class UCharacterAnimSequence;
struct FSlateBrush;

UCLASS()
class UCharacterAnimSet : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TArray<UCharacterAnimSequence*> mSequenceArray;

public:
	void Load(CharacterType characterType) noexcept;

public:
	const UCharacterAnimSequence* GetSequence(CharacterAnimType animationType) const noexcept;
	const FSlateBrush* GetBrush(CharacterAnimType animationType, float timePos) const noexcept;
};


