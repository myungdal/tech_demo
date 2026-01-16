// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientPlayerController.generated.h"

/**
 * 클라이언트 전용 PlayerController.
 */
UCLASS()
class AClientPlayerController : public APlayerController
{
		GENERATED_BODY()
protected:
	UTouchInterface* mTouchInterface = nullptr;

public:
	   bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

	   void SetTouchInterfaceEnabled(bool bEnabled);

protected:
	   void BeginPlay() override;

private:
	   UPROPERTY()
	   UTouchInterface* MyTouchInterface = nullptr;
};
