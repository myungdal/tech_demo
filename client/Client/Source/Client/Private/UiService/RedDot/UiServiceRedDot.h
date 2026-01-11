// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

#include "SubSystem/UiServiceManager/UiServiceBase/UiServiceBase.h"


struct FUiRedDotNode;

class FUiServiceRedDot : public FUiServiceBase
{
private:
	friend class UUiServiceManager;
	static inline size_t mServiceIdx = 0;

private:	
	using RedDotNodeArray = std::array<FUiRedDotNode*, static_cast<size_t>(RedDotType::MAX)>;

	RedDotNodeArray mRedDotNodeArray;
	
public:
	DISABLE_COPY(FUiServiceRedDot);
	explicit FUiServiceRedDot();
	~FUiServiceRedDot();

private:
	void AttachChildToParent(RedDotType child, RedDotType parent);

public:
	bool HasRedDot(RedDotType redDotType) const;

public:
	void OnDispatchUiEvent(UUiEventBase* uiEvent) override;
	void OnUiEvent(UUiEventBase& uiEvent) {}
	void OnUiEvent(UUiEvent_RED_DOT_ADD& uiEvent);
	void OnUiEvent(UUiEvent_RED_DOT_REMOVE& uiEvent);
};
