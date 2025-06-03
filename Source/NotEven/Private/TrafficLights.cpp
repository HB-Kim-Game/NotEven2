// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLights.h"

#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"

// Sets default values
ATrafficLights::ATrafficLights()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);

	RedLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("RedLight"));
	RedLight->SetupAttachment(BoxComp);
	GreenLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("GreenLight"));
	GreenLight->SetupAttachment(BoxComp);
	
}

// Called when the game starts or when spawned
void ATrafficLights::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrafficLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrafficLights::SetGreen()
{
}

void ATrafficLights::SetRed()
{
}

