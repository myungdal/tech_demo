// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"

#include "UiEvent/Base/UiMsgBase.h"
#include "UiEvent/UiEventType.h"

#include "UiEventBase.generated.h"


UCLASS()
class UUiEventBase : public UObject
{
	GENERATED_BODY()

protected:
	TSharedPtr<FUiMsgBase> mMsg = nullptr;

public:
	const TSharedPtr<FUiMsg<UUiEventBase>> GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEventBase>>(mMsg); }
	void SetMsg(TSharedPtr<FUiMsgBase> msg) { mMsg = msg; }

public:
	static constexpr UiEventType TYPE = UiEventType::NONE;
	virtual UiEventType GetUiEventType() const { return UiEventType::NONE; }
};

UCLASS()
class UUiEventEmpty : public UUiEventBase
{
	GENERATED_BODY()

protected:
	UiEventType mUiEventType = UiEventType::NONE;

public:
	void SetUiEventType(UiEventType uiEventType) { mUiEventType = uiEventType; }
	UiEventType GetUiEventType() const override	{ return mUiEventType; }

public:
	const TSharedPtr<FUiMsg<UUiEventEmpty>> GetMsg() const { return StaticCastSharedPtr<FUiMsg<UUiEventEmpty>>(mMsg); }
};
