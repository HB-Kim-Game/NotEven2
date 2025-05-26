// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashBox.h"

#include "Components/BoxComponent.h"

ATrashBox::ATrashBox()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp -> SetupAttachment(BoxComp);
	MeshComp -> SetRelativeLocation(FVector(0, 0, -20));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		MeshComp-> SetStaticMesh(tempMesh.Object); 
	}
}

void ATrashBox::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	
	
}

void ATrashBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrashBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

