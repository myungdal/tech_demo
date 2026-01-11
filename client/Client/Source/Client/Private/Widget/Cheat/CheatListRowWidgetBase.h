// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CheatListRowWidgetBase.generated.h"


class UBorder;
class UTextBlock;

/**
 * 치트 목록 행 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UCheatListRowWidgetBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UBorder* mBorder = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mText = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClicked();

protected:
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
}; 
