// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "Widget/Base/WidgetBase.h"
#include "GameListWidgetBase.generated.h"


class UButtonWidgetBase;
class UListView;
class UGameListGameDetailWidgetBase;

/**
 * 게임 목록 팝업 위젯 베이스.
 */
UCLASS(Abstract, Blueprintable)
class UGameListWidgetBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UListView* mGameListView = nullptr;

	UPROPERTY(meta = (BindWidget))
	UGameListGameDetailWidgetBase* mGameDetail = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mCloseBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButtonWidgetBase* mOkBtn = nullptr;

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
public:
	void OnDispatchPacket(UPacketPtr* packetPtr) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
	HandleResult OnPacket(class MC_ACK_GAME_LIST_OVER& rp);
};
