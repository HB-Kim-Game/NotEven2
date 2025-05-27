// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/FoodSubmitTable.h"

#include "IngredientEnums.h"
#include "IngredientStruct.h"
#include "NotEvenGameMode.h"
#include "OrderListViewer.h"
#include "PlayerUI.h"
#include "GameManager/OrderManager.h"

AFoodSubmitTable::AFoodSubmitTable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	BoxComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxComp->SetCollisionProfileName(FName("UnGrabObj"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);

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

	// 임시
	TArray<FRecipeIngredientData> array;
	FRecipeIngredientData d1;
	d1.IngredientID = TEXT("Fish");
	d1.RequireState = EIngredientState::Sliced;
	array.Add(d1);
		
	this->SubmitFood(array);
}

void AFoodSubmitTable::SubmitFood(const TArray<struct FRecipeIngredientData>& ingredients)
{
	OrderManager->PlayerUI->OrderListViewer->CheckOrderSuccess(ingredients);
}
