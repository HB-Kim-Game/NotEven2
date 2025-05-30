// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SubmitFood.h"

ASubmitFood::ASubmitFood()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UDataTable> tempTable(TEXT("/Script/Engine.DataTable'/Game/KHB/DataTable/SubmitMeshTable.SubmitMeshTable'"));

	if (tempTable.Succeeded())
	{
		MeshTable = tempTable.Object;
	}

}

void ASubmitFood::AddIngredient(FIngredientData data, EIngredientState state, float CookingProgress, FIngredientPlaceData placeData)
{
	if (Ingredients.Num() >= 4) return;

	UE_LOG(LogTemp, Warning, TEXT("Add"));
	
	FSubmitFoodIngredientData add = FSubmitFoodIngredientData();
	
	FRecipeIngredientData temp = FRecipeIngredientData();
	temp.IngredientID = data.IngredientID;
	temp.RequireState = state;
	temp.IconImagePath = data.IconAssetPath;

	add.IngredientData = temp;
	add.PlacementData = placeData;

	CurrentCookingProgress += CookingProgress;
	MaxCookingProgress += data.MaxCookingProgress;
	
	Ingredients.Add(add);

	FindMesh();
}

TArray<FRecipeIngredientData> ASubmitFood::GetIngredients() const
{
	TArray<FRecipeIngredientData> temp;
	for (auto i : Ingredients)
	{
		temp.Add(i.IngredientData);
	}
	
	return temp;
}

void ASubmitFood::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
}

void ASubmitFood::FindMesh()
{
	TArray<FSubmitFoodMeshData*> array;
	MeshTable->GetAllRows<FSubmitFoodMeshData>(nullptr, array);

	UE_LOG(LogTemp, Warning, TEXT("Count : %d"), array.Num());
	
	TArray<FRecipeIngredientData> temp = GetIngredients();
	
	if (auto find = array.FindByPredicate([this, temp](const FSubmitFoodMeshData* item)
	{
		UE_LOG(LogTemp,Warning,TEXT("ASubmitFood::[%s]"), *item->MeshAssetPath);
		bool condition = true;
		for (auto i : temp)
		{
			condition = item->Ingredients.ContainsByPredicate([i](const FRecipeIngredientData& recipe)
			{
				return i.IngredientID == recipe.IngredientID && i.RequireState == recipe.RequireState;
			});
		}

		return condition;
	}))
	{
		
		UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *((*find)->MeshAssetPath));
		MeshComp->SetStaticMesh(mesh);
	}
}