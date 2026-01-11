// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Runtime/Core/Public/Modules/ModuleInterface.h"


class FMapEditorModule : public IModuleInterface
{
public:
	TSharedPtr<class FUICommandList> mUiCommandList;

public:
	void StartupModule() override;
	void ShutdownModule() override;
};
