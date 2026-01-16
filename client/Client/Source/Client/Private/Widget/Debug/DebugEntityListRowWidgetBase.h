// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Blueprint\UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DebugEntityListRowWidgetBase.generated.h"


class UBorder;
class UTextBlock;

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UDebugEntityListRowWidgetBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UBorder* mBorder = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mText = nullptr;

protected:
	EntityIdp mEntityIdp = INVALID_ENTITY_IDP;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClicked();

protected:
	void NativeOnItemSelectionChanged(bool bIsSelected) override;
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
}; 
