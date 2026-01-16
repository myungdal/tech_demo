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
#include "Widgets/Views/SListView.h"
#include "HAL/FileManager.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"

#include "ClientUtil/ClientFileUtil.h"


template<typename _DataType>
class SMapEditorListWidget : public SCompoundWidget
{
public:
	struct FItem
	{
		UPROPERTY()
		_DataType* mData = nullptr;
	};

	using ItemPtr = TSharedPtr<FItem>;
	using ItemPtrList = TArray<ItemPtr>;
	using ListView = SListView<ItemPtr>;
	using ListViewPtr = TSharedPtr<ListView>;

	using DataList = TArray<_DataType*>;

protected:
	FString mFilename = TEXT("");
	ListViewPtr mListView;
	ItemPtrList mItemList;
	DataList* mDataList = nullptr;	

public:
	void Load()
	{
		TSharedPtr<FJsonObject> jsonRoot = FClientFileUtil::LoadJsonFile(mFilename);
		if (nullptr == jsonRoot)
			return;

		const TArray<TSharedPtr<FJsonValue>>* dataList;
		if (false == jsonRoot->TryGetArrayField(TEXT("dataList"), dataList))
			return;

		mDataList->Empty(0);
		for (int32 i = 0; i < dataList->Num(); ++i)
		{
			TSharedPtr<FJsonObject> dataObj = (*dataList)[i]->AsObject();

			_DataType* data = NewObject<_DataType>();
			data->Load(dataObj.ToSharedRef());

			mDataList->Add(data);
		}

		mItemList.Empty(0);
		for (_DataType* data : *mDataList)
		{
			ItemPtr item = MakeShareable(new FItem);
			item->mData = data;

			mItemList.Add(item);
		}

		RebuildList();
	}

	void Save() const
	{
		FString jsonStr;
		TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<>::Create(&jsonStr);
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteArrayStart(TEXT("dataList"));
		for (_DataType* data : *mDataList)
		{
			jsonWriter->WriteObjectStart();
			data->Save(jsonWriter);
			jsonWriter->WriteObjectEnd();
		}
		jsonWriter->WriteArrayEnd();
		jsonWriter->WriteObjectEnd();
		jsonWriter->Close();

		FClientFileUtil::SaveTxtFile(mFilename, jsonStr);
	}

public:
	void RebuildList()
	{
		mListView->RebuildList();
	}

	FReply DeleteItem(ItemPtr item)
	{
		mDataList->Remove(item->mData);
		mItemList.Remove(item);		
		Save();
		RebuildList();

		return FReply::Handled();
	}

	FReply OnAddNewClicked()
	{
		_DataType* data = NewObject<_DataType>();
		mDataList->Add(data);

		ItemPtr itemPtr = MakeShareable(new FItem);
		itemPtr->mData = data;
		mItemList.Add(itemPtr);

		RebuildList();
		Save();

		return FReply::Handled();
	}
};

#if 0
#include "Serialization/ObjectReader.h"
#include "Serialization/ObjectWriter.h"

void Load()
{
	mDataList->Empty(0);
	mItemList.Empty(0);

	FString fullPath = FPaths::Combine(FPaths::ProjectSavedDir(), mFilename);
	TUniquePtr<FArchive> reader(IFileManager::Get().CreateFileReader(*fullPath));
	if (reader.IsValid())
	{
		uint32 listSize = 0;
		*reader << listSize;
		for (uint32 i = 0; i < listSize; ++i)
		{
			uint32 byteArraySize;
			*reader << byteArraySize;

			TArray<uint8> byteArray;
			uint8 zero = 0;
			byteArray.Init(zero, byteArraySize);
			reader->Serialize(byteArray.GetData(), byteArraySize);

			_DataType* data = NewObject<_DataType>();
			FObjectReader objectReader(data, byteArray, false, false);

			ItemPtr item = MakeShareable(new FItem);
			item->mData = data;

			mDataList->Add(data);
			mItemList.Add(item);
		}
		reader->Close();
	}

	RebuildList();
}

void Save() const
{
	FString fullPath = FPaths::Combine(FPaths::ProjectSavedDir(), mFilename);
	TUniquePtr<FArchive> writer(IFileManager::Get().CreateFileWriter(*fullPath));
	if (writer.IsValid())
	{
		uint32 listSize = mDataList->Num();
		*writer << listSize;
		for (_DataType* data : *mDataList)
		{
			TArray<uint8> byteArray;
			FObjectWriter objectWriter(data, byteArray, false, false, false, 0);

			uint32 byteArraySize = byteArray.Num();
			*writer << byteArraySize;
			writer->Serialize(byteArray.GetData(), byteArraySize);

			JsonUtility.FromJson

				//ItemPtr item2 = MakeShareable(new FItem);
				//item2->mData = NewObject<_DataType>();
				//FObjectReader objectReader(item2->mData, byteArray);
		}

		writer->Close();
	}
}
#endif
