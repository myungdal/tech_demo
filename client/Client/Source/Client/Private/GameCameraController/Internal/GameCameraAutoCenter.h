// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Dev/macro.h"


class FMapRenderer;
struct WorldUser;
class Engine;

class FGameCameraAutoCenter
{
private:
	// 자동 카메라 모드 여부 (true: 캐릭터 따라감, false: 수동 카메라 이동)
	bool mIsAutoCameraMode = true;

public:
	DISABLE_COPY(FGameCameraAutoCenter);
	FGameCameraAutoCenter() = default;

	void TryAutoCameraCenter(float deltaTime, FMapRenderer& mapRenderer, const WorldUser& worldUser, Engine& engine);

	// 자동 카메라 모드 활성화 (WASD 이동, 명령 입력 시 호출)
	void EnableAutoMode() { mIsAutoCameraMode = true; }

	// 자동 카메라 모드 비활성화 (화면 드래그 시 호출)
	void DisableAutoMode() { mIsAutoCameraMode = false; }

	// 현재 자동 카메라 모드 여부
	bool IsAutoMode() const { return mIsAutoCameraMode; }
};

