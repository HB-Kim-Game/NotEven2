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
	if (!IconMatDynamic) SetMaterial();
	IconMatDynamic->SetTextureParameterValue(FName("IconImage"), Icon);	
}

void UIngredientActorIcon::SetMaterial()
{
	IconMatDynamic = UMaterialInstanceDynamic::Create(IconMaterialInstance, this);
	IconImage->SetBrushFromMaterial(IconMatDynamic);	
}
