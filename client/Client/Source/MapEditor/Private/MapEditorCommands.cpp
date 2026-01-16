// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MapEditorCommands.h"

#include "MapEditor/Private/MapEditorToolkit.h"


#define LOCTEXT_NAMESPACE "MapEditor"

FMapEditorCommands::FMapEditorCommands()
	: TCommands<FMapEditorCommands>("MapEditorCommands", FText::FromString(TEXT("MapEditorCommands")), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FMapEditorCommands::RegisterCommands()
{
	UI_COMMAND(mCommand1, "OpenMapEditor", "", EUserInterfaceActionType::Button, FInputChord());
}

TSharedPtr<FMapEditorToolkit> FMapEditorActions::mMapEditorToolkit = nullptr;

void FMapEditorActions::Action1()
{
	//#include "Misc/MessageDialog.h"
	//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("777")));

	if (false == mMapEditorToolkit.IsValid())
	{
		TSharedPtr<FMapEditorToolkit> mapEditorToolkit(new FMapEditorToolkit());
		mMapEditorToolkit = mapEditorToolkit;
	}
	
	mMapEditorToolkit->Init();
}

#undef LOCTEXT_NAMESPACE
