// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OrderListViewer.h"

#include "OrderItem.h"
#include "PlayerUI.h"
#include "PriceUI.h"
#include "UIViewerItemBase.h"
#include "Components/HorizontalBox.h"
#include "Data/RecipeData.h"
#include "GameManager/OrderManager.h"

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

	for (int j = 0; j < SpawnItems.Num(); j++)
	{
		SpawnItems[j]->FetchData(nullptr);
		SpawnItems[j]->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	for (int i = 0; i < FetchedDatas.Num(); i++)
	{
		SpawnItems[i]->FetchData(FetchedDatas[i]);
		SpawnItems[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	CurrentOrderCount = FetchedDatas.Num();
}

void UOrderListViewer::SetOrderManager(class UOrderManager* orderManager)
{
	OrderManager = orderManager;
}

void UOrderListViewer::CheckOrderSuccess(TArray<FRecipeIngredientData>)
{
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

			item->OnOrderFailed.Add(FOnOrderFailed::FDelegate::CreateLambda([item,this](URecipeData* data)
			{
				this->CurrentComboCount = 0;
				this->OrderManager->PlayerUI->PriceUI->ShowCurrentScore(-data->Price);
				this->OrderManager->AddScore(-data->Price);
				this->OrderManager->RemoveOrder(data, false);
			}));
			item->OnOrderSuccess.Add(FOnOrderSuccess::FDelegate::CreateLambda([this](URecipeData* data)
			{
				this->CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 0, 4);
				this->OrderManager->PlayerUI->PriceUI->ShowCurrentScore(data->Price * CurrentComboCount);
				this->OrderManager->AddScore(data->Price * CurrentComboCount);
				this->OrderManager->RemoveOrder(data, true);
			}));
		}
	}
	
	for (int i = 0; i < FetchedDatas.Num(); i++)
	{
		SpawnItems[i]->FetchData(FetchedDatas[i]);
		SpawnItems[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	CurrentOrderCount = FetchedDatas.Num();
}
