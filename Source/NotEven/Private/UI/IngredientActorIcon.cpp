// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IngredientActorIcon.h"

#include "Components/Image.h"

void UIngredientActorIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IconMatDynamic) SetMaterial();
}

void UIngredientActorIcon::SetIconImage(UTexture2D* Icon)
{
	if (nullptr == Icon)
	{
		UE_LOG(LogTemp, Error, TEXT("Icon Texture is NULL"));
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	SetVisibility(ESlateVisibility::HitTestInvisible);

	if (!IconMatDynamic) SetMaterial();
	IconMatDynamic->SetTextureParameterValue(FName("IconImage"), Icon);	
}

void UIngredientActorIcon::SetMaterial()
{
	IconMatDynamic = UMaterialInstanceDynamic::Create(IconMaterialInstance, this);
	IconImage->SetBrushFromMaterial(IconMatDynamic);	
}
