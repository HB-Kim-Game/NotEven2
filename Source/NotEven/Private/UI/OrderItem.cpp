// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OrderItem.h"
#include "IngredientStruct.h"
#include "IngredientUI.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Data/RecipeData.h"

void UOrderItem::FetchData(UObject* Data)
{
	Super::FetchData(Data);


	if (auto cast = Cast<URecipeData>(Data))
	{
		RecipeData = cast;
		CurrentCookingTime = RecipeData->MaxCookingTime;
		
		for (auto* i : Ingredients) i->SetVisibility(ESlateVisibility::Collapsed);
		
		for (int j = 0; j < cast->RequiredIngredients.Num(); j++)
		{
			if (j >= Ingredients.Num())
			{
				if (UIngredientUI* item = CreateWidget<UIngredientUI>(GetWorld(), IngredientUIClass))
				{
					Ingredients.Add(item);
					IngredientPanel->AddChildToHorizontalBox(item);
					item->ShowIngredient(cast->RequiredIngredients[j]);
					continue;
				}
			}
			
			Ingredients[j]->SetVisibility(ESlateVisibility::HitTestInvisible);
			Ingredients[j]->ShowIngredient(cast->RequiredIngredients[j]);
		}

		if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *cast->IconImagePath))
		{
			IconImage->SetBrushFromTexture(texture);
		}
	}
	
}

void UOrderItem::Selected()
{
	Super::Selected();
}

void UOrderItem::Deselected()
{
	Super::Deselected();
}

void UOrderItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (nullptr == RecipeData) return;

	float maxCookingTime = RecipeData->MaxCookingTime;
	if (CurrentCookingTime <= 0.f)
	{
		RecipeData = nullptr;
		CurrentCookingTime = 0.f;
		SetVisibility(ESlateVisibility::Collapsed);
	}
	
	CurrentCookingTime -= InDeltaTime;
	float percent = CurrentCookingTime / maxCookingTime;
	RemainProgress->SetPercent(percent);
	FLinearColor color = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, percent);
	RemainProgress->SetFillColorAndOpacity(color);
}
