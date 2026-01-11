// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MapEditorToolkit.h"

#include "Framework/Docking/TabManager.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "MapEditor"

FMapEditorToolkit::FMapEditorToolkit()
{
	if (false == mTabManager.IsValid())
	{
		const TSharedRef<SDockTab> MajorTab = SNew(SDockTab).TabRole(ETabRole::MajorTab);
		mTabManager = FGlobalTabmanager::Get()->NewTabManager(MajorTab);
	}

	RegisterTabSpawners();
}

FMapEditorToolkit::~FMapEditorToolkit()
{
	UnregisterTabSpawners();
}

void FMapEditorToolkit::Init()
{
	if (mRootWindow.IsValid())
	{
		mRootWindow->GetNativeWindow()->SetWindowFocus();
		return;
	}

	FDisplayMetrics displayMetrics;
	FSlateApplication::Get().GetCachedDisplayMetrics(displayMetrics);
	const FVector2D pos(
		displayMetrics.PrimaryDisplayWorkAreaRect.Left, displayMetrics.PrimaryDisplayWorkAreaRect.Top
	);
	const FVector2D size(
		displayMetrics.PrimaryDisplayWorkAreaRect.Right - displayMetrics.PrimaryDisplayWorkAreaRect.Left,
		displayMetrics.PrimaryDisplayWorkAreaRect.Bottom - displayMetrics.PrimaryDisplayWorkAreaRect.Top
	);
	const FVector2D finalSize = size * 0.9f;
	const FVector2D finalPos = pos + (size - finalSize) * 0.5f;

	mRootWindow = SNew(SWindow)
		.AutoCenter(EAutoCenter::None)
		.Title(LOCTEXT("MapEditor", "MapEditorWindow"))
		.IsInitiallyMaximized(false)
		.ScreenPosition(finalPos)
		.ClientSize(finalSize)
		.CreateTitleBar(true)
		.SizingRule(ESizingRule::UserSized)
		.SupportsMinimize(true)
		.SupportsMaximize(true);

	mRootWindow->SetRequestDestroyWindowOverride(FRequestDestroyWindowOverride::CreateRaw(this, &FMapEditorToolkit::RequestDestroyWindowOverride));
	FSlateApplication::Get().AddWindow(mRootWindow.ToSharedRef(), false);
	FSlateApplication::Get().SetUnhandledKeyDownEventHandler(FOnKeyEvent::CreateRaw(this, &FMapEditorToolkit::OnKeyEvent));

	TSharedPtr<FTabManager::FLayout> editorLayout = FTabManager::NewLayout("MapEditorLayout")		
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(1.f)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)->SetSizeCoefficient(0.2f)
				->Split
				(
					FTabManager::NewStack()->SetSizeCoefficient(0.3f)
					->AddTab(FTabId("MapListTab"), ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()->SetSizeCoefficient(0.4f)
					->AddTab(FTabId("EntityListTab"), ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()->SetSizeCoefficient(0.3f)
					->AddTab(FTabId("DecoListTab"), ETabState::OpenedTab)
				)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)->SetSizeCoefficient(0.8f)				
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(8.f)
					->Split
					(
						FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)->SetSizeCoefficient(0.7f)
						->Split
						(
							FTabManager::NewStack()->SetSizeCoefficient(0.1f)
							->AddTab(FTabId("ToolbarTab"), ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()->SetSizeCoefficient(0.9f)
							->AddTab(FTabId("ViewportTab"), ETabState::OpenedTab)
						)
					)
					->Split
					(
						FTabManager::NewStack()->SetSizeCoefficient(0.3f)
						->AddTab(FTabId("DetailTab"), ETabState::OpenedTab)
					)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(2.f)
					->Split
					(
						FTabManager::NewStack()->SetSizeCoefficient(0.5f)
						->AddTab(FTabId("EntityTemplateListTab"), ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()->SetSizeCoefficient(0.5f)
						->AddTab(FTabId("DecoTemplateListTab"), ETabState::OpenedTab)
					)
				)
			)				
		)
	);

	TSharedPtr<SWidget> mainFrameContent = mTabManager->RestoreFrom(editorLayout.ToSharedRef(), mRootWindow, false);
	mRootWindow->SetContent(mainFrameContent.ToSharedRef());
	mRootWindow->ShowWindow();
}

TSharedRef<SDockTab> FMapEditorToolkit::SpawnMapListTab(const FSpawnTabArgs& Args)
{
	SAssignNew(mMapListWidget, SMapListWidget)
		.dataList(&mMapDataList)
		.OnMouseButtonClick(this, &FMapEditorToolkit::OnMapListMouseButtonClick);

	TSharedPtr<SDockTab> dockTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	dockTab->SetContent(mMapListWidget.ToSharedRef());

	return dockTab.ToSharedRef();
}

