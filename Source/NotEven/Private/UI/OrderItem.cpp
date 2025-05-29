// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OrderItem.h"
#include "IngredientStruct.h"
#include "IngredientUI.h"
#include "Animation/WidgetAnimation.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Data/RecipeData.h"

void UOrderItem::FetchData(UObject* Data)
{
	Super::FetchData(Data);

	if (Data == nullptr)
	{
		RecipeData = nullptr;
	}
	
	if (auto cast = Cast<URecipeData>(Data))
	{
		RecipeData = cast;
		bIsReceiveOrder = true;
		
		float percent = cast->CurrentCookingTime / cast->MaxCookingTime;
		RemainProgress->SetPercent(percent);
		FLinearColor color = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, percent);
		RemainProgress->SetFillColorAndOpacity(color);
		
		Ingredients.Empty();
		IngredientPanel->ClearChildren();
		
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

class URecipeData* UOrderItem::GetRecipeData() const
{
	return RecipeData;
}

void UOrderItem::Selected()
{
	Super::Selected();
}

void UOrderItem::Deselected()
{
	Super::Deselected();
}

void UOrderItem::NativeConstruct()
{
	Super::NativeConstruct();

	OnOrderSuccess.Add(FOnOrderSuccess::FDelegate::CreateLambda([this](URecipeData* data)
	{
		this->StopAllAnimations();
		this->PlayAnimation(Success);
	}));

	OnOrderFailed.Add(FOnOrderSuccess::FDelegate::CreateLambda([this](URecipeData* data)
	{
		this->StopAllAnimations();
		this->PlayAnimation(Failed);
	}));

	OnAnimationFinished.BindDynamic(this, &UOrderItem::AnimationFinished);

	BindToAnimationFinished(Success, OnAnimationFinished);
	BindToAnimationFinished(Failed, OnAnimationFinished);
}

void UOrderItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (nullptr == RecipeData) return;
	if (!bIsReceiveOrder) return;

	if (RecipeData->CurrentCookingTime <= 0.f)
	{
		bIsReceiveOrder = false;
		OnOrderFailed.Broadcast(RecipeData);
		return;
	}
	
	RecipeData->CurrentCookingTime -= InDeltaTime;
	float percent = RecipeData->CurrentCookingTime / RecipeData->MaxCookingTime;
	RemainProgress->SetPercent(percent);
	FLinearColor color = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, percent);
	RemainProgress->SetFillColorAndOpacity(color);
}

void UOrderItem::AnimationFinished()
{
	bIsChecked = false;
}
