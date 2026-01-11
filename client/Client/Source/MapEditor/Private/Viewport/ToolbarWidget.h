// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"


class SToolbarWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SToolbarWidget)
	{}
	SLATE_END_ARGS()

public:
	virtual ~SToolbarWidget();

	void Construct(const FArguments& InArgs);

protected:
	FReply OnReloadDebugInfoFile();
};
