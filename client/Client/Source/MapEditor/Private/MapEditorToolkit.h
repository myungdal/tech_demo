// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "CoreMinimal.h"

#include "MapEditor/Private/Map/MapListWidget.h"
#include "MapEditor/Private/Entity/EntityListWidget.h"
#include "MapEditor/Private/Deco/DecoListWidget.h"

#include "MapEditor/Private/Entity/EntityTemplateListWidget.h"
#include "MapEditor/Private/Deco/DecoTemplateListWidget.h"

#include "MapEditor/Private/Viewport/ToolbarWidget.h"
#include "MapEditor/Private/Viewport/ViewportWidget.h"


class SWindow;
class FTabManager;
class SDockTab;
class FSpawnTabArgs;
class FReply;
struct FKeyEvent;
class IDetailsView;

class FMapEditorToolkit : public TSharedFromThis<FMapEditorToolkit>
{
private:
	TSharedPtr<SWindow> mRootWindow = nullptr;
	TSharedPtr<FTabManager> mTabManager = nullptr;

private:
	TSharedPtr<SMapListWidget> mMapListWidget = nullptr;
	TSharedPtr<SEntityListWidget> mEntityListWidget = nullptr;
	TSharedPtr<SDecoListWidget> mDecoListWidget = nullptr;
	
	TSharedPtr<SToolbarWidget> mToolbarWidget = nullptr;
	TSharedPtr<SViewportWidget> mViewportWidget = nullptr;

	TSharedPtr<SDecoTemplateListWidget> mDecoTemplateListWidget = nullptr;
	TSharedPtr<SEntityTemplateListWidget> mEntityTemplateListWidget = nullptr;

	TSharedPtr<IDetailsView> mDetailView = nullptr;

private:
	TArray<UMapData*> mMapDataList;

	TArray<UEntityData*> mEntityDataList;
	TArray<UDecoData*> mDecoDataList;

	TArray<UEntityTemplateData*> mEntityTemplateDataList;
	TArray<UDecoTemplateData*> mDecoTemplateDataList;

private:
	SMapListWidget::ItemPtr mSelectedMapListItem = nullptr;

public:
	DISABLE_COPY(FMapEditorToolkit);
	explicit FMapEditorToolkit();
	virtual ~FMapEditorToolkit();

	void Init();

private:
	TSharedRef<SDockTab> SpawnMapListTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnEntityListTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnDecoListTab(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnEntityTemplateListTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnDecoTemplateListTab(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnToolbarTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnViewportTab(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnDetailTab(const FSpawnTabArgs& Args);

public:
	void RegisterTabSpawners();
	void UnregisterTabSpawners();
	void RequestDestroyWindowOverride(const TSharedRef<SWindow>& window);
	FReply OnKeyEvent(const FKeyEvent& InKeyEvent);

public:	
	void OnMapListMouseButtonClick(SMapListWidget::ItemPtr item);
	void OnEntityListMouseButtonClick(SEntityListWidget::ItemPtr item);
	void OnDecoListMouseButtonClick(SDecoListWidget::ItemPtr item);
	void OnEntityTemplateListMouseButtonClick(SEntityTemplateListWidget::ItemPtr item);
	void OnDecoTemplateListMouseButtonClick(SDecoTemplateListWidget::ItemPtr item);
	
	void OnDetailViewFinishedChangingProperties(const FPropertyChangedEvent& event);	
};
