// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "TimerManager.h"
#include "Widget/Base/WidgetBase.h"
#include "DebugWidgetBase.generated.h"


class UListView;
class UTextBlock;
class UEditableTextBox;
class UButtonWidgetBase;

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UDebugWidgetBase : public UWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mCloseBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mPauseBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mPlayBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mStepBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mExportBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UListView* mTestEntityListView = nullptr;

	UPROPERTY(meta = (BindWidget))
	UListView* mTestComponentListView = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mTestComponentDetailText = nullptr;

private:
	FTimerHandle mTimerHandleReloadDebugInfoFile;
	FTimerHandle mTimerHandleUpdateComponentDetail;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

private:
	void OnReloadDebugInfoFile();
	void OnUpdateComponentDetail();
	
public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_DEBUG_PAUSE_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_DEBUG_PLAY_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_DEBUG_STEP_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_DEBUG_EXPORT_CLICKED& uiEvent);
	void OnUiEvent(UUiEvent_DEBUG_ENTITY_SELECTED& uiEvent);
	void OnUiEvent(UUiEvent_DEBUG_COMPONENT_SELECTED& uiEvent);
};
