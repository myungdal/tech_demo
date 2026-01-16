// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"


class FMapEditorToolkit;

class FMapEditorCommands : public TCommands<FMapEditorCommands>
{
public:
	TSharedPtr<FUICommandInfo> mCommand1;	

public:
	FMapEditorCommands();

public:
	virtual void RegisterCommands() override;
};

class FMapEditorActions
{
private:
	static TSharedPtr<FMapEditorToolkit> mMapEditorToolkit;

public:
	static void Action1();
};
