// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyCamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALobbyCamera::ALobbyCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp=CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SceneComp->SetupAttachment(RootComponent);

	CameraComp=CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SceneComp);
}

// Called when the game starts or when spawned
void ALobbyCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALobbyCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto pc = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (!pc)
		return;

	AActor* currentTarget = pc->GetViewTarget();
	if (currentTarget != this)
	{
		pc->SetViewTargetWithBlend(this,0,VTBlend_Linear,0,false);
	}
}

