// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodIngredient.h"

#include "Components/BoxComponent.h"

AFoodIngredient::AFoodIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	BoxComp->SetSimulatePhysics(false);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	BoxComp->SetCollisionProfileName(FName("OverlapAllDynamic"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	MeshComp->SetCollisionProfileName(FName("BlockAllDynamic"));
}

void AFoodIngredient::BeginPlay()
{
	Super::BeginPlay();
}

void AFoodIngredient::InitializeIngredient(FIngredientData data, FIngredientPlaceData place)
{
	Data = data;
	PlaceData = place;

	if (Data.MeshPaths.Num() <= 0) return;
	
	for (auto path : Data.MeshPaths)
	{
		if (UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path.StaticMeshPath))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *path.StaticMeshPath);
			IngredientMeshes.Add(path.State, mesh);	
		}
	}

	SetState(EIngredientState::None);
}

FIngredientData AFoodIngredient::GetIngredientData() const
{
	return Data;
}

EIngredientState AFoodIngredient::GetIngredientState() const
{
	return CurrentState;
}

TArray<FIngredientPlaceRule> AFoodIngredient::GetIngredientPlaceRules() const
{
	return PlaceData.PlacementRules;
}

void AFoodIngredient::AddCookingProgress(float addProgress)
{
	CurrentCookingProgress += addProgress;
}

float AFoodIngredient::GetNormalizedCookingProgress() const
{
	return CurrentCookingProgress / MaxCookingProgress;
}

void AFoodIngredient::SetState(EIngredientState newState)
{
	CurrentState = newState;

	if (auto mesh = IngredientMeshes.Find(newState))
	{
		if (*mesh)
		{
			MeshComp->SetStaticMesh(*mesh);	
		}
	}
	
	CurrentCookingProgress = 0.0f;
}

void AFoodIngredient::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	
}

