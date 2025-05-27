// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"

APlate::APlate()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/plate.plate'"));
	if (tempMesh.Succeeded())
	{
		MeshComp -> SetStaticMesh(tempMesh.Object);
	}
	MeshComp->SetRelativeLocation(FVector(0, 0, -12.5));
}

void APlate::BeginPlay()
{
	Super::BeginPlay();
}

void APlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