TSharedRef<SDockTab> FMapEditorToolkit::SpawnEntityListTab(const FSpawnTabArgs& Args)
{
	SAssignNew(mEntityListWidget, SEntityListWidget)
		.dataList(&mEntityDataList)
		.OnMouseButtonClick(this, &FMapEditorToolkit::OnEntityListMouseButtonClick);

		TSharedPtr<SDockTab> dockTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	dockTab->SetContent(mEntityListWidget.ToSharedRef());

	return dockTab.ToSharedRef();
}

TSharedRef<SDockTab> FMapEditorToolkit::SpawnDecoListTab(const FSpawnTabArgs& Args)
{
	SAssignNew(mDecoListWidget, SDecoListWidget)
		.dataList(&mDecoDataList)
		.OnMouseButtonClick(this, &FMapEditorToolkit::OnDecoListMouseButtonClick);

		TSharedPtr<SDockTab> dockTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	dockTab->SetContent(mDecoListWidget.ToSharedRef());

	return dockTab.ToSharedRef();
}

TSharedRef<SDockTab> FMapEditorToolkit::SpawnEntityTemplateListTab(const FSpawnTabArgs& Args)
{
	SAssignNew(mEntityTemplateListWidget, SEntityTemplateListWidget)
		.dataList(&mEntityTemplateDataList)
		.OnMouseButtonClick(this, &FMapEditorToolkit::OnEntityTemplateListMouseButtonClick);

	TSharedPtr<SDockTab> dockTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	dockTab->SetContent(mEntityTemplateListWidget.ToSharedRef());

	return dockTab.ToSharedRef();
}

TSharedRef<SDockTab> FMapEditorToolkit::SpawnDecoTemplateListTab(const FSpawnTabArgs& Args)
{
	SAssignNew(mDecoTemplateListWidget, SDecoTemplateListWidget)
		.dataList(&mDecoTemplateDataList)
		.OnMouseButtonClick(this, &FMapEditorToolkit::OnDecoTemplateListMouseButtonClick);

	TSharedPtr<SDockTab> dockTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	dockTab->SetContent(mDecoTemplateListWidget.ToSharedRef());

	return dockTab.ToSharedRef();
}

TSharedRef<SDockTab> FMapEditorToolkit::SpawnToolbarTab(const FSpawnTabArgs& Args)
{
	SAssignNew(mToolbarWidget, SToolbarWidget);

	TSharedPtr<SDockTab> dockTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	dockTab->SetContent(mToolbarWidget.ToSharedRef());

	return dockTab.ToSharedRef();
}

TSharedRef<SDockTab> FMapEditorToolkit::SpawnViewportTab(const FSpawnTabArgs& Args)
{
	SAssignNew(mViewportWidget, SViewportWidget);
	
	TSharedPtr<SDockTab> dockTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	dockTab->SetContent(mViewportWidget.ToSharedRef());

	return dockTab.ToSharedRef();
}

TSharedRef<SDockTab> FMapEditorToolkit::SpawnDetailTab(const FSpawnTabArgs& Args)
{
	FPropertyEditorModule& propertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FDetailsViewArgs args;
	args.bUpdatesFromSelection = false;
	args.bLockable = false;
	args.bAllowSearch = false;
	args.NameAreaSettings = FDetailsViewArgs::ObjectsUseNameArea;

	mDetailView = propertyEditorModule.CreateDetailView(args);
	mDetailView->OnFinishedChangingProperties().AddSP(this, &FMapEditorToolkit::OnDetailViewFinishedChangingProperties);

	mDetailView->SetObject(nullptr);

	TSharedPtr<SDockTab> dockTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	dockTab->SetContent(mDetailView.ToSharedRef());

	return dockTab.ToSharedRef();
}

void FMapEditorToolkit::RegisterTabSpawners()
{
	mTabManager->RegisterTabSpawner(FName("MapListTab"), FOnSpawnTab::CreateRaw(this, &FMapEditorToolkit::SpawnMapListTab));
	mTabManager->RegisterTabSpawner(FName("EntityListTab"), FOnSpawnTab::CreateRaw(this, &FMapEditorToolkit::SpawnEntityListTab));
	mTabManager->RegisterTabSpawner(FName("DecoListTab"), FOnSpawnTab::CreateRaw(this, &FMapEditorToolkit::SpawnDecoListTab));

	mTabManager->RegisterTabSpawner(FName("EntityTemplateListTab"), FOnSpawnTab::CreateRaw(this, &FMapEditorToolkit::SpawnEntityTemplateListTab));
	mTabManager->RegisterTabSpawner(FName("DecoTemplateListTab"), FOnSpawnTab::CreateRaw(this, &FMapEditorToolkit::SpawnDecoTemplateListTab));

	mTabManager->RegisterTabSpawner(FName("ToolbarTab"), FOnSpawnTab::CreateRaw(this, &FMapEditorToolkit::SpawnToolbarTab));
	mTabManager->RegisterTabSpawner(FName("ViewportTab"), FOnSpawnTab::CreateRaw(this, &FMapEditorToolkit::SpawnViewportTab));
	
	mTabManager->RegisterTabSpawner(FName("DetailTab"), FOnSpawnTab::CreateRaw(this, &FMapEditorToolkit::SpawnDetailTab));
}

