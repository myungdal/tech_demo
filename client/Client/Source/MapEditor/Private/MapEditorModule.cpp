// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MapEditorModule.h"

#include "Modules/ModuleManager.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "LevelEditor.h"


#include "MapEditor/Private/MapEditorCommands.h"

IMPLEMENT_MODULE(FMapEditorModule, MapEditor);

void FMapEditorModule::StartupModule()
{
	FMapEditorCommands::Register();

	mUiCommandList = MakeShareable(new FUICommandList());
	mUiCommandList->MapAction(FMapEditorCommands::Get().mCommand1, FExecuteAction::CreateStatic(&FMapEditorActions::Action1), FCanExecuteAction());

	struct Menu
	{
		static void Create(FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.BeginSection("MapEditorMenu", FText::FromString("MapEditor"));
			MenuBuilder.AddMenuEntry(FMapEditorCommands::Get().mCommand1);
			MenuBuilder.EndSection();
		}
	};

	TSharedPtr<FExtender> extender = MakeShareable(new FExtender());
	extender->AddMenuExtension("WindowLayout", EExtensionHook::After, mUiCommandList, FMenuExtensionDelegate::CreateStatic(&Menu::Create));

	FLevelEditorModule& levelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditorModule.GetMenuExtensibilityManager()->AddExtender(extender);
}

void FMapEditorModule::ShutdownModule()
{
	FMapEditorCommands::Unregister();
}
