// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OrderListViewer.h"

#include "IngredientStruct.h"
#include "OrderItem.h"
#include "PlayerUI.h"
#include "PriceUI.h"
#include "UIViewerItemBase.h"
#include "Animation/WidgetAnimation.h"
#include "Components/HorizontalBox.h"
#include "Data/RecipeData.h"
#include "GameManager/OrderManager.h"
#include "Kismet/GameplayStatics.h"

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
		SpawnItems[j]->StopAllAnimations();
		SpawnItems[j]->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	for (int i = 0; i < FetchedDatas.Num(); i++)
	{
		SpawnItems[i]->FetchData(FetchedDatas[i]);
		SpawnItems[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (FetchedDatas.Num() > CurrentOrderCount)
	{
		SpawnItems[FetchedDatas.Num() - 1]->Selected();
	}

	CurrentOrderCount = FetchedDatas.Num();
}

void UOrderListViewer::SetOrderManager(class AOrderManager* orderManager)
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
		if (cast->bIsChecked) continue;

		TArray<FRecipeIngredientData*> checkedData;
		
		for (auto& d : data)
		{
			if (auto find = cast->GetRecipeData()->RequiredIngredients.FindByPredicate([d](const FRecipeIngredientData& recipe)
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

	for (auto* item : SpawnItems)
	{
		auto* cast = Cast<UOrderItem>(item);
		cast->StopAllAnimations();
		cast->PlayAnimation(cast->Failed);
	}

	if (NoOrderSound) UGameplayStatics::PlaySound2D(GetWorld(), NoOrderSound);
	
	UE_LOG(LogTemp, Warning, TEXT("No Same Order"));
	return false;
}

void UOrderListViewer::NativeConstruct()
{
	Super::NativeConstruct();
	
	OnAnimFinished.BindDynamic(this, &UOrderListViewer::OnRefresh);
}

void UOrderListViewer::OnRefresh()
{
	if (OrderManager)
	OrderManager->RefreshOrder();
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
			OrderManager->OnGameEnd.Add(FSimpleDelegate::CreateLambda([item]()
			{
				item->FetchData(nullptr);
			}));
			
			SpawnItems.Add(item);
			item->SetVisibility(ESlateVisibility::Collapsed);
			castPanel->AddChildToHorizontalBox(item);

			item->OnOrderFailed.Add(FOnOrderFailed::FDelegate::CreateLambda([item,this](URecipeData* data)
			{
				if (this->OrderManager->HasAuthority())
				{
					this->OrderManager->AddFailure(data->Price / 2);
					this->OrderManager->AddScore(-data->Price / 2);	
					this->OrderManager->RemoveOrder(data, false);
					item->BindToAnimationFinished(item->Failed, this->OnAnimFinished);
				}
				if (FailSound) UGameplayStatics::PlaySound2D(GetWorld(), FailSound);
				item->bIsChecked = true;
			}));
			item->OnOrderSuccess.Add(FOnOrderSuccess::FDelegate::CreateLambda([item,this](URecipeData* data)
			{
				if (this->OrderManager->HasAuthority())
				{
					this->OrderManager->AddSuccess(data->Price);
					this->OrderManager->AddScore(data->Price + 8.f * this->OrderManager->GetCurrentComboCount());	
					this->OrderManager->RemoveOrder(data, true);
				}

				if (SuccessSound)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), SuccessCoinSound);
					UGameplayStatics::PlaySound2D(GetWorld(), SuccessSound);
				}
				item->bIsChecked = true;
				
				item->BindToAnimationFinished(item->Success, this->OnAnimFinished);
			}));
		}
	}
	
	for (int i = 0; i < FetchedDatas.Num(); i++)
	{
		SpawnItems[i]->FetchData(FetchedDatas[i]);
		SpawnItems[i]->Selected();
		SpawnItems[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	CurrentOrderCount = FetchedDatas.Num();
}

int32 UOrderListViewer::GetSpawnItemsCount() const
{
	return SpawnItems.Num();
}
