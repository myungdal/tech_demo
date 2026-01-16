// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "ButtonWidgetBase.generated.h"


class UUiButton;
class UImage;
class UTextBlock;

/**
 * 공용 버튼 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UButtonWidgetBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UUiButton* mButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* mButtonIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* mRedDot = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mName = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void SetupButton(UiEventType uiEventTypeClicked, const TCHAR* name);

	void SetNameVisible(bool visible);
	void SetRedDotVisible(bool visible);
};
