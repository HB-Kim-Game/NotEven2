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

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/KHB/Models/trafficlight_A.trafficlight_A'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}

	RedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RedLight"));
	RedLight->SetupAttachment(BoxComp);
	RedLight->SetRelativeLocation(FVector(11.5f, 0.f, 34.f));
	RedLight->AttenuationRadius = 35.f;
	
	GreenLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("GreenLight"));
	GreenLight->SetupAttachment(BoxComp);
	GreenLight->SetRelativeLocation(FVector(11.5f, 0.f, 20.f));
	GreenLight->AttenuationRadius = 35.f;
	
	YellowLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("YellowLight"));
	YellowLight->SetRelativeLocation(FVector(11.5f, 0.f, 27.f));
	YellowLight->SetupAttachment(BoxComp);
	YellowLight->AttenuationRadius = 35.f;

	BoxComp->SetRelativeScale3D(FVector(7.5f));
	BoxComp->SetBoxExtent(FVector(17.f, 17.f, 35.f));

	MeshComp->SetRelativeLocationAndRotation(FVector(0, 0, -35.f), FRotator(0, -90.f, 0));

	RedLight->SetVisibility(false);
	GreenLight->SetVisibility(false);
	YellowLight->SetVisibility(false);

	bReplicates = true;
	SetReplicates(true);
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


void ATrafficLights::SetYellow_Implementation()
{
	YellowLight->SetVisibility(true);
	RedLight->SetVisibility(false);
	GreenLight->SetVisibility(false);
}

void ATrafficLights::SetGreen_Implementation()
{
	GreenLight->SetVisibility(true);
	RedLight->SetVisibility(false);
	YellowLight->SetVisibility(false);
}


void ATrafficLights::SetRed_Implementation()
{
	RedLight->SetVisibility(true);
	GreenLight->SetVisibility(false);
	YellowLight->SetVisibility(false);
}

