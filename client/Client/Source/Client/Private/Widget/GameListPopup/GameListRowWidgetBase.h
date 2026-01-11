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
#include "GameListRowWidgetBase.generated.h"


class UBorder;
class UTextBlock;
class GAME;

/**
 * 게임 목록 행 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UGameListRowWidgetBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	const GAME* mGameData = nullptr;

protected:
	UPROPERTY(meta = (BindWidget))
	UBorder* mBorder = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mGameId = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mGameSid = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mDateCreated = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void OnClicked();

protected:
	void NativeOnItemSelectionChanged(bool bIsSelected) override;
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
