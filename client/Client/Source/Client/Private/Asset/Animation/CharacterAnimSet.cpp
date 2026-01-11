// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "CharacterAnimSet.h"

#include "Asset/Animation/CharacterAnimSequence.h"

void UCharacterAnimSet::Load(CharacterType characterType) noexcept
{
	mSequenceArray.Empty();

	// 유효하지 않은 타입 검사
	if (characterType == CharacterType::NONE)
		return;

	mSequenceArray.Reserve(static_cast<int>(CharacterAnimType::MAX));

	for (int i = 0; i < static_cast<int>(CharacterAnimType::MAX); ++i)
	{
		UCharacterAnimSequence* sequence = NewObject<UCharacterAnimSequence>();
		if (sequence)
		{
			sequence->Load(characterType, static_cast<CharacterAnimType>(i));
			mSequenceArray.Emplace(sequence);
		}
	}
}

const UCharacterAnimSequence* UCharacterAnimSet::GetSequence(CharacterAnimType animationType) const noexcept
{
	// 유효하지 않은 타입 검사
	if (animationType == CharacterAnimType::NONE || animationType >= CharacterAnimType::MAX)
		return nullptr;

	int32 idx = static_cast<int32>(animationType);

	if (idx < 0 || mSequenceArray.Num() <= idx)
		return nullptr;

	return mSequenceArray[idx];
}

const FSlateBrush* UCharacterAnimSet::GetBrush(CharacterAnimType animationType, float timePos) const noexcept
{
	if (const UCharacterAnimSequence* sequence = GetSequence(animationType))
	{
		return sequence->GetBrush(timePos);
	}
	return nullptr;
}


