// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IngredientUI.h"
#include "IngredientStruct.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UIngredientUI::ShowIngredient(FRecipeIngredientData data)
{
	if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *data.IconImagePath))
	{
		if (UImage* iconImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass()))
		{
			iconImage->SetBrushFromTexture(texture);
			iconImage->SetDesiredSizeOverride(FVector2D(64.f, 64.f));
			UVerticalBoxSlot* slot = IconPanel->AddChildToVerticalBox(iconImage);
			slot->SetPadding(FMargin(10.0f, 10.0f));
		}
	}

	for (auto state : data.RequireStates)
	{
		if (auto* texture = Icons.Find(state))
		{
			if (UImage* iconImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass()))
			{
				iconImage->SetBrushFromTexture(*texture);
				iconImage->SetDesiredSizeOverride(FVector2D(64.f, 64.f));
				UVerticalBoxSlot* slot = IconPanel->AddChildToVerticalBox(iconImage);
				slot->SetPadding(FMargin(10.0f, 5.0f, 10.0f, 15.f));
			}
		}
	}
}
