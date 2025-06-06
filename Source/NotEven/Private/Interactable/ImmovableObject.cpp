// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmovableObject.h"

#include "Components/BoxComponent.h"

// Sets default values
AImmovableObject::AImmovableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetBoxExtent(FVector(100, 100, 50));
	BoxComp-> SetCollisionProfileName(TEXT("UnGrabObj"));
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp -> SetupAttachment(BoxComp);
	MeshComp->SetRelativeLocation(FVector(0,0,-50));
	
}

void AImmovableObject::Interact(class ANotEvenPlayer* player)
{
	
}

// Called when the game starts or when spawned
void AImmovableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AImmovableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

