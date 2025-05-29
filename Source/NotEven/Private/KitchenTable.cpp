// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenTable.h"

AKitchenTable::AKitchenTable()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh
	(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/kitchencounter_straight_B.kitchencounter_straight_B'"));

	if (tempMesh.Succeeded())
	{
		MeshComp ->SetStaticMesh(tempMesh.Object);
	}

	MeshComp->SetRelativeLocation(FVector(0,0,-50));
}

void AKitchenTable::BeginPlay()
{
	Super::BeginPlay();
}

void AKitchenTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
