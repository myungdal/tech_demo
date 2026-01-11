// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#include "GameAssetManager.h"

#include "Asset/Animation/CharacterAnimSet.h"
#include "Asset/Animation/EffectAnimSequence.h"
#include "Paper2D/Classes/PaperSprite.h"


void UGameAssetManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnInitialize();
}

void UGameAssetManager::Deinitialize()
{
	OnRelease();
	Super::Deinitialize();
}

void UGameAssetManager::OnInitialize()
{
	// Character AnimSet 로드
	for (int i = 0; i < static_cast<int>(CharacterType::MAX); ++i)
	{
		UCharacterAnimSet* animSet = NewObject<UCharacterAnimSet>();
		animSet->Load(static_cast<CharacterType>(i));
		mCharacterAnimSets.Emplace(animSet);
	}
	
	// Effect AnimSequence 로드
	for (int i = 0; i < static_cast<int>(EffectAnimType::MAX); ++i)
	{
		UEffectAnimSequence* sequence = NewObject<UEffectAnimSequence>();
		sequence->Load(static_cast<EffectAnimType>(i));
		mEffectAnimSequences.Emplace(sequence);
	}

	// Prop 로드
	for (int propIdx = 0; propIdx < 1; ++propIdx)
	{
		BrushArray brushArray;
		brushArray.SetNum(4);

		for (int spriteIdx = 0; spriteIdx < 4; ++spriteIdx)
		{
			FString name = FString::Printf(TEXT("/Game/Sprite/Prop/prop%d/prop%d_Sprite_%d.prop%d_Sprite_%d"), propIdx, propIdx, spriteIdx, propIdx, spriteIdx);

			if (UPaperSprite* sprite = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, *name)))
			{
				FSlateBrush& brush = brushArray[spriteIdx];
				brush.SetResourceObject(sprite);
				//brush.Mirroring = ESlateBrushMirrorType::Horizontal;
			}
		}

		mPropTable.Add(MoveTemp(brushArray));
	}
}

void UGameAssetManager::OnRelease()
{
}

const UCharacterAnimSet* UGameAssetManager::GetCharacterAnimSet(CharacterType characterType) const noexcept
{
	size_t idx = static_cast<size_t>(characterType);

	if (mCharacterAnimSets.Num() <= idx)
		return nullptr;

	UCharacterAnimSet* animSet = mCharacterAnimSets[idx];
	return animSet;
}

const FSlateBrush* UGameAssetManager::GetCharacterBrush(CharacterType characterType, CharacterAnimType animationType, float timePos) const noexcept
{
	if (const UCharacterAnimSet* animSet = GetCharacterAnimSet(characterType))
	{
		return animSet->GetBrush(animationType, timePos);
	}
	return nullptr;
}

const FSlateBrush* UGameAssetManager::GetPropBrush(int propIdx, int spriteIdx) const noexcept
{
	if (propIdx < 0 || propIdx >= mPropTable.Num())
		return nullptr;
	
	const BrushArray& brushArray = mPropTable[propIdx];
	
	if (spriteIdx < 0 || spriteIdx >= brushArray.Num())
		return nullptr;
	
	return &brushArray[spriteIdx];
}

const UEffectAnimSequence* UGameAssetManager::GetEffectAnimSequence(EffectAnimType effectType) const noexcept
{
	size_t idx = static_cast<size_t>(effectType);

	if (mEffectAnimSequences.Num() <= idx)
		return nullptr;

	UEffectAnimSequence* sequence = mEffectAnimSequences[idx];
	return sequence;
}

const FSlateBrush* UGameAssetManager::GetEffectBrush(EffectAnimType effectType, float timePos) const noexcept
{
	if (const UEffectAnimSequence* sequence = GetEffectAnimSequence(effectType))
	{
		return sequence->GetBrush(timePos);
	}
	return nullptr;
}
