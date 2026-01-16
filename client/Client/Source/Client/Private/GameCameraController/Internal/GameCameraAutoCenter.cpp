// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "GameCameraAutoCenter.h"

#include "Common/PCH/pch_world.h"

#include "MmoSync/WorldManager/WorldUserRegistry/WorldUser.h"
#include "MmoSync/Engine/Engine.h"
#include "MmoSync/Component/PositionComponent.h"

#include "GameRenderer/Map/MapRenderer.h"

void FGameCameraAutoCenter::TryAutoCameraCenter(float deltaTime, FMapRenderer& mapRenderer, const WorldUser& worldUser, Engine& engine)
{
	// 수동 카메라 모드일 때는 자동 이동하지 않음
	if (!mIsAutoCameraMode)
		return;

	// WorldUser에 캐싱된 leaderEntityIdp 사용 (최적화)
	if (worldUser.leaderEntityIdp == INVALID_ENTITY_IDP)
		return;

	Entity* leaderEntity = engine.GetEntity(worldUser.leaderEntityIdp);
	if (!leaderEntity)
		return;

	PositionComponent* position = engine.TryGetComponent<PositionComponent>(*leaderEntity);
	if (!position)
		return;

	mapRenderer.SetCameraTargetByWorldPos(
		position->x,
		position->y
	);
	
	// Visibility는 카메라 타겟 변경 시 자동으로 업데이트됨 (UpdateViewport에서 처리)
}

