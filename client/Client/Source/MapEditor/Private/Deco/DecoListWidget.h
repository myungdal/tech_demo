// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MapEditor/Private/MapEditorListWidget.h"
#include "MapEditor/Private/Data/DecoData.h"
#include "MapEditor/Private/Map/MapListWidget.h"


class SDecoListWidget : public SMapEditorListWidget<UDecoData>
{
public:
	using Parent = SMapEditorListWidget<UDecoData>;
	using ItemPtr = Parent::ItemPtr;
	using ItemPtrList = Parent::ItemPtrList;
	using ListView = Parent::ListView;
	using ListViewPtr = Parent::ListViewPtr;

public:
	SLATE_BEGIN_ARGS(SDecoListWidget)
	{}
	SLATE_ARGUMENT(DataList*, dataList)
	SLATE_EVENT(ListView::FOnMouseButtonClick, OnMouseButtonClick)
	SLATE_EVENT(ListView::FOnMouseButtonDoubleClick, OnMouseButtonDoubleClick)
	SLATE_END_ARGS()

public:
	virtual ~SDecoListWidget();

	void Construct(const FArguments& InArgs);

	void Load(SMapListWidget::ItemPtr item);

public:
	TSharedRef<ITableRow> OnGenerateRow(SDecoListWidget::ItemPtr item, const TSharedRef<STableViewBase>& ownerTable);
};