void FMapEditorToolkit::UnregisterTabSpawners()
{
	mTabManager->UnregisterTabSpawner(FName("MapListTab"));
	mTabManager->UnregisterTabSpawner(FName("EntityListTab"));
	mTabManager->UnregisterTabSpawner(FName("DecoListTab"));

	mTabManager->UnregisterTabSpawner(FName("EntityTemplateListTab"));
	mTabManager->UnregisterTabSpawner(FName("DecoTemplateListTab"));

	mTabManager->UnregisterTabSpawner(FName("ToolbarTab"));
	mTabManager->UnregisterTabSpawner(FName("ViewportTab"));
	
	mTabManager->UnregisterTabSpawner(FName("DetailTab"));
}

void FMapEditorToolkit::RequestDestroyWindowOverride(const TSharedRef<SWindow>& window)
{
	FSlateApplicationBase::Get().RequestDestroyWindow(window);
	mTabManager->CloseAllAreas();
	mRootWindow = nullptr;
}

FReply FMapEditorToolkit::OnKeyEvent(const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.IsControlDown() && InKeyEvent.GetCharacter() == 'S') 
	{
		// 저장
	}

	return FReply::Handled();
}

void FMapEditorToolkit::OnMapListMouseButtonClick(SMapListWidget::ItemPtr item)
{
	mSelectedMapListItem = item;
	
	if (nullptr != mSelectedMapListItem)
	{
		mDetailView->SetObject(mSelectedMapListItem->mData);

		mEntityListWidget->Load(mSelectedMapListItem);
		mDecoListWidget->Load(mSelectedMapListItem);
		mViewportWidget->Load(mSelectedMapListItem);
	}
}

void FMapEditorToolkit::OnEntityListMouseButtonClick(SEntityListWidget::ItemPtr item)
{
	if (nullptr == item)
		return;

	mDetailView->SetObject(item->mData);
}

void FMapEditorToolkit::OnDecoListMouseButtonClick(SDecoListWidget::ItemPtr item)
{
	if (nullptr == item)
		return;

	mDetailView->SetObject(item->mData);
}

void FMapEditorToolkit::OnEntityTemplateListMouseButtonClick(SEntityTemplateListWidget::ItemPtr item)
{
	if (nullptr == item)
		return;

	mDetailView->SetObject(item->mData);
}

void FMapEditorToolkit::OnDecoTemplateListMouseButtonClick(SDecoTemplateListWidget::ItemPtr item)
{
	if (nullptr == item)
		return;

	mDetailView->SetObject(item->mData);
}

void FMapEditorToolkit::OnDetailViewFinishedChangingProperties(const FPropertyChangedEvent& event)
{
	bool isMapData = false;
	bool isEntityData = false;
	bool isDecoData = false;
	bool isEntityTemplateData = false;
	bool isDecoTemplateData = false;	
	const TArray<TWeakObjectPtr<UObject>>& selected = mDetailView->GetSelectedObjects();
	for (TWeakObjectPtr<UObject> obj : selected)
	{
		if (Cast<UMapData>(obj)) isMapData = true;
		if (Cast<UEntityData>(obj)) isEntityData = true;
		if (Cast<UDecoData>(obj)) isDecoData = true;
		if (Cast<UEntityTemplateData>(obj)) isEntityTemplateData = true;
		if (Cast<UDecoTemplateData>(obj)) isDecoTemplateData = true;		
	}

	if (isMapData)
	{
		mMapListWidget->Save();
		mMapListWidget->RebuildList();
	}
	if (isEntityData)
	{
		mEntityListWidget->Save();
		mEntityListWidget->RebuildList();
	}
	if (isDecoData)
	{
		mDecoListWidget->Save();
		mDecoListWidget->RebuildList();
	}
	if (isEntityTemplateData)
	{
		mEntityTemplateListWidget->Save();
		mEntityTemplateListWidget->RebuildList();
	}
	if (isDecoTemplateData)
	{
		mDecoTemplateListWidget->Save();
		mDecoTemplateListWidget->RebuildList();
	}

	if (nullptr != mSelectedMapListItem)
	{
		mViewportWidget->Load(mSelectedMapListItem);
	}
}

#undef LOCTEXT_NAMESPACE
