// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/FoodSubmitTable.h"

#include "IngredientEnums.h"
#include "IngredientStruct.h"
#include "NotEvenGameMode.h"
#include "OrderListViewer.h"
#include "Plate.h"
#include "PlayerUI.h"
#include "SubmitFood.h"
#include "GameManager/OrderManager.h"

AFoodSubmitTable::AFoodSubmitTable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/KHB/Models/kitchentable_B.kitchentable_B'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}
	
}

void AFoodSubmitTable::BeginPlay()
{
	Super::BeginPlay();

	if (auto gm = Cast<ANotEvenGameMode>(GetWorld()->GetAuthGameMode()))
	{
		OrderManager = gm->OrderManager;
	}
}

void AFoodSubmitTable::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	if (auto plate = Cast<APlate>(player->OwnedObj))
	{
		if (plate->SubmitFood)
		{
			SubmitFood(plate->SubmitFood->GetIngredients(), plate);
		}
	}
		
}

void AFoodSubmitTable::SubmitFood(const TArray<struct FRecipeIngredientData>& ingredients, APlate* plate)
{
	OrderManager->CheckOrder(ingredients);
	//auto food = plate->SubmitFood;
	//plate->SubmitFood = nullptr;
	//food->Destroy();
}
