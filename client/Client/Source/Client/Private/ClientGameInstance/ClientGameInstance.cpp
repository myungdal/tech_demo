// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================



#include "ClientGameInstance.h"

#include "HAL/IConsoleManager.h"

#include "SubSystem/UiServiceManager/UiServiceManager.h"
#include "SubSystem/UiAssetManager/UiAssetManager.h"
#include "SubSystem/NetworkManager/NetworkManager.h"
#include "SubSystem/GameAssetManager/GameAssetManager.h"
#include "SubSystem/NetworkContext/NetworkContext.h"
#include "SubSystem/DevPacketSystem/DevPacketSystem.h"
#include "UiService/Debug/UiServiceDebug.h"
#include "UiService/Game/UiServiceGame.h"
#include "UiService/Inventory/UiServiceInventory.h"
#include "UiService/RedDot/UiServiceRedDot.h"


void UClientGameInstance::OnStart()
{
	Super::OnStart();

	gCurrWorld = GetWorld();
		 
	// 브레이크 포인트로 디버깅 할 때 참조 목적
	gDevPacketSystem = UDevPacketSystem::Get(this);
	gGameAssetManager = UGameAssetManager::Get(this);
	gNetworkContext = UNetworkContext::Get(this);
	gNetworkManager = UNetworkManager::Get(this);
	gUiAssetManager = UUiAssetManager::Get(this);
	gUiServiceManager = UUiServiceManager::Get(this);

	// 브레이크 포인트로 디버깅 할 때 참조 목적
	gUiServiceDebug = gUiServiceManager->GetService<FUiServiceDebug>().Get();
	gUiServiceGame = gUiServiceManager->GetService<FUiServiceGame>().Get();
	gUiServiceInventory = gUiServiceManager->GetService<FUiServiceInventory>().Get();
	gUiServiceRedDot = gUiServiceManager->GetService<FUiServiceRedDot>().Get();

	IConsoleManager::Get().RegisterConsoleVariable(
		TEXT("testLineCnt"),
		0,
		TEXT("1) testLineCnt\n2) testLineCnt"),
		ECVF_Scalability | ECVF_RenderThreadSafe
	);
}
