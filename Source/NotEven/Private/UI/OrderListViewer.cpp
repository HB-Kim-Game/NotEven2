// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OrderListViewer.h"

#include "OrderItem.h"
#include "UIViewerItemBase.h"
#include "Components/HorizontalBox.h"

int32 UOrderListViewer::GetCursor() const
{
	return Super::GetCursor();
}

void UOrderListViewer::SetCursor(int32 value)
{
	Super::SetCursor(value);
}

UObject* UOrderListViewer::GetSelectedItem() const
{
	return Super::GetSelectedItem();
}

void UOrderListViewer::MoveCursor(int32 Gap, bool bIsRefresh)
{
	Super::MoveCursor(Gap, bIsRefresh);
}

void UOrderListViewer::RefreshOnDataFetched()
{
	Super::RefreshOnDataFetched();

	for (auto item : SpawnItems)
	{
		item->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	for (int i = 0; i < FetchedDatas.Num(); i++)
	{
		SpawnItems[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
		// i가 CurrentOrderCount 이상일 경우 애니메이션 재생
	}

	CurrentOrderCount = FetchedDatas.Num();
}

void UOrderListViewer::OnDataFetched()
{
	Super::OnDataFetched();

	MoveCursor(0);
}

void UOrderListViewer::InitializeItem()
{
	Super::InitializeItem();

	SpawnItems.Empty();
	UHorizontalBox* castPanel = Cast<UHorizontalBox>(Panel);
	for (int32 i = 0; i < ItemPoolCount; i++)
	{
		if (UOrderItem* item = Cast<UOrderItem>(CreateWidget(GetWorld(), ItemClass)))
		{
			SpawnItems.Add(item);
			item->SetVisibility(ESlateVisibility::Collapsed);
			castPanel->AddChildToHorizontalBox(item);
		}
	}
	
	for (int i = 0; i < FetchedDatas.Num(); i++)
	{
		SpawnItems[i]->FetchData(FetchedDatas[i]);
		SpawnItems[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	CurrentOrderCount = FetchedDatas.Num();
}
