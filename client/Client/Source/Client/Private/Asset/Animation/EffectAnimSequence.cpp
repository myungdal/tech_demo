// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "EffectAnimSequence.h"

#include "Paper2D/Classes/PaperSprite.h"
#include "Styling/SlateBrush.h"

#include "ClientUtil/ClientStringUtil.h"


void UEffectAnimSequence::Load(EffectAnimType effectType) noexcept
{
	mBrushArray.Empty();

	// 유효하지 않은 타입 검사
	if (effectType == EffectAnimType::NONE)
		return;

	const wchar_t* effectTypeWStr = GetEffectAnimTypeString(effectType);
	const wchar_t* effectKey0 = GetEffectAnimTypeProgramKey0(effectType);
	const wchar_t* effectKey1 = GetEffectAnimTypeProgramKey1(effectType);

	// null 또는 빈 문자열 검사
	if (!effectTypeWStr || !effectKey0 || !effectKey1)
		return;

	FString effectTypeStr = ClientStringUtil::w_to_s(effectTypeWStr);
	if (effectTypeStr.IsEmpty())
		return;

	int idxFrom = FCString::Atoi(*ClientStringUtil::w_to_s(effectKey0));
	int idxTo = FCString::Atoi(*ClientStringUtil::w_to_s(effectKey1));

	// 유효하지 않은 인덱스 범위 검사
	if (idxFrom > idxTo || idxTo < 0)
		return;

	// 최대 크기 예약
	mBrushArray.Reserve(idxTo - idxFrom + 1);

	for (int i = idxFrom; i <= idxTo; ++i)
	{
		const FString name = FString::Printf(
			TEXT("PaperSprite'/Game/Sprite/Clip/%s/%s_Sprite_%d.%s_Sprite_%d'"),
			*effectTypeStr, *effectTypeStr, i,
			*effectTypeStr, i
		);

		// 존재하는 스프라이트만 배열에 추가 (없는 인덱스는 건너뜀)
		if (UPaperSprite* sprite = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, *name)))
		{
			FSlateBrush& brush = mBrushArray.AddDefaulted_GetRef();
			brush.SetResourceObject(sprite);
		}
	}
}

const FSlateBrush* UEffectAnimSequence::GetBrush(float timePos) const noexcept
{
	if (0 == mBrushArray.Num())
		return nullptr;

	size_t idx = static_cast<size_t>(timePos * mBrushArray.Num());
	if (mBrushArray.Num() <= idx)
		idx = mBrushArray.Num() - 1;

	return &mBrushArray[idx];
}


