// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IngredientActorIcon.h"

#include "Components/Image.h"

void UIngredientActorIcon::NativeConstruct()
{
	Super::NativeConstruct();

	IconMatDynamic = UMaterialInstanceDynamic::Create(IconMaterialInstance, this);
	IconImage->SetBrushFromMaterial(IconMatDynamic);
}

void UIngredientActorIcon::SetIconImage(class UTexture2D* Icon)
{
	if (nullptr == Icon)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	SetVisibility(ESlateVisibility::HitTestInvisible);
	IconMatDynamic->SetTextureParameterValue("IconImage", Icon);
}
