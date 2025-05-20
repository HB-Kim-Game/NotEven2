// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListViewerBase.h"
#include "UIListViewerBase.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UUIListViewerBase : public UUserWidget, public IListViewerBase
{
	GENERATED_BODY()

public:
	template<typename T>
	void FetchDatas(const TArray<T*>& datas)
	{
		FetchedDatas.Empty();
		for (T* data : datas)
		{
			if (data)
			{
				auto* cast = Cast<UObject>(data);
				FetchedDatas.Add(cast);
			}
		}

		OnDataFetched();
	}

	virtual int32 GetCursor() const override;

	virtual void SetCursor(int32 value) override;

	virtual UObject* GetSelectedItem() const override;

	virtual void MoveCursor(int32 Gap, bool bIsRefresh = false) override;
	
	virtual void OnDataFetched() override
	{
		if (SpawnItems.Num() > 0)
		{
			RefreshOnDataFetched();
			// 새로 패치된 리스트 정보만 다시 뿌려줌.
			return;
		}

		InitializeItem();
	}

	virtual void RefreshOnDataFetched() {};

protected:

	UPROPERTY()
	TArray<UObject*> FetchedDatas;
	
	int32 Cursor = 0;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* Panel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ListViewer)
	int ItemPoolCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ListViewer)
	int CursorOffset = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ListViewer)
	TSubclassOf<class UUIViewerItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ListViewer)
	float VerticalOffset = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ListViewer)
	float HorizontalOffset = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ListViewer)
	float VercialInterval = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ListViewer)
	float HorizontalInterval = 0.0f;

	TArray<class UUIViewerItemBase*> SpawnItems;

	virtual void InitializeItem();
};
