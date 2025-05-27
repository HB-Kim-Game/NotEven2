// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OrderListViewer.h"

#include "IngredientStruct.h"
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

bool UOrderListViewer::CheckOrderSuccess(const TArray<struct FRecipeIngredientData>& data)
{
	for (auto* item : SpawnItems)
	{
		auto* cast = Cast<UOrderItem>(item);

		if (nullptr == cast) continue;
		if (nullptr == cast->GetRecipeData()) continue;

		TArray<FRecipeIngredientData*> checkedData;

		for (auto& d : data)
		{
			if (auto find = cast->GetRecipeData()->RequiredIngredients.FindByPredicate([checkedData, d](const FRecipeIngredientData& recipe)
			{
					return recipe.IngredientID == d.IngredientID && recipe.RequireState == d.RequireState;
			}))
			{
				if (!checkedData.Contains(find))
				{
					checkedData.Add(find);
				}
				if (checkedData.Num() >= data.Num())
				{
					cast->OnOrderSuccess.Broadcast(cast->GetRecipeData());
					UE_LOG(LogTemp, Warning, TEXT("Order Success"));
					return true;
				}
				continue;
			}
			break;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No Same Order"));
	return false;
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
				this->OrderManager->AddFailure(data->Price);
				this->OrderManager->AddScore(-data->Price);
				this->OrderManager->PlayerUI->PriceUI->ShowCurrentScore();
				this->OrderManager->RemoveOrder(data, false);
			}));
			item->OnOrderSuccess.Add(FOnOrderSuccess::FDelegate::CreateLambda([this](URecipeData* data)
			{
				this->OrderManager->AddSuccess(data->Price);
				this->OrderManager->AddScore(data->Price * this->OrderManager->GetCurrentComboCount());
				this->OrderManager->PlayerUI->PriceUI->ShowCurrentScore();
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
