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
#include "ServerListRowWidgetBase.generated.h"


class UBorder;
class UTextBlock;
class SERVER;

/**
 * 서버 목록의 개별 행 위젯
 */
UCLASS(Abstract, Blueprintable)
class UServerListRowWidgetBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	const SERVER* mServerData = nullptr;

protected:
	UPROPERTY(meta = (BindWidget))
	UBorder* mBorder = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mServerId = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mServerName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* mServerState = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void OnClicked();

protected:
	void NativeOnItemSelectionChanged(bool bIsSelected) override;
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
