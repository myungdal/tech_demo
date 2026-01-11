// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "CoreMinimal.h"
#include "PacketPtr.generated.h"

class NetworkPacket;

UCLASS()
class UPacketPtr : public UObject
{
	GENERATED_BODY()

public:
	NetworkPacket* rp;
};
