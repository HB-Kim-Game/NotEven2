// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUI.h"

#include "IngredientStruct.h"
#include "OrderListViewer.h"
#include "PlayTimeUI.h"
#include "Data/RecipeData.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayTime->SetMaxTime(180.f);
	PlayTime->SetIsPlaying(true);
}
