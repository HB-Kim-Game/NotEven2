// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleManager.h"

#include "Vehicle.h"
#include "Components/ArrowComponent.h"

// Sets default values
AVehicleManager::AVehicleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AVehicleManager::BeginPlay()
{
	Super::BeginPlay();

	float minTime = 5.f;
	float maxTime = 10.f;
	float Timer = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(MakeVehicleTimer,this,&AVehicleManager::MakeVehicle,Timer,true);
	
}

// Called every frame
void AVehicleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVehicleManager::MakeVehicle()
{
	GetWorld()->SpawnActor<AVehicle>(VehicleFactory,SpawnPoint->GetComponentTransform());
}

