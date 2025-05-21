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

	UDataTable* temp = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/KHB/DataTable/RecipeTable.RecipeTable'"));

	if (temp->GetRowNames().Num() > 0)
	{
		if (FRecipe* data = temp->FindRow<FRecipe>(FName("RawFish"), FString("")))
		{
			UE_LOG(LogTemp, Warning, TEXT("RawFish"));
			auto* tempObject = NewObject<URecipeData>();
			tempObject->RequiredIngredients = data->RequiredIngredients;
			tempObject->IconImagePath = data->IconImagePath;
			tempObject->MaxCookingTime = data->MaxCookingTime;

			TArray<URecipeData*> tempArray;
			tempArray.Add(tempObject);

			OrderListViewer->FetchDatas(tempArray);
		}
	}

}
