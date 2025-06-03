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

	// 시작 시 랜덤한 시간(10-20초)마다 장애물이 생성되게
	float minTime = 10.f; // 최소시간
	float maxTime = 20.f; // 최대시간
	float Timer = FMath::RandRange(minTime, maxTime); 
	GetWorld()->GetTimerManager().SetTimer(MakeVehicleTimer,this,&AVehicleManager::MakeVehicle,Timer,true);
	
}
// 장애물 만드는 함수
void AVehicleManager::MakeVehicle()
{
	GetWorld()->SpawnActor<AVehicle>(VehicleFactory,SpawnPoint->GetComponentTransform());
}

// Called every frame
void AVehicleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
