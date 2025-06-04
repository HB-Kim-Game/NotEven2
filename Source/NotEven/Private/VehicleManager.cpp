// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleManager.h"
#include "TrafficLights.h"
#include "Vehicle.h"
#include "Components/ArrowComponent.h"
#include "GameManager/OrderManager.h"
#include "Kismet/GameplayStatics.h"

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

	if (HasAuthority())
	{
		if (auto orderManager = Cast<AOrderManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AOrderManager::StaticClass())))
		{
			orderManager->OnGameStart.Add(FSimpleDelegate::CreateLambda([&]()
			{
				StartMakeVehicle();
			}));
		}
	}
	SetRedLight();
}

void AVehicleManager::StartMakeVehicle()
{
	float late = FMath::RandRange(MinTime, MaxTime);
	// 시작 시 랜덤한 시간(7.5-12.5초)마다 장애물이 생성되게
	
	GetWorld()->GetTimerManager().SetTimer(YellowTimer, this, &AVehicleManager::SetYellowLight, FMath::Max(late - 1.f, 0.f), false);
	GetWorld()->GetTimerManager().SetTimer(MakeVehicleTimer,this,&AVehicleManager::MakeVehicle,late, false);
}

// 장애물 만드는 함수
void AVehicleManager::MakeVehicle()
{
	float late = FMath::RandRange(MinTime, MaxTime);
	
	GetWorld()->SpawnActor<AVehicle>(VehicleFactory,SpawnPoint->GetComponentTransform());
	
	SetGreenLight();
	GetWorld()->GetTimerManager().SetTimer(RedTimer, this, &AVehicleManager::SetRedLight, 3.f, false);
	GetWorld()->GetTimerManager().SetTimer(YellowTimer, this, &AVehicleManager::SetYellowLight, FMath::Max(late - 1.f, 0.f), false);
	GetWorld()->GetTimerManager().SetTimer(MakeVehicleTimer,this,&AVehicleManager::MakeVehicle,late, false);
}

void AVehicleManager::SetYellowLight()
{
	for (auto light : TrafficLights)
	{
		light->SetYellow();
	}
}

void AVehicleManager::SetGreenLight()
{
	for (auto light : TrafficLights)
	{
		light->SetGreen();
	}
}

void AVehicleManager::SetRedLight()
{
	for (auto light : TrafficLights)
	{
		light->SetRed();
	}
}

// Called every frame
void AVehicleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
