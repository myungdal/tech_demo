// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "SubSystem/UiServiceManager/UiServiceBase/UiServiceBase.h"


class FUiServiceDebug : public FUiServiceBase
{
private:
	friend class UUiServiceManager;
	static inline size_t mServiceIdx = 0;

private:
	std::atomic<TimeStep> mTimeStepToPlay = INVALID_TIME_STEP;
	EntityIdp mSelectedEntityIdp = INVALID_ENTITY_IDP;
	ComponentType mSelectedComponentType = ComponentType::NONE;

public:
	FUiServiceDebug();
	virtual ~FUiServiceDebug();

public:
	void SelectEntity(Scalar x, Scalar y);

public:
	void SetTimeStepToPlay(TimeStep timeStep) { mTimeStepToPlay.store(timeStep);  }
	TimeStep GetTimeStepToPlay() const { return mTimeStepToPlay.load(); }

	void SetSelectedEntityIdp(EntityIdp val) { mSelectedEntityIdp = val; }
	EntityIdp GetSelectedEntityIdp() const { return mSelectedEntityIdp;  }

	void SetSelectedComponentType(ComponentType val) { mSelectedComponentType = val; }
	ComponentType GetSelectedComponentType() const { return mSelectedComponentType; }

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}

public:
	void OnDispatchPacket(NetworkPacket& rp) override;
	HandleResult OnPacket(NetworkPacket& rp) { return HandleResult::OK; }
};
