// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "Widget/Base/WidgetBase.h"
#include "CheatWidgetBase.generated.h"


class UButtonWidgetBase;
class UListView;

/**
 * 개발/디버그용 치트 UI 위젯.
 */
UCLASS()
class UCheatWidgetBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mCloseBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UListView* mCheatListView = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	TSharedRef<SWidget> RebuildWidget() override;
};
