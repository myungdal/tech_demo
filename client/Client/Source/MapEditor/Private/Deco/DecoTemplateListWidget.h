// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MapEditor/Private/MapEditorListWidget.h"
#include "MapEditor/Private/Data/DecoTemplateData.h"


class SDecoTemplateListWidget : public SMapEditorListWidget<UDecoTemplateData>
{
public:
	using Parent = SMapEditorListWidget<UDecoTemplateData>;
	using ItemPtr = Parent::ItemPtr;
	using ItemPtrList = Parent::ItemPtrList;
	using ListView = Parent::ListView;
	using ListViewPtr = Parent::ListViewPtr;

public:
	SLATE_BEGIN_ARGS(SDecoTemplateListWidget)
	{}
	SLATE_ARGUMENT(DataList*, dataList)
	SLATE_EVENT(ListView::FOnMouseButtonClick, OnMouseButtonClick)
	SLATE_EVENT(ListView::FOnMouseButtonDoubleClick, OnMouseButtonDoubleClick)
	SLATE_END_ARGS()

public:
	virtual ~SDecoTemplateListWidget();

	void Construct(const FArguments& InArgs);

public:
	TSharedRef<ITableRow> OnGenerateRow(SDecoTemplateListWidget::ItemPtr item, const TSharedRef<STableViewBase>& ownerTable);
};
