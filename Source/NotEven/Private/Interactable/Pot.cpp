// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Pot.h"

#include "SubmitFood.h"
#include "Net/UnrealNetwork.h"

APot::APot()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/pot_A.pot_A'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}

	MeshComp->SetRelativeLocation(FVector(0, 0, -25.f));

	bReplicates = true;
}

void APot::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->SetSimulatePhysics(false);
}

void APot::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
}

void APot::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	// Plate 처럼
}

void APot::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APot, SubmitFood);
}

void APot::AddProgress(float progress)
{
	SubmitFood->AddProgress(progress);
}

bool APot::HasSubmitFood()
{
	return SubmitFood != nullptr;
}
